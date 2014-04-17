
	.text 			# The .text assembler directive indicates
	.globl main		# The following is the code (as oppose to data)
main:				# .globl makes main know to the 
				# outside of the program.
# Prologue begins 
	sw $ra, 0($sp)		# Save the return address
	sw $fp, -4($sp)		# Save the frame pointer
	sub $fp, $sp, 8		# Update the frame pointer

	sub $sp, $sp, 56		# Make space for the locals
# Prologue ends


label2:    	
	li $v0, 2
	sw $v0, -44($fp)
	li $v0, 3
	sw $v0, -40($fp)
	li $v0, 4
	sw $v0, -36($fp)
	li $v0, 5
	sw $v0, -32($fp)
	li.d $f2, 2.30
	s.d $f2, -24($fp)
	li.d $f2, 3.40
	s.d $f2, -16($fp)
	li.d $f2, 4.50
	s.d $f2, -8($fp)
	li.d $f2, 5.50
	s.d $f2, 0($fp)
	lw $v0, -44($fp)
	mtc1 $f2, $v0
	l.d $f4, -24($fp)
	l.d $f6, -16($fp)
	div.d $f8, $f4, $f6
	lw $v0, -32($fp)
	mtc1 $f4, $v0
	mul.d $f6, $f8, $f4
	add.d $f4, $f2, $f6
	mfc1 $v0, $f4
	sw $v0, -44($fp)
	lw $v0, -40($fp)
	mtc1 $f2, $v0
	lw $v0, -32($fp)
	mtc1 $f4, $v0
	l.d $f6, -8($fp)
	l.d $f8, 0($fp)
	mul.d $f10, $f6, $f8
	l.d $f6, -24($fp)
	div.d $f8, $f10, $f6
	add.d $f6, $f4, $f8
	sle $v0, $f2, $f6
	lw $t0, -44($fp)
	sne $t1, $v0, $t0
	bne $t1, $zero, label3 
	j label4

label3:    	
	lw $v0, -44($fp)
	li $t0, 1
	add $t1, $v0, $t0
	sw $t1, -44($fp)
	j label5

label4:    	
	l.d $f2, -24($fp)
	li.d $f4, 1.00
	add.d $f6, $f2, $f4
	s.d $f6, -24($fp)

label5:    	
	j epilogue_main

# Epilogue Begins
epilogue_main:
	add $sp, $sp, 56
	lw $fp, -4($sp)  
	lw $ra, 0($sp)   
	jr        $31		# Jump back to the called procedure
# Epilogue Ends

