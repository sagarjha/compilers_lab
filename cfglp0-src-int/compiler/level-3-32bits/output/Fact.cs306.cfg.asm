
	.data
prod:	.word 0

	.text 			# The .text assembler directive indicates
	.globl fact		# The following is the code (as oppose to data)
fact:				# .globl makes main know to the 
				# outside of the program.
# Prologue begins 
	sw $ra, 0($sp)		# Save the return address
	sw $fp, -4($sp)		# Save the frame pointer
	sub $fp, $sp, 8		# Update the frame pointer

	sub $sp, $sp, 8		# Make space for the locals
# Prologue ends


label2:    	
	lw $v0, 12($fp)
	li $t0, 1
	seq $t1, $v0, $t0
	bne $t1, $zero, label3 
	j label4

label3:    	
	li $v0, 1
	move $v1, $v0
	j epilogue_fact

label4:    	
	lw $v0, prod
	lw $t0, 12($fp)
	mul $t1, $v0, $t0
	sw $t1, prod
	lw $v0, 12($fp)
	li $t0, -1
	add $t1, $v0, $t0
	sw $t1, 0($sp)
	sub $sp, $sp, 4
	jal fact
	add $sp, $sp, 4
	move $v0, $v1
	j epilogue_fact

# Epilogue Begins
epilogue_fact:
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
	li $v0, 1
	sw $v0, prod
	lw $v0, 0($fp)
	sw $v0, 0($sp)
	sub $sp, $sp, 4
	jal fact
	add $sp, $sp, 4
	move $v0, $v1
	j epilogue_main

# Epilogue Begins
epilogue_main:
	add $sp, $sp, 12
	lw $fp, -4($sp)  
	lw $ra, 0($sp)   
	jr        $31		# Jump back to the called procedure
# Epilogue Ends

