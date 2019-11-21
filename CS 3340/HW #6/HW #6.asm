# Name: Sanjeev Penupala
# Assignemnt: Homework #6
# Class/Section Number: CS 3340.502
# Date: 3/31/19

.data
		.align 4
buffer: 	.space 80
array: 		.space 80
len: 		.word  20
sum: 		.word
sumF: 		.double
mean: 		.float
median: 	.word
medianF: 	.float
stdDev: 	.float

fileTxt: 	.asciiz "input.txt"
errorMsgTxt:	.asciiz "Error with opening fileTxt"
preArrayTxt: 	.asciiz "The array before :: "
postArrayTxt: 	.asciiz "The array after :: "
meanTxt: 	.asciiz "The mean is :: "
medianTxt:	.asciiz "The median is :: "
stdDevTxt:	.asciiz "The standard deviation is :: "
newLine:	.asciiz "\n"
space: 		.asciiz " "
	
.text
	main:
	#Part 1
	la $a0, fileTxt 	# Load Input File
	la $a1, buffer		# Load Buffer address
	jal readFile 		# Jump To Functions of Part 1
	blt $t2, $zero, errorMsgTxt
	#-------------------------------------------------------------------------------------------------------------
	#Part 2
	la $a0, array 		# Load Array
	lw $a1, len 		# Hard-Code Length of The Array
	la $a2, buffer 		# Load Buffer
	jal loadArray 		# Jump To Functions of Part 2
	
	# Print preArray
	addi $v0, $zero, 4  	
   	la $a0, preArrayTxt    
    	syscall
	#-------------------------------------------------------------------------------------------------------------
	#Part 3
	la $t0, array 		# Load Array
	lw $t2, len 		# Hard-Code Length of The Array
	li $t3, 0 		# Index Of The For Loop
	jal printNums 		# Jump To Functions of Part 3
	#-------------------------------------------------------------------------------------------------------------
	#Part 4
	la $a0, array     	# Load Array
   	lw $a1, len		# Hard-Code Length of The Array
   	jal selectionSort  	# Calls Selection Sort Method
   	
	addi $v0, $zero, 4  	# Syscall Addition For Printing The String
   	la $a0, newLine    	# Load newLine
    	syscall
	
	#Print postArray
	la $a0, postArrayTxt
	li $v0, 4
	syscall
	#-------------------------------------------------------------------------------------------------------------
	#Part 3 Again (After Printing postArray...)
	la $t0, array		# Load Array
	lw $t2, len		# Hard-Code Length of The Array
	li $t3, 0		# Index Of The For Loop
	jal printNums 		# Jump To Functions of Part 3
	
	addi $v0, $zero, 4  	# Syscall Addition For Printing The String
   	la $a0, newLine    	# Load newLine
    	syscall
	
	# Print meanTxt
	addi $v0, $zero, 4  	
   	la $a0, meanTxt
    	syscall
	#-------------------------------------------------------------------------------------------------------------
	#Part 5
	la $a0, array 		# Load Array
	lw $a1, len 		# Load Length of The Array
	li $t1, 0 		# Index Of The For Loop
	li $t2, 0 		# Load Current Sum
	jal sumTotal		# Jump To "sumTotal" method
	
	s.s $f0, mean		# Store Into Mean
	l.s $f12, mean 		# Load Mean
	li $v0, 2
	syscall 
	
	addi $v0, $zero, 4  	# Syscall Addition For Printing The String
   	la $a0, newLine    	# Load newLine
    	syscall
	
	# Print medianTxt
	addi $v0, $zero, 4  	# Syscall Addition For Printing The String
   	la $a0, medianTxt
    	syscall
	#-------------------------------------------------------------------------------------------------------------
	#Part 6
	la $a0, array		# Load Array
	lw $a1, len		# Load Length of The Array
	jal gLoop		# Jump to "gLoop" method
	
	addi $v0, $zero, 4  	# Syscall Addition For Printing The String
   	la $a0, newLine    	# Load newLine
    	syscall
	
	# Print stdDevTxt
	addi $v0, $zero, 4  	
   	la $a0, stdDevTxt    
    	syscall
	#-------------------------------------------------------------------------------------------------------------
	#Part 7
	la $a0, array 		# Load Array
	lw $a1, len 		# Load Length of The Array
	li $t1, 0 		# Index Of The For Loop
	li $t2, 0 		# Load Current Sum
	jal sumTotal		# Jump To "sumTotal" method
	
	s.s $f0, mean		# Store Into Mean
	la $a0, array		# Load Array
	lw $a1, len		# Load Length of The Array
	li $t0, 0
	
	# Method For Clearing All Registers
	li $t0, 0
	mtc1 $t0, $f0
	mtc1 $t0, $f1
	mtc1 $t0, $f2
	mtc1 $t0, $f3
	mtc1 $t0, $f5
	mtc1 $t0, $f7
	mtc1 $t0, $f9
	mtc1 $t0, $f12
	
	l.s $f0, sumF
	l.s $f1, mean
	jal SDSumLoop
	s.s $f7, stdDev
	l.s $f12, stdDev
	li $v0, 2
	
	syscall
	j End
