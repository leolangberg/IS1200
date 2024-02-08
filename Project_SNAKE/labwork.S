.global enable_interrupt
.global delay
.global time2string
  # labwork.S
  # Written 2015-2017 by F Lundevall
  # Skeleton file for IS1200/IS1500 lab 1.
  # The macros PUSH and POP are in the public domain.
  # Please add your own code at the end of the file.

  #
  # Please keep the two macros PUSH and POP unchanged
  #




###################################################
.macro	PUSH reg
	addi	$sp,$sp,-4
	sw	\reg,0($sp)
.endm

.macro	POP reg  
	lw	\reg,0($sp)  
	addi	$sp,$sp,4
.endm


  #
  # Please add your own code below this line
  #

#######################################################
.data
.text

hexasc:
    andi $t0, $a0, 0x0f # extract 4 LSBs of $a0
    addi $t1, $zero, 48 # ASCII code for '0'
    blt $t0, 10, digit_case # branch if $t0 is between 0 and 9
    nop
    addi $t1, $t1, 7 # adjust ASCII code for letters 'A' to 'F'
    add $v0, $t0, $t1
    jr $ra
    nop
digit_case:
    add $v0, $t0, $t1
    jr $ra
    nop
    
    
delay:

    li    $t0, 0   # i = 0
    bgt     $a0, $zero, while  # if (ms > 0) go to while
    nop
    j       exit         # else go to exit
    nop
    
while:

    addi    $a0, $a0, -1   # ms = ms - 1
    bgt     $a0, $zero, while  # if (ms > 0) go to while
    nop
    j       for         # else go to loop2
    nop
    
for:

    addi    $t0, $t0, 1    # i = i + 1
    blt     $t0, 4711000, for  # if (i < 4711) go to for   4711*1000
    nop
    j       exit         # else go to exit
    nop
    
exit:
    jr      $ra           # return
    nop 

time2string: 



    PUSH $ra
    PUSH $s0
    PUSH $s1

    #addi $sp,$sp,-4
    #sw  $ra,0($sp)


    la $s0, ($a0) # We will use s0 to load the address from a0
    la $s1, ($a1)

    andi    $t2, $a1, 0xF000 # Extract first digit from a1
    srl     $a0, $t2, 12     # Shift by 12 bits for the first digit and send to a0
    jal     hexasc           # Convert to ASCII
    nop          ###############
    sb      $v0, 0($s0)      # Store result in memory

    andi    $t3, $a1, 0xF00  # Extract second digit from a1
    srl     $a0, $t3, 8      # Shift by 8 bits for the second digit and send to a0
    jal     hexasc           # Convert to ASCII
    nop
    sb      $v0, 1($s0)      # Store result in memory

    li $t0, 0x3A       # Get ASCII code for colon
    sb $t0, 2($s0)       # Store result in memory

    andi    $t4, $a1, 0xF0   # Extract third digit from a1
    srl     $a0, $t4, 4      # Shift by 4 bits for the third digit and send to a0
    jal     hexasc           # convert to ASCII
    nop
    sb      $v0, 3($s0)      # Store result in memory
    
    andi    $t5, $a1, 0xF    # Extract fourth digit from a1
    move    $a0, $t5         # Here we don't need to shift
    jal     hexasc           # convert to ASCII
    nop
    sb      $v0, 4($s0)      # Store result in memory

    li $a0, 0x00       # ASCII code for NUll-byte
    sb $a0, 5($s0)       # Store result in memory
    
    POP $s1
    POP $s0
    POP $ra

    #lw  $ra,0($sp)  
    #addi    $sp,$sp,4
    

    jr      $ra
    nop




enable_interrupt:
    ei
    jr $ra
    nop

