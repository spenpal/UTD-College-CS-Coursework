# Name: Sanjeev Penupala
# Date: 2/17/19
# Assignment: Homework #3

	.data
prompt: 	.asciiz 	"Enter some text: "
input: 		.space 		100
length: 	.word 		100
numChars: 	.word 		0
numWords: 	.word 		0
wordMsg: 	.asciiz 	" words "
charMsg: 	.asciiz 	" characters \n"
exit: 		.asciiz 	"Exit Msg: "
goodbye:	.asciiz 	"Goodbye!"

	.text
	# Get Input Through Dialog Box
L1:	la $a0, prompt  	# $a0 = "Enter some text: "
	la $a1, input 		# $a1 = 100 bytes
	lw $a2, length		# $a2 = 100
	li $v0, 54		# Dialog Box
	syscall
	
	# Checking Input
	beq $a1, -2, end
	beq $a1, -3, end
	lw $a1, length
	la $a0, input
	jal count # Call Function "Count()"
	
	# Store charCount and wordCount from return registers
	sw $v0, numChars
	sw $v1, numWords
	
	# Display Word(s) Input
	la $a0, input
	li $v0, 4
	syscall
	
	# Display wordCount 
	lw $a0, numWords		
	li $v0, 1
	syscall
	la $a0, wordMsg
	li $v0, 4
	syscall
	
	# Display charCount 
	lw $a0, numChars		
	li $v0, 1
	syscall
	la $a0, charMsg
	li $v0, 4
	syscall
	
	b L1 #Restart Loop
	
end:	# Display GoodBye Message
	la $a0, exit
	la $a1, goodbye
	li $v0, 59
	syscall

	li $v0, 10
	syscall
	
count:	#Counting The Number of Characters And Words In The String
	addi $sp, $sp, -4	# Intializing the Stack For 4 Bytes
	sw $s1, ($sp)		# Storing $s1's value on the stack
	move $s1, $a0		# Copying Contents From $a0 to $s1
	li $t1, 0 		# $t1 = 0 [Char Count]
	li $t2, 1 		# $t2 = 1 [Word Count]
	
L2:	#The Place Where You Actually Count The Number of Characters And Words In The String
	lb $t3, ($s1)
	beq $t3, '\n', restart 	# Check to See If There Is A Newline Char And Ends L2
	beq $t3, '\0', restart 	# Check to See If There Is A Null Char And And Ends L2
	add $t1, $t1, 1 	# charCount++
	beq $t3, ' ', countWords
	b next
	
countWords: # If There Is A Space... = wordCount++
	addi $t2, $t2, 1
	
next: 	# Increase Index "i" Of The String
	addi $s1, $s1, 1
	b L2
	
restart: # End Of String And Updates Variables
	lw $s1, ($sp) 		# Pop Off The Stack
	add $sp, $sp, 4 	# Re-Adjust The Stack
	move $v0, $t1		# $v0 = $t1
	move $v1, $t2		# $v1 = $t2
	jr $ra			# Go Back to L1
