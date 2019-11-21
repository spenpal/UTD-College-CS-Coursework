# MACRO FUNCTIONS (Help Ease The Flow Of The Code)
	# Print An Integer
	.macro printInt(%int)
		lw $a0, %int
		li $v0, 1
		syscall
	.end_macro
	
	# Print A String
	.macro printStr(%str) 		
		li $v0, 4      		
		la $a0, %str  		
		syscall    
	.end_macro
	
	# Load String Character(s) Into Buffer
	.macro loadChars (%int, %str)
		la %int, %str
	.end_macro

	# Loading Text File Name
	.macro inputFile
		la $a0, inputName
	.end_macro

	# Open The File
	.macro openFile
		li $v0, 13      
    	 	li $a1, 0       
    		li $a2, 0       
    	 	syscall
    	.end_macro
	
	# Store Characters
	.macro storeChars
		li $t7, 0
		store:
			sb $t1, ($a1) 		# Storing Byte
			addi $t7, $t7, 1 	# Counter++
			addi $a1, $a1, 1 	# Point To Next Byte
			addi $t0, $t0, 1 	# Size++
			bne $t7, $t3, store
	.end_macro
	
	# Comparsion of Digits
	.macro checkDigits
		beq $t2, $t3, FLoopDos 		# If (character == characterNext), go to FLoopDos
		sb $t2, ($a1)			# Store Char Into bufferTwo
		addi $a1, $a1, 1
		li $t6, 0 			# Counter = 0;
		bgt $t1, 9, convertDigits
	.end_macro
	
	# Return Function
	.macro return
		jr $ra
	.end_macro