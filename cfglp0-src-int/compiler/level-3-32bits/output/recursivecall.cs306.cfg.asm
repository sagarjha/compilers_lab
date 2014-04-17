
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
	li $v0, 5
	sw $v0, -8($fp)
	lw $v0, -8($fp)
	sw $v0, 0($sp)
	sub $sp, $sp, 4
	jal r
	add $sp, $sp, 4
	move $v0, $v1
	sw $v0, -4($fp)
	li $v0, 3
	sw $v0, 0($sp)
	sub $sp, $sp, 4
	jal r
	add $sp, $sp, 4
	move $v0, $v1
	sw $v0, 0($fp)
	j epilogue_main

# Epilogue Begins
epilogue_main:
	add $sp, $sp, 20
	lw $fp, -4($sp)  
	lw $ra, 0($sp)   
	jr        $31		# Jump back to the called procedure
# Epilogue Ends


	.text 			# The .text assembler directive indicates
	.globl r		# The following is the code (as oppose to data)
r:				# .globl makes main know to the 
				# outside of the program.
# Prologue begins 
	sw $ra, 0($sp)		# Save the return address
	sw $fp, -4($sp)		# Save the frame pointer
	sub $fp, $sp, 8		# Update the frame pointer

	sub $sp, $sp, 16		# Make space for the locals
# Prologue ends


label2:    	
	lw $v0, 12($fp)
	li $t0, 1
	seq $t1, $v0, $t0
	bne $t1, $zero, label3 
	j label4

label3:    	
	li $v0, 1
	sw $v0, -4($fp)
	j label5

label4:    	
	lw $v0, 12($fp)
	li $t0, -1
	add $t1, $v0, $t0
	sw $t1, 0($sp)
	sub $sp, $sp, 4
	jal r
	add $sp, $sp, 4
	move $v0, $v1
	sw $v0, 0($fp)
	lw $v0, 0($fp)
	li $t0, 1
	add $t1, $v0, $t0
	sw $t1, -4($fp)

label5:    	
	lw $v0, -4($fp)
	move $v1, $v0
	j epilogue_r

# Epilogue Begins
epilogue_r:
	add $sp, $sp, 16
	lw $fp, -4($sp)  
	lw $ra, 0($sp)   
	jr        $31		# Jump back to the called procedure
# Epilogue Ends

