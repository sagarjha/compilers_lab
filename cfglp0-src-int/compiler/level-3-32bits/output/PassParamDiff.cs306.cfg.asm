
	.text 			# The .text assembler directive indicates
	.globl fn		# The following is the code (as oppose to data)
fn:				# .globl makes main know to the 
				# outside of the program.
# Prologue begins 
	sw $ra, 0($sp)		# Save the return address
	sw $fp, -4($sp)		# Save the frame pointer
	sub $fp, $sp, 8		# Update the frame pointer

	sub $sp, $sp, 8		# Make space for the locals
# Prologue ends


label2:    	
	l.d $f2, 16($fp)
	mfc1 $v0, $f2
	move $v1, $v0
	j epilogue_fn

# Epilogue Begins
epilogue_fn:
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
	li $v0, 10
	sw $v0, 0($fp)
	li.d $f2, 10.00
	s.d $f2, 0($sp)
	lw $v0, 0($fp)
	mtc1 $f2, $v0
	s.d $f2, -8($sp)
	sub $sp, $sp, 16
	jal fn
	add $sp, $sp, 16
	move $v0, $v1
	j epilogue_main

# Epilogue Begins
epilogue_main:
	add $sp, $sp, 12
	lw $fp, -4($sp)  
	lw $ra, 0($sp)   
	jr        $31		# Jump back to the called procedure
# Epilogue Ends

