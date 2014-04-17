
	.text 			# The .text assembler directive indicates
	.globl main		# The following is the code (as oppose to data)
main:				# .globl makes main know to the 
				# outside of the program.
# Prologue begins 
	sw $ra, 0($sp)		# Save the return address
	sw $fp, -4($sp)		# Save the frame pointer
	sub $fp, $sp, 8		# Update the frame pointer

	sub $sp, $sp, 64		# Make space for the locals
# Prologue ends


label2:    	
	li.d $f2, 2.30
	s.d $f2, -32($fp)
	li.d $f2, 3.40
	s.d $f2, -24($fp)
	li.d $f2, 4.50
	s.d $f2, -16($fp)
	li.d $f2, 5.60
	s.d $f2, -8($fp)
	li.d $f2, 6.70
	s.d $f2, 0($fp)
	l.d $f2, -24($fp)
	l.d $f4, -16($fp)
	add.d $f6, $f2, $f4
	l.d $f2, -32($fp)
	sle $v0, $f6, $f2
	li $t0, 0
	sne $t1, $v0, $t0
	bne $t1, $zero, label3 
	j label4

label3:    	
	li.d $f2, 1.00
	s.d $f2, -40($fp)
	j label5

label4:    	
	li.d $f2, 0.00
	s.d $f2, -40($fp)

label5:    	
	l.d $f2, -40($fp)
	l.d $f4, -8($fp)
	sgt $v0, $f2, $f4
	li $t0, 0
	sne $t1, $v0, $t0
	bne $t1, $zero, label6 
	j label7

label6:    	
	li.d $f2, 1.00
	s.d $f2, -48($fp)
	j label8

label7:    	
	li.d $f2, 0.00
	s.d $f2, -48($fp)

label8:    	
	l.d $f2, -48($fp)
	l.d $f4, 0($fp)
	l.d $f6, -8($fp)
	mul.d $f8, $f4, $f6
	l.d $f4, -16($fp)
	div.d $f6, $f8, $f4
	sgt $v0, $f2, $f6
	li $t0, 0
	sne $t1, $v0, $t0
	bne $t1, $zero, label9 
	j label10

label9:    	
	l.d $f2, -32($fp)
	l.d $f4, -8($fp)
	mul.d $f6, $f2, $f4
	l.d $f2, 0($fp)
	div.d $f4, $f6, $f2
	s.d $f4, -32($fp)
	j label11

label10:    	
	l.d $f2, 0($fp)
	l.d $f4, -8($fp)
	add.d $f6, $f2, $f4
	s.d $f6, 0($fp)

label11:    	
	j epilogue_main

# Epilogue Begins
epilogue_main:
	add $sp, $sp, 64
	lw $fp, -4($sp)  
	lw $ra, 0($sp)   
	jr        $31		# Jump back to the called procedure
# Epilogue Ends