#--------------------------------------------------------------------------------------------------------------------------------------------------
# All Of The Sections Below Are The Back-End Code For Each Individual Part
#
#
#

#Functions For Part 1

errorMsg:
	li $v0, 4
	la $a0, errorMsgTxt
	syscall
	j End

readFile:
	# Open "fileTxt"
	li $v0, 13
	la $a0, fileTxt
	li $a1, 0          
	li $a2, 0
	syscall            
	move $s6, $v0
	
	# Read "fileTxt"
	li   $v0, 14       	
	move $a0, $s6      
	la   $a1, buffer   	# Load Buffer
	li   $a2, 80       	# Load Hard-Coded Buffer Length
	syscall            
	
	# Close "fileTxt"
	li   $v0, 16       
	move $a0, $s6     
	syscall            
	
	jr $ra
#--------------------------------------------------------------------------------------------------------------------------------------------------
#Functions For Part 2
# Run A "For Loop" To Store The Integers In An Array

loadArray:
	lb $t1, ($a2) 		# Load 1st Byte
	subi $t1, $t1, 48	
	addi $a2, $a2, 1	
	lb $t2, ($a2)		# Load 2nd Byte
	
	beq $t2, 13, addNums   
	subi $t2, $t2, 48	# Gets Decimal Answer
	mul $t1, $t1, 10	
	add $t1, $t1, $t2
	addi $a2, $a2, 1
	
addNums:
	sw $t1, ($a0) 		# Store Integers In An Array
	addi $a0, $a0, 4 	# Loading Next Address
	addi $a2, $a2, 2 	# Loading Next Byte
	subi $a1, $a1, 1
	beqz $a1, exit
	j loadArray
	
exit:
	jr $ra
#--------------------------------------------------------------------------------------------------------------------------------------------------	
#Functions For Part 3

printNums:
	# Print Integers
	lw $t1, ($t0) 
	li $v0, 1 		# Load Call For Printing Integers
	move $a0, $t1 		# Load Argument (Syscall)
	syscall
	
	addi $v0, $zero, 4  	# Syscall Addition For Printing The String
   	la $a0, space    	# Load Space
    	syscall
	
	addi $t0, $t0, 4	# Next Number
	addi $t3, $t3, 1
	bne $t2, $t3, printNums
	jr $ra
#--------------------------------------------------------------------------------------------------------------------------------------------------	
#Functions For Part 4

selectionSort:
        move $a2, $a1       	# $a2 = n; // (Length)
	sub $a1, $a1, 1      	# $a1 = n - 1; // (Length - 1)
   	li $t0, 0      		# i = 0;
   	
loop1:
   	beq $t0, $a1, return   	# if (i == (n - i)), return
   	move $s0, $t0
   	move $t1, $t0
   	
loop2:      
   	add $t1, $t1, 1       	# j = i + 1
   	bne $t1, $a2, compare	
   	j nextNum        	# if(j == n - 1), i++;

