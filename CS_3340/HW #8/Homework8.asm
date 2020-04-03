# Name: Sanjeev Penupala
# Class: CS 3340
# Section: 502

.include	"macros.asm"

.data
#Buffer Space
bufferOne: 		.space 1024
bufferTwo: 		.space 1024
bufferThree: 		.space 1024
bufferSpace: 		.space 1024

len:			.word 0
compressedLen: 		.word 0

#Prompts
newLine: 		.asciiz "\n"
inputName: 		.asciiz ""
errorPrompt: 		.asciiz "Error opening file. Project terminating. " 
prompt1: 		.asciiz "Please enter the filename to compress of <enter> to exit: "

#More Prompts
compressedSize: 	.asciiz "Compressed file size: "
originalSize:		.asciiz "Original file size: "
originalData: 		.asciiz "Original data:\n"
uncompressedData: 	.asciiz "Uncompressed data:\n"
compressedData: 	.asciiz "Compressed data:\n"

#------------------------------------------------------------------------------------------------------------
.text
main:	
	# Ask For File Name
	printStr(prompt1)
	
	# Input The File
	inputFile
		
	li $v0, 8
	li $a1, 21
	syscall
	
	inputFile
	lb $t1, ($a0)
	beq $t1, 10, end 
	
	# Using A Loop Counter
	li $t0, 0       	
    	li $t1, 21      	
	jal cleanse 				# Cleanse The File
	
	# Open File
	inputFile
	la $a1, bufferOne
	jal readFile 				# Read Functions
	
	loadChars($a0, bufferOne) 		
	li $t0, 0
	li $t2, 0	
	jal fileLen				# Get Buffer Size
	
	# Print Out Part 1 Of Output
	printStr (originalData)
	printStr (bufferOne) 
	printStr (newLine) 
	printStr (compressedData)
		
	#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	# Load Buffer Characters
	loadChars($a0, bufferOne) 		
	loadChars($a1, bufferTwo)	
	 
	lw $a2, len 				# Load "bufferOne" size
	li $t0, 0 				# Counter
	li $t5, 0 				# Compressed File Register
	jal loopChecker
	 		
	sw $v0, compressedLen 			# Save compressedFile size
	
	# Print Out Part 2 Of Output
	printStr (bufferTwo) 
	printStr (newLine) 
	printStr (uncompressedData)
	
	#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	loadChars($a0, bufferTwo) 		# Load "bufferTwo"
	loadChars($a1, bufferThree)		# Load "bufferThree"
	
	lw $a2, len 				# Load "bufferOne" size
	li $t0, 0
	jal uncompressionMain 			# Uncompression Procedures
	
	#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	# Print Out Part 3 Of Output
	printStr (bufferThree) 
	printStr (newLine) 
	printStr (originalSize) 
	printInt (len)
	printStr (newLine) 
	printStr (compressedSize)
	printInt (compressedLen)
	printStr (newLine) 
	printStr (newLine) 
	
	#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	# Repeat Process
	j main 
#------------------------------------------------------------------------------------------------------------
# USEFUL PROCEDURES
#///////////////////////////////////////////////////////////////////////////////////////////////
# Clean the Input Name
cleanse:
    	 beq $t0, $t1, returnToMain
    	 lb $t3, inputName($t0) 		# Load All Bytes From The File
    	 bne $t3, 0x0a, incrementer 		# Byte++
   	 sb $zero, inputName($t0) 
   	 
incrementer:
   	 addi $t0, $t0, 1 			
         j cleanse
         
returnToMain:
	 return
#///////////////////////////////////////////////////////////////////////////////////////////////
# File Reading Procedures
readFile:  
	# Open The Input File
    	openFile
    	move $s0, $v0      
    	bltz $v0, printErr  			# File Opening Error
    	 
   	li $v0, 14      
   	move $a0, $s0       
   	la $a1, bufferOne     			# Load Into First Buffer
   	li $a2, 1024				# Allocating Bytes
   	syscall
   	 
   	add $a1, $a1, $v0			# Creating an Empty Pointer
   	sb $0, ($a1) 	
   	li $v0, 16     
	move $a0, $s0     
	syscall

	return

