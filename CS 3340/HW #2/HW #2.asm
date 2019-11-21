# Name: Sanjeev Penupala
# Date: 2/3/19
# Section 502

.data

a:		.word 0							#variables for a, b, c
b:		.word 0
c:		.word 0
num1:		.word 0
num2:		.word 0
num3:		.word 0
name:		.space 30
newline: 	.asciiz "\n"						#this is to go to the next line after sample 1
space:		.ascii " "						#Prints space
pName:		.asciiz "Enter your name: "				#prompts name
pInt:		.asciiz "Please enter an integer between 1-100: "	#prompts integer
pAns:		.asciiz "your answers are: "				#shows answer
	
.text
main:	
	#RUN 1
	la $a0, pName
	li $v0, 4
	syscall
	la $a0, name
	li $a1, 30
	li $v0, 8
	syscall
	
	la $a0, pInt
	li $v0, 4
	syscall
	li $v0, 5
	syscall
	sw $v0, a
	
	la $a0, pInt
	li $v0, 4
	syscall
	li $v0, 5
	syscall
	sw $v0, b

	la $a0, pInt
	li $v0, 4
	syscall
	li $v0, 5
	syscall
	sw $v0, c
	
	lw $t0, a		
	lw $t1, b
	lw $t2, c
	add $t3, $t0, $t0	#Calulates the num1
	sub $t3, $t3, $t1
	addi $t3, $t3, 9
	sw $t3, num1
	li $t3, 0
	
	sub $t3, $t2, $t1	#Calulates the num2
	add $t3, $t3, $t0
	sub $t3, $t3, 5
	sw $t3, num2
	li $t3, 0
	
	sub $t3, $t0, 3		#Calulates the num3
	addi $t4, $t1, 4
	addi $t5, $t2, 7
	add $t3, $t3, $t4
	sub $t3, $t3, $t5
	sw $t3, num3
	
	la $a0, name
	li $v0, 4
	syscall
	
	la $a0, pAns
	li $v0, 4
	syscall
	lw $a0, num1
	li $v0, 1
	syscall
	li $a0, 0x20 		#Space between numbers
	li $v0, 11
	syscall
	lw $a0, num2
	li $v0, 1
	syscall
	li $a0, 0x20 		#Space between numbers
	li $v0, 11
	syscall
	lw $a0, num3
	li $v0, 1
	syscall
	li $v0, 4       	#Next Line For Sample 2
	la $a0, newline
	syscall

	#RUN 2
	la $a0, pName
	li $v0, 4
	syscall
	la $a0, name
	li $a1, 30
	li $v0, 8
	syscall
	
	la $a0, pInt
	li $v0, 4
	syscall
	li $v0, 5
	syscall
	sw $v0, a
	
	la $a0, pInt
	li $v0, 4
	syscall
	li $v0, 5
	syscall
	sw $v0, b

	la $a0, pInt
	li $v0, 4
	syscall
	li $v0, 5
	syscall
	sw $v0, c
	
	lw $t0, a		
	lw $t1, b
	lw $t2, c
	add $t3, $t0, $t0	#Calulates the num1
	sub $t3, $t3, $t1
	addi $t3, $t3, 9
	sw $t3, num1
	li $t3, 0
	
	sub $t3, $t2, $t1	#Calulates the num2
	add $t3, $t3, $t0
	sub $t3, $t3, 5
	sw $t3, num2
	li $t3, 0
	
	sub $t3, $t0, 3		#Calulates the num3
	addi $t4, $t1, 4
	addi $t5, $t2, 7
	add $t3, $t3, $t4
	sub $t3, $t3, $t5
	sw $t3, num3
	
	la $a0, name
	li $v0, 4
	syscall
	
	la $a0, pAns
	li $v0, 4
	syscall
	lw $a0, num1
	li $v0, 1
	syscall
	li $a0, 0x20 		#Space between numbers
	li $v0, 11
	syscall
	lw $a0, num2
	li $v0, 1
	syscall
	li $a0, 0x20 		#Space between numbers
	li $v0, 11
	syscall
	lw $a0, num3
	li $v0, 1
	syscall
	 


	#Test Run 1
	#Enter your name: John
	#Please enter an integer between 1-100: 5
	#Please enter an integer between 1-100: 10
	#Please enter an integer between 1-100: 20
	#John
	#your answers are: 9 10 -11
	
	 #Test Run 2
	 #Enter your name: Jane
	 #Please enter an integer between 1-100: 11
	 #Please enter an integer between 1-100: 22
	 #Please enter an integer between 1-100: 33
	 #Jane
	 #your answers are: 9 17 -6

