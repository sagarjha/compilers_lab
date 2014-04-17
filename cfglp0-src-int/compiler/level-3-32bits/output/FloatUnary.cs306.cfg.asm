
	.text 			# The .text assembler directive indicates
	.globl main		# The following is the code (as oppose to data)
main:				# .globl makes main know to the 
				# outside of the program.
# Prologue begins 
	sw $ra, 0($sp)		# Save the return address
	sw $fp, -4($sp)		# Save the frame pointer
	sub $fp, $sp, 8		# Update the frame pointer

	sub $sp, $sp, 32		# Make space for the locals
# Prologue ends


label2:    	
	li.d $f2, 3.23
	s.d $f2, -8($fp)
	li.d $f2, 5.68
	s.d $f2, 0($fp)
	l.d $f2, 0($fp)
	neg.d $f4, $f2
	l.d $f2, -8($fp)
	mul.d $f6, $f4, $f2
	l.d $f2, -8($fp)
	div.d $f4, $f6, $f2
	l.d $f2, 0($fp)
	sub.d $f6, $f4, $f2
	s.d $f6, -8($fp)
	l.d $f2, 0($fp)
	neg.d $f4, $f2
	l.d $f2, -8($fp)
	sle $v0, $f4, $f2
	li $t0, 0
	sne $t1, $v0, $t0
	bne $t1, $zero, label3 
	j label4

label3:    	
	li.d $f2, 1.00
	s.d $f2, -16($fp)
	j label5

label4:    	
	li.d $f2, 0.00
	s.d $f2, -16($fp)

label5:    	
	l.d $f2, -16($fp)
	l.d $f4, -8($fp)
	neg.d $f6, $f4
	slt $v0, $f2, $f6
	li $t0, 0
	sne $t1, $v0, $t0
	bne $t1, $zero, label6 
	j label7

label6:    	
	l.d $f2, -8($fp)
	li.d $f4, 1.00
	add.d $f6, $f2, $f4
	s.d $f6, -8($fp)
	j label8

label7:    	
	l.d $f2, -8($fp)
	neg.d $f4, $f2
	li.d $f2, 1.00
	sub.d $f6, $f4, $f2
	s.d $f6, 0($fp)

label8:    	
	j epilogue_main

# Epilogue Begins
epilogue_main:
	add $sp, $sp, 32
	lw $fp, -4($sp)  
	lw $ra, 0($sp)   
	jr        $31		# Jump back to the called procedure
# Epilogue Ends

