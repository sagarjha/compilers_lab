
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
	li.d $f2, 2.30
	s.d $f2, 0($fp)
	l.d $f2, 0($fp)
	li.d $f4, 3.00
	add.d $f6, $f2, $f4
	mfc1 $v0, $f6
	sw $v0, -8($fp)
	lw $v0, -8($fp)
	mtc1 $f2, $v0
	li.d $f4, 2.30
	add.d $f6, $f2, $f4
	s.d $f6, 0($fp)
	l.d $f2, 0($fp)
	li.d $f4, 4.30
	add.d $f6, $f2, $f4
	mfc1 $v0, $f6
	sw $v0, -8($fp)
	lw $v0, -8($fp)
	li $t0, 5
	add $t1, $v0, $t0
	mtc1 $f2, $t1
	s.d $f2, 0($fp)
	j epilogue_main

# Epilogue Begins
epilogue_main:
	add $sp, $sp, 20
	lw $fp, -4($sp)  
	lw $ra, 0($sp)   
	jr        $31		# Jump back to the called procedure
# Epilogue Ends

