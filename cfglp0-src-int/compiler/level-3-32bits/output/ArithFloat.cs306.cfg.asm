
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
	li.d $f2, 2.30
	s.d $f2, -16($fp)
	li.d $f2, 3.40
	s.d $f2, -8($fp)
	l.d $f2, -16($fp)
	l.d $f4, -8($fp)
	mul.d $f6, $f2, $f4
	l.d $f2, -16($fp)
	div.d $f4, $f6, $f2
	s.d $f4, 0($fp)
	j epilogue_main

# Epilogue Begins
epilogue_main:
	add $sp, $sp, 32
	lw $fp, -4($sp)  
	lw $ra, 0($sp)   
	jr        $31		# Jump back to the called procedure
# Epilogue Ends

