#Name: Sanjeev Penupala
#Section: 504
#Date: 3/8/2019

.data
promptN:	.asciiz "What is your Name? "
promptH:	.asciiz "Please enter your height in inches: "
promptW:	.asciiz "Please enter your weight in pounds (round to a whole number) "
endMsg:		.asciiz ", your bmi is: "
bmiUnder: 	.float 18.5
bmiNormal: 	.float 25
bmiOver: 	.float 30
name: 		.space 20
msgUW: 		.asciiz "\nThis is considered underweight."
msgW: 		.asciiz "\nThis is normal weight."
msgOW: 		.asciiz "\nThis is considered overweight."
msgO: 		.asciiz "\nThis is considered obese."

.text
	main:
	#Prompt User To Enter Their First Name
	li $v0,4
	la $a0, promptN
	syscall

	#Store The Inputted Name Into "name"
	li $v0, 8
	la $a1, 20
	la $a0, name
	syscall

	#Remove The newLine Char At The End Of Stream
	li $t0, 0
	while: 
		lb $t1, name($t0)
		addi $t0, $t0, 1
		bnez $t1, while
		beq $t2, $t0, gone
		subiu $t0,$t0,2
		sb $zero, name($t0)

	gone:
	#Prompt User To Enter Height
	la $a0, promptH
	li $v0, 4
	syscall

	#Store The Height
	li $v0, 5
	syscall
	
	move $s0, $v0

	#Prompt User To Enter Weight
	la $a0, promptW
	li $v0, 4
	syscall

	#Store The Weight
	li $v0, 5
	syscall
	
	move $s1, $v0

	#Multiply Height By Itself
	mul $s0, $s0, $s0

	#Multiply Weight By 703
	mul $s1, $s1, 703

	#Move Values Into Floating Point Registers For Further Calculations
	mtc1 $s0, $f20
	cvt.s.w $f20, $f20
	mtc1 $s1, $f21
	cvt.s.w $f21, $f21

	#Calculate BMI
	div.s $f12, $f21, $f20

	#Display BMI
	la $a0, name
	li $v0, 4
	syscall
	
	la $a0, endMsg
	li $v0, 4
	syscall

	li $v0, 2
	syscall

	#If (BMI < 18.5)
	l.s $f0, bmiUnder
	c.lt.d $f12, $f0
	bc1t under

	#If (BMI < 25)
	l.s $f0, bmiNormal
	c.lt.d $f12, $f0
	bc1t normal

	#If (BMI < 30)
	l.s $f0, bmiOver
	c.lt.d $f12, $f0
	bc1t over

	#Else (Assume BMI Is Obese)
	j obese
	
under: 
	li $v0, 4
	la $a0, msgUW
	syscall
	
	j exit

normal:	
	li $v0, 4
	la $a0, msgW
	syscall
	
	j exit

over:
	li $v0, 4
	la $a0, msgOW
	syscall
	
	j exit

obese:
	li $v0, 4
	la $a0, msgO
	syscall

exit: 
	#Escape Function With No Code
