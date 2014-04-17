
	.text 			# The .text assembler directive indicates
	.globl main		# The following is the code (as oppose to data)
main:				# .globl makes main know to the 
				# outside of the program.
# Prologue begins 
	sw $ra, 0($sp)		# Save the return address
	sw $fp, -4($sp)		# Save the frame pointer
	sub $fp, $sp, 8		# Update the frame pointer

	sub $sp, $sp, 20		# Make space for the locals
# Prologue ends


label2:    	
	li $v0, 3
	sw $v0, -8($fp)
	li $v0, 8
	sw $v0, -4($fp)
	lw $v0, -4($fp)
	lw $t0, -8($fp)
	add $t1, $v0, $t0
	lw $v0, -8($fp)
	sle $t0, $t1, $v0
	lw $v0, -4($fp)
	seq $t1, $t0, $v0
	sw $t1, 0($fp)
	j epilogue_main

# Epilogue Begins
epilogue_main:
	add $sp, $sp, 20
	lw $fp, -4($sp)  
	lw $ra, 0($sp)   
	jr        $31		# Jump back to the called procedure
# Epilogue Ends

