 
  # timetemplate.asm
  # Written 2015 by F Lundevall
  # Copyright abandonded - this file is in the public domain.

.macro	PUSH (%reg)
	addi	$sp,$sp,-4
	sw	%reg,0($sp)
.end_macro

.macro	POP (%reg)
	lw	%reg,0($sp)
	addi	$sp,$sp,4
.end_macro

	.data
	.align 2
mytime:	.word 0x5957
timstr:	.ascii "text more text lots of text\0"
	.text
main:
	# print timstr
	la	$a0,timstr
	li	$v0,4
	syscall
	nop
	# wait a little
	li	$a0,2     #2
	jal	delay
	nop
	# call tick
	la	$a0,mytime
	jal	tick
	nop
	# call your function time2string
	la	$a0,timstr
	la	$t0,mytime
	lw	$a1,0($t0)
	jal	time2string
	nop
	# print a newline
	li	$a0,10
	li	$v0,11
	syscall
	nop
	# go back and do it all again
	j	main
	nop
# tick: update time pointed to by $a0
tick:	lw	$t0,0($a0)	# get time
	addiu	$t0,$t0,1	# increase
	andi	$t1,$t0,0xf	# check lowest digit
	sltiu	$t2,$t1,0xa	# if digit < a, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0x6	# adjust lowest digit
	andi	$t1,$t0,0xf0	# check next digit
	sltiu	$t2,$t1,0x60	# if digit < 6, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0xa0	# adjust digit
	andi	$t1,$t0,0xf00	# check minute digit
	sltiu	$t2,$t1,0xa00	# if digit < a, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0x600	# adjust digit
	andi	$t1,$t0,0xf000	# check last digit
	sltiu	$t2,$t1,0x6000	# if digit < 6, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0xa000	# adjust last digit
tiend:	sw	$t0,0($a0)	# save updated result
	jr	$ra		# return
	nop

  # you can write your code for subroutine "hexasc" below this line
  #
hexasc:
    andi $t0, $a0, 0xf # 4 LSBs of $a0
    addi $t1, $zero, 0x30 # ASCII  '0'
    blt $t0, 10, lownum # branch if $t0 < 10
    nop
    addi $t1, $t1, 0x7 #  ASCII  'A' to 'F' (0x30 + 0x7)
    add $v0, $t0, $t1
    jr $ra
    nop
lownum:   #(0-9)
    add $v0, $t0, $t1
    jr $ra
    nop
    
    
delay:

    li      $t0, 0	   #i = 0
    bgt     $a0, $zero, while  # if (ms > 0) go to while
    nop
    j       exit         # else go to exit
    nop
    
while:
    addi    $a0, $a0, -1   # ms = ms - 1
    bgt     $a0, $zero, for  # if (ms > 0) go to for
    nop
    j       for         # else go to for
    nop
    
for:
    addi    $t0, $t0, 1    # i = i + 1
    blt     $t0, 4711, for  # if (i < 4711) go to for    (4711)  1000 --> 20
    nop
    j       delay         # else go to exit
    nop
    
exit:
    jr      $ra           # return
    nop	


time2string: 



    PUSH ($ra)
    PUSH ($s0)
    PUSH ($s1)


    la $s0, ($a0) # We will use s0 to load the address from a0
    la $s1, ($a1)

    andi    $t2, $s1, 0xF000 # Extract first digit from a1
    srl     $a0, $t2, 12     # Shift by 12 bits for the first digit and send to a0
    jal     hexasc           # Convert to ASCII
    nop			     ###############
    sb      $v0, 0($s0)      # Store result in memory

    andi    $t3, $s1, 0xF00  # Extract second digit from a1
    srl     $a0, $t3, 8      # Shift by 8 bits for the second digit and send to a0
    jal     hexasc           # Convert to ASCII
    nop
    sb      $v0, 1($s0)      # Store result in memory

    li $t0, 0x3A	     # Get ASCII code for colon
    sb $t0, 2($s0)	     # Store result in memory

    andi    $t4, $s1, 0xF0   # Extract third digit from a1
    srl     $a0, $t4, 4      # Shift by 4 bits for the third digit and send to a0
    jal     hexasc           # convert to ASCII
    nop
    sb      $v0, 3($s0)      # Store result in memory
       
    andi    $t5, $s1, 0xF    # Extract fourth digit from a1
    move    $a0, $t5         # Here we don't need to shift
    jal     hexasc           # convert to ASCII
    nop
    sb      $v0, 4($s0)      # Store result in memory

 ##############################


    
############################################

    li $a0, 0x00	     # ASCII code for NUll-byte
    sb $a0, 5($s0)	     # Store result in memory
    
    
    POP($s1)
    POP($s0)
    POP($ra)
    
    

    jr      $ra
    nop   
