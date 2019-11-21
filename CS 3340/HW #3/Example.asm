#Homework 3

	.data
prompt: .asciiz "Enter some text: "
length: .word 100
input: .space 100 
charCnt: .word 0
wordCnt: .word 0
wordMsg: .asciiz " word(s) "
characterMsg: .asciiz " character(s) \n"
goodbye: .asciiz "Good Bye."
Exit: .asciiz "Message: "

	.text
Loop:	la $a0, prompt  #show a dialog box to user to get input input
	la $a1, input
	lw $a2, length
	li $v0, 54
	syscall
	
	#check the input
	beq $a1, -2, EndLoop 
	beq $a1, -3, EndLoop 
	lw $a1, length
	la $a0, input
	jal count # call the count 
	
	#The count of characters and words stored in the listed registers
	sw $v0, charCnt
	sw $v1, wordCnt
	
	#display the input and counts
	la $a0, input #displaying input
	li $v0, 4
	syscall
	
	
	lw $a0, wordCnt		#wordCount Display
	li $v0, 1
	syscall
	
	la $a0, wordMsg
	li $v0, 4
	syscall
	
	
	lw $a0, charCnt		#characterCount Display
	li $v0, 1
	syscall
	
	la $a0, characterMsg
	li $v0, 4
	syscall
	
	b Loop
	#Display GoodBye
EndLoop:la $a0, Exit
	la $a1, goodbye
	li $v0, 59
	syscall

	li $v0, 10
	syscall
count:
	addi $sp, $sp, -4 	#save register on stack
	sw $s0, ($sp)
	move $s0, $a0
	li $t1, 0 #char count
	li $t2, 1 #word count
Loop2:	
	lb $t3, ($s0)
	beq $t3, '\n', Reset #goto endloop on seeing newline or null terminator
	beq $t3, '\0', Reset
	add $t1, $t1, 1 #increment char count
	beq $t3, ' ', wordCounter
	b nextIndex
wordCounter:
	addi $t2, $t2, 1
nextIndex:
	addi $s0, $s0, 1
	b Loop2
Reset:
	lw $s0, ($sp) #restores $s position in stack
	add $sp, $sp, 4
	move $v0, $t1
	move $v1, $t2
	jr $ra


	
	