fileLen:
	# For Loop
	lb $t1, ($a0)
	addi $t0, $t0, 1 			# Counter++
	addi $a0, $a0, 1 			# Increment Placement In Pointer
	beq  $t1, 13, updateLen 		# Check If Byte Char Is A newLine
	bne $t1, $zero, fileLen
	
updateLen:
	# Update The New Length
	addi $t2, $t2, 1			# Iterator
	bne $t1, $zero, fileLen
	sub $t0, $t0, $t2 			# Buffer Size = Buffer Size - New Lines
	sw $t0, len
	return
#///////////////////////////////////////////////////////////////////////////////////////////////	
# Compression Proecedures
loopChecker:
	beq $t0, $a2, exitCompression 		# Loop Checker, Otherwise Exit
	li $t1, 0
	 			
FLoopDos:
	lb $t2, ($a0)			
	beq $t2, 13, skipNL 	
	addi $t4, $a0, 1
	lb $t3, ($t4) 				# Load Bytes From bufferOne
	
	addi $t1, $t1, 1
	addi $a0, $a0, 1
	addi $t0, $t0, 1
	
 	checkDigits				# Checker For the Digits
 	
	addi $t1, $t1, 48
	sb $t1, ($a1) 				# Store Digit Number
	addi $a1, $a1, 1 
	addi $t5, $t5, 2 			# bufferSize++
	
	j loopChecker
	 			
skipNL:
	addi $a0, $a0, 1 			# Incrementing To Skip Over newLine characters
	j loopChecker
			
convertDigits:
	subi $t1, $t1, 10		
	addi $t6, $t6, 1 		
	bgt $t1, 10, convertDigits
	
	addi $t6, $t6, 48 			# Calculate ASCII Value
	sb $t6, ($a1) 				# Store Bytes Into bufferTwo
	addi $a1, $a1, 1
	addi $t1, $t1, 48 			# Calculate ASCII Value
	sb $t1, ($a1)			
	addi $a1, $a1, 1
	addi $t5, $t5, 2 			# size = size + 2
	
	j loopChecker

exitCompression:
	sb $0, ($a1) 				# Store newLine character at the end of the bufferTwo
	move $v0, $t5 			
	return
#///////////////////////////////////////////////////////////////////////////////////////////////
# Uncompression Procedures
uncompressionMain:
	beq $t0, $a2, exitUncompression 	# Exit If Iterator Reaches Max Size
	
	lb $t1, ($a0) 				# Load Bytes From bufferTwo
	addi $t2, $a0, 1 			# Byte++
	
	lb $t3, ($t2)
	subi $t3, $t3, 48			# Convert Back From ASCII Value (Decimal Value)
	
	addi $t4, $a0, 2 	
	lb $t5, ($t4) 		
	subi $t5, $t5, 48 			# Convert Back From ASCII Value (Decimal Value)
	li $t6, 0
	
decimalChecker:
	beq $t6, $t5, doubleDec			# If (Byte = Decimal Value), Go To doubleDec
	
	addi $t6, $t6, 1 	
	ble  $t6, 9, decimalChecker 		# If (Byte = Decimal Value), Repeat Loop
	
	storeChars
	
	addi $a0, $a0, 2 			# Byte = Byte + 2 (Traversing Through bufferTwo)
	
	j uncompressionMain
	
doubleDec:
	mul $t3, $t3, 10
	add $t3, $t3, $t5
	 			
	storeChars
	
	addi $a0, $a0, 3
	 	
	j uncompressionMain
	
exitUncompression:
	sb $0, ($a1) 				# Store newLine character at the end of the bufferThree
	return
#///////////////////////////////////////////////////////////////////////////////////////////////	 
# Error Procedure
printErr:
	printStr (errorPrompt)
#///////////////////////////////////////////////////////////////////////////////////////////////
# End Procedure	  
end:
	li $v0, 10
	syscall
