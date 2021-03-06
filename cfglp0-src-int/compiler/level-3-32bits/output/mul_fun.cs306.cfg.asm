
	.text 			# The .text assembler directive indicates
	.globl add		# The following is the code (as oppose to data)
add:				# .globl makes main know to the 
				# outside of the program.
# Prologue begins 
	sw $ra, 0($sp)		# Save the return address
	sw $fp, -4($sp)		# Save the frame pointer
	sub $fp, $sp, 8		# Update the frame pointer

	sub $sp, $sp, 8		# Make space for the locals
# Prologue ends


label2:    	
	l.d $f2, 16($fp)
	l.d $f4, 24($fp)
	add.d $f6, $f2, $f4
	move.d $f0, $f6
	j epilogue_add

# Epilogue Begins
epilogue_add:
	add $sp, $sp, 8
	lw $fp, -4($sp)  
	lw $ra, 0($sp)   
	jr        $31		# Jump back to the called procedure
# Epilogue Ends


	.text 			# The .text assembler directive indicates
	.globl div		# The following is the code (as oppose to data)
div:				# .globl makes main know to the 
				# outside of the program.
# Prologue begins 
	sw $ra, 0($sp)		# Save the return address
	sw $fp, -4($sp)		# Save the frame pointer
	sub $fp, $sp, 8		# Update the frame pointer

	sub $sp, $sp, 8		# Make space for the locals
# Prologue ends


label2:    	
	l.d $f2, 16($fp)
	l.d $f4, 24($fp)
	div.d $f6, $f2, $f4
	move.d $f0, $f6
	j epilogue_div

# Epilogue Begins
epilogue_div:
	add $sp, $sp, 8
	lw $fp, -4($sp)  
	lw $ra, 0($sp)   
	jr        $31		# Jump back to the called procedure
# Epilogue Ends


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
	li.d $f2, 2.00
	s.d $f2, -40($fp)
	li.d $f2, 3.00
	s.d $f2, -32($fp)
	l.d $f2, -32($fp)
	s.d $f2, 0($sp)
	l.d $f2, -40($fp)
	s.d $f2, -8($sp)
	sub $sp, $sp, 16
	jal add
	add $sp, $sp, 16
	move.d $f2, $f0
	s.d $f2, -24($fp)
	l.d $f2, -32($fp)
	s.d $f2, 0($sp)
	l.d $f2, -40($fp)
	s.d $f2, -8($sp)
	sub $sp, $sp, 16
	jal sub
	add $sp, $sp, 16
	move.d $f2, $f0
	s.d $f2, -16($fp)
	l.d $f2, -32($fp)
	s.d $f2, 0($sp)
	l.d $f2, -40($fp)
	s.d $f2, -8($sp)
	sub $sp, $sp, 16
	jal mul
	add $sp, $sp, 16
	move.d $f2, $f0
	s.d $f2, -8($fp)
	l.d $f2, -32($fp)
	s.d $f2, 0($sp)
	l.d $f2, -40($fp)
	s.d $f2, -8($sp)
	sub $sp, $sp, 16
	jal div
	add $sp, $sp, 16
	move.d $f2, $f0
	s.d $f2, 0($fp)
	j epilogue_main

# Epilogue Begins
epilogue_main:
	add $sp, $sp, 56
	lw $fp, -4($sp)  
	lw $ra, 0($sp)   
	jr        $31		# Jump back to the called procedure
# Epilogue Ends


	.text 			# The .text assembler directive indicates
	.globl mul		# The following is the code (as oppose to data)
mul:				# .globl makes main know to the 
				# outside of the program.
# Prologue begins 
	sw $ra, 0($sp)		# Save the return address
	sw $fp, -4($sp)		# Save the frame pointer
	sub $fp, $sp, 8		# Update the frame pointer

	sub $sp, $sp, 8		# Make space for the locals
# Prologue ends


label2:    	
	l.d $f2, 16($fp)
	l.d $f4, 24($fp)
	mul.d $f6, $f2, $f4
	move.d $f0, $f6
	j epilogue_mul

# Epilogue Begins
epilogue_mul:
	add $sp, $sp, 8
	lw $fp, -4($sp)  
	lw $ra, 0($sp)   
	jr        $31		# Jump back to the called procedure
# Epilogue Ends


	.text 			# The .text assembler directive indicates
	.globl sub		# The following is the code (as oppose to data)
sub:				# .globl makes main know to the 
				# outside of the program.
# Prologue begins 
	sw $ra, 0($sp)		# Save the return address
	sw $fp, -4($sp)		# Save the frame pointer
	sub $fp, $sp, 8		# Update the frame pointer

	sub $sp, $sp, 8		# Make space for the locals
# Prologue ends


label2:    	
	l.d $f2, 16($fp)
	l.d $f4, 24($fp)
	sub.d $f6, $f2, $f4
	move.d $f0, $f6
	j epilogue_sub

# Epilogue Begins
epilogue_sub:
	add $sp, $sp, 8
	lw $fp, -4($sp)  
	lw $ra, 0($sp)   
	jr        $31		# Jump back to the called procedure
# Epilogue Ends

