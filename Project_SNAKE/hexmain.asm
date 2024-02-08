  # hexmain.asm
  # Written 2015-09-04 by F Lundevall
  # Copyright abandonded - this file is in the public domain.

	.text
main:
	li	$a0,10		# change this to test different values

	jal	hexasc		# call hexasc
	nop			# delay slot filler (just in case)	

	move	$a0,$v0		# copy return value to argument register

	li	$v0,11		# syscall with v0 = 11 will print out
	syscall			# one byte from a0 to the Run I/O window
	
stop:	j	stop		# stop after one run
	nop			# delay slot filler (just in case)

  # You can write your own code for hexasc here
  #
hexasc:
    andi $t0, $a0, 0xf # extract 4 LSBs of $a0        
    addi $t1, $zero, 0x30 # ASCII code for '0'
    blt $t0, 10, lownum # branch if $t0 < 10
    nop
    addi $t1, $t1, 0x7 # ASCII  'A' to 'F' (0x37)
    add $v0, $t0, $t1
    jr $ra
    nop
lownum:   #(0-9)
    add $v0, $t0, $t1
    jr $ra
    nop
    
