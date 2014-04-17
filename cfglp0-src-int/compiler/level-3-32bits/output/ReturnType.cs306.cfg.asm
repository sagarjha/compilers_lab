
	.text 			# The .text assembler directive indicates
	.globl fn2		# The following is the code (as oppose to data)
fn2:				# .globl makes main know to the 
				# outside of the program.
# Prologue begins 
	sw $ra, 0($sp)		# Save the return address
	sw $fp, -4($sp)		# Save the frame pointer
	sub $fp, $sp, 8		# Update the frame pointer

	sub $sp, $sp, 8		# Make space for the locals
# Prologue ends


label2:    	
	li $v0, 2
	sw $v0, 16($fp)
	lw $v0, 16($fp)
	move $v1, $v0
	j epilogue_fn2

# Epilogue Begins
epilogue_fn2:
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

	sub $sp, $sp, 12		# Make space for the locals
# Prologue ends


label2:    	
	li $v0, 3
	sw $v0, 0($fp)
	lw $v0, 0($fp)
	sw $v0, 0($sp)
	lw $v0, 0($fp)
	sw $v0, -4($sp)
	sub $sp, $sp, 8
	jal fn2
	add $sp, $sp, 8
	move $v0, $v1
	sw $v0, 0($fp)
	j epilogue_main

# Epilogue Begins
epilogue_main:
	add $sp, $sp, 12
	lw $fp, -4($sp)  
	lw $ra, 0($sp)   
	jr        $31		# Jump back to the called procedure
# Epilogue Ends