nextNum:		
   	li $t2, 4     		# Start Array With First Index (0) At Index J
   	mul $t3, $t2, $t0      
   	add $t3, $t3, $a0  
   	
   	lw $t5, ($t3)      	# $t5 = array[i];
   	mul $t4, $t2, $s0  
   	add $t4, $t4, $a0              
   	
   	lw $t6, ($t4)       	# $t6 = array[0];
   	sw $t5, ($t4)
   	sw $t6, ($t3)
   	add $t0, $t0, 1
   	
   	j loop1   		
   		 
compare:
	li $t2, 4      
   	mul $t3, $t2, $t1
   	add $t3, $t3, $a0      
   	mul $t4, $t2, $s0  
   	add $t4, $t4, $a0      
   	lw $t5, ($t3)       	      
   	lw $t6, ($t4)       	  
   	slt $t8, $t5, $t6     	# if ($t5 < $t6), $t8 = 1
   	bne $t8, 1, loop2    	# if ($t8 == 1), loop2
   	move $s0, $t1        
   	j loop2           	# go back to loop2
   	
return:
   	jr $ra           
#--------------------------------------------------------------------------------------------------------------------------------------------------   	
#Functions For Part 5

sumTotal:	 	
	# Summation Of The Array
	lw $t5, ($a0)
	add $t1, $t1, 1		# Index i
	add $t2, $t2, $t5 	# $t2 = total;
	add $a0, $a0, 4		
	bne $t1, $a1, sumTotal 

avgArray:
	sw $t2, -88($fp)
	l.s $f5, -88($fp)
	cvt.s.w $f5, $f5	# Conversion From Int To Floating Point
	
	sw $a1, -88($fp)
	l.s $f3, -88($fp)
	cvt.s.w $f3, $f3	# Conversion From Int To Ploating Point
	
	div.s $f0, $f5, $f3
	
	jr $ra			# return "meanTxt"
#--------------------------------------------------------------------------------------------------------------------------------------------------
#Functions For Part 6

gLoop:			
	# Is Array Even Or Odd?
	li $t1, 2
	div $a1, $t1
	mflo $t2
	andi $t3, $a1, 0x01	# Force Binary Digits to be equal to 0 
	beq $t3, $zero, medLoop2
	
medLoop1:
	lw $t4, ($a0)
	subi $t2, $t2, 1
	addi $a0, $a0, 4
	bnez $t2, medLoop1
		
oddCheck:		
	# If "Array Size = Odd", then "Average of 2 Middle Numbers = Median"
	sw $t4, median						
	la $a0, median		
	li $v0, 1
	syscall
	
	jr $ra	
	
medLoop2:
	lw $t4, ($a0)
	add $a2, $a0, 4
	lw $t5, ($a2)
	sub $t2, $t2, 1
	add $a0, $a0, 4
	bnez $t2, medLoop2	
	
evenCheck:		
	# If "Array Size = Even", then "Average of 2 Middle Numbers = Median"
	mtc1 $t4, $f1		
	cvt.s.w $f1, $f1	
	mtc1 $t5, $f3
	cvt.s.w $f3, $f3
	mtc1, $t1, $f5
	cvt.s.w $f5, $f5
	add.s $f7, $f3, $f1
	div.s $f9, $f7, $f5
	s.s $f9, medianF
	l.s $f12, medianF
	li $v0, 2
	syscall
	
	jr $ra
#--------------------------------------------------------------------------------------------------------------------------------------------------
#Functions For Part 7

SDSumLoop:
	# Numerator Summation, According To StdDev Formula			 
	lw $t1, ($a0)		
	mtc1 $t1, $f2
	cvt.s.w $f2, $f2
	
	sub.s $f3, $f2, $f1
	mul.s $f4, $f3, $f3
	add.s $f0, $f0, $f4
	
	addi $a0, $a0, 4
	addi $t0, $t0, 1
	bne $t0, $a1, SDSumLoop
	
stdDevCalc:	
	# Calculate stdDev
	subi $a1, $a1, 1
	mtc1 $a1, $f5
	cvt.s.w $f5, $f5
	div.s $f6, $f0, $f5
	sqrt.s $f7, $f6

	jr $ra
#--------------------------------------------------------------------------------------------------------------------------------------------------	
#End OF Program
End:	
    	li $v0, 10
    	syscall
