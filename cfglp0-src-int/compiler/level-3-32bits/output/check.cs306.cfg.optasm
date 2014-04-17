
	.text 			# The .text assembler directive indicates
	.globl f		# The following is the code (as oppose to data)
f:				# .globl makes main know to the 
				# outside of the program.
# Prologue begins 
	sw $ra, 0($sp)		# Save the return address
	sw $fp, -4($sp)		# Save the frame pointer
	sub $fp, $sp, 8		# Update the frame pointer

	sub $sp, $sp, 8		# Make space for the locals
# Prologue ends


label2:    	
	lw $v0, 12($fp)
	lw $t0, 16($fp)
	add $t1, $v0, $t0
	sw $t1, 12($fp)
	li.d $f2, 3.00
	s.d $f2, 24($fp)
	j epilogue_f

# Epilogue Begins
epilogue_f:
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

	sub $sp, $sp, 8		# Make space for the locals
# Prologue ends


label2:    	
	li.d $f2, 1.00
	s.d $f2, 0($sp)
	li $v0, 2
	sw $v0, -8($sp)
	li $v0, 1
	sw $v0, -12($sp)
	sub $sp, $sp, 16
	jal f
	add $sp, $sp, 16
	j epilogue_main

# Epilogue Begins
epilogue_main:
	add $sp, $sp, 8
	lw $fp, -4($sp)  
	lw $ra, 0($sp)   
	jr        $31		# Jump back to the called procedure
# Epilogue Ends

