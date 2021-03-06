
	.text 			# The .text assembler directive indicates
	.globl main		# The following is the code (as oppose to data)
main:				# .globl makes main know to the 
				# outside of the program.
# Prologue begins 
	sw $ra, 0($sp)		# Save the return address
	sw $fp, -4($sp)		# Save the frame pointer
	sub $fp, $sp, 8		# Update the frame pointer

	sub $sp, $sp, 40		# Make space for the locals
# Prologue ends


label2:    	
	li $v0, 3
	sw $v0, -28($fp)
	li $v0, 4
	sw $v0, -24($fp)
	li $v0, 5
	sw $v0, -20($fp)
	lw $v0, -28($fp)
	lw $t0, -24($fp)
	lw $t1, -20($fp)
	add $t2, $t0, $t1
	div $t0, $v0, $t2
	sw $t0, -16($fp)
	lw $v0, -28($fp)
	lw $t0, -24($fp)
	div $t1, $v0, $t0
	lw $v0, -20($fp)
	add $t0, $t1, $v0
	sw $t0, -12($fp)
	lw $v0, -28($fp)
	lw $t0, -24($fp)
	add $t1, $v0, $t0
	lw $v0, -20($fp)
	div $t0, $t1, $v0
	sw $t0, -8($fp)
	lw $v0, -24($fp)
	lw $t0, -20($fp)
	sub $t1, $v0, $t0
	lw $v0, -28($fp)
	add $t0, $t1, $v0
	sw $t0, -4($fp)
	lw $v0, -28($fp)
	lw $t0, -24($fp)
	add $t1, $v0, $t0
	lw $v0, -20($fp)
	mul $t0, $t1, $v0
	lw $v0, -12($fp)
	lw $t1, -8($fp)
	sub $t2, $v0, $t1
	lw $v0, -28($fp)
	lw $t1, -24($fp)
	sub $t3, $v0, $t1
	sgt $v0, $t2, $t3
	div $t1, $t0, $v0
	sw $t1, 0($fp)
	j epilogue_main

# Epilogue Begins
epilogue_main:
	add $sp, $sp, 40
	lw $fp, -4($sp)  
	lw $ra, 0($sp)   
	jr        $31		# Jump back to the called procedure
# Epilogue Ends

