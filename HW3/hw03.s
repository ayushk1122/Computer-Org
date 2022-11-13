################################################################################
# Homework 3: Matrix-matrix multiplication
# CSCI-2500 Fall 2021
# Ayush Krishnappa 
################################################################################
## Data segment
################################################################################
.data
  newline:  .asciiz "\n"
  tab:      .asciiz "\t"

################################################################################
## Text segment
################################################################################
.text
  .align 2          # instructions must be on word boundaries
  .globl main       # main is a global label
  .globl mm_read    # read in NxM matrix
  .globl mm_alloc   # allocate space for NxM matrix
  .globl mm_print   # print out formatted NxM matrix
  .globl mm_mult    # multiple two non-square matrices

################################################################################
# Main segment
################################################################################
main:
  # save return address on the stack
  sub $sp, $sp, 4   
  sw  $ra, 0($sp)

  # Read in a matrix 1 of some NxM size
  # Return in $v0 - N
  # Return in $v1 - M
  # Return in 0($sp) - base address of matrix
  sub $sp, $sp, 4   # make space on stack for return value
  jal mm_read

  # save matrix 1 values
  move  $s0, $v0
  move  $s1, $v1
  lw    $s2, 0($sp)
  add   $sp, $sp, 4   # restore stack

  # # Setup arguments and print out matrix 1
  move  $a0, $s0
  move  $a1, $s1
  move  $a2, $s2
  jal mm_print

  # Read in matrix 2 of some NxM size
  sub $sp, $sp, 4   # make space on stack for return value
  jal mm_read

  # # save matrix 2 values
  move  $s3, $v0
  move  $s4, $v1
  lw    $s5, 0($sp)
  add   $sp, $sp, 4   # restore stack

  # # Setup arguments and print out matrix 2
  move  $a0, $s3
  move  $a1, $s4
  move  $a2, $s5
  jal mm_print

  # Setup arguments and call mm_mult
  # Note: 5 arguments, so we need to use stack
  # Note: num cols matrix 1 == num rows matrix 2
  move  $a0, $s0    # num rows matrix1
  move  $a1, $s1    # num cols matrix1
  move  $a2, $s2    # address of matrix 1
  move  $a3, $s4    # num cols matrix2
  sub   $sp, $sp, 4   
  sw    $s5, 0($sp) # address of matrix 2
  jal mm_mult
  add   $sp, $sp, 4

  # # print the result
  move $a0, $s0 
  move $a1, $s4
  move $a2, $v0
  jal mm_print

  # restore $ra, free stack space, and return
  lw  $ra, 0($sp)
  add $sp, $sp, 4
  jr  $ra

################################################################################
# mm_read: Read in a NxM matrix from standard input
# Note: You will be calling mm_alloc from this function, so you'll need to save 
#       $ra on the stack. Also, you'll be returning 3 values back to main, one
#       value (the matrix base address), will need to be saved on the stack.
#       Matrix format is:
#         First line:  N = num rows
#         Second line: M = num cols
#         Subsequent lines contain one value each, filling each row in turn
#         (same format as hw01, except N,M on separate lines)  
# Input:
#   None
# Return:
#   v0 = N
#   v1 = M
#   0($sp) = base address of matrix
################################################################################
mm_read:
  # save return address and any saved registers on the stack, if necessary
  sub $sp, $sp, 28
  sw $ra, 0($sp)
  sw  $s6, 4($sp)  
  sw $s0, 8($sp) 
  sw $s1, 12($sp)
  sw $s2, 16($sp)
  sw $s4, 20($sp)
  sw $s5, 24($sp)

  # get N
  li $v0, 5
  syscall
  move $s0, $v0

  # get M
  li $v0, 5
  syscall
  move $s1, $v0

  # Setup up arguments and call mm_alloc - v0 is returned as base address
  add $s4, $zero, $s0
  add $s5, $zero, $s1
  move  $a0, $s0
  move  $a1, $s1
  jal mm_alloc
  move $s6, $v0 # allocated memory address stored in v0

  # # do nested loops to read in values
  add $t0, $zero, $zero
  mul $t1, $s4, $s5
  add $t4, $zero, $zero

  move $s2, $s6 
  outerloop: 
  beq $t0, $t1, END1 # loops through N*M number of items
    li $v0, 5
    syscall
    move $t3, $v0

    sw $t3, 0($s6) # adds each array value to array 

    addi $s6, $s6, 4 # moves array index
    addi $t0, $t0, 1 # increments loop var
    
    j outerloop

  END1:
    
  # setup up return values
  move $v0, $s4 
  move $v1, $s5
  move $t5, $s2

  # Note: third return value goes on the stack *after* restoration below

  # restore stack, ra, and any saved registers, if necessary
  lw $ra, 0($sp)
  lw $s6, 4($sp)
  lw $s0, 8($sp)
  lw $s1, 12($sp)
  lw $s2, 16($sp)
  lw $s4, 20($sp)
  lw $s5, 24($sp)
  addi $sp, $sp, 28

  sw $t5, 0($sp) # third return added to stack after restores 

  # return to main
  jr  $ra

################################################################################
# mm_alloc: Allocate an NxM matrix
# Note: You can just allocate a single array of length N*M.
# Input:
#   a0 = N
#   a1 = M
# Return:
#   v0 = base address of allocated space
################################################################################
mm_alloc:
  # save return address and any saved registers on the stack, if necessary

  # Allocate sufficient space
  mul $t0, $a0, $a1
  mul $t0, $t0, 4
  add $a0, $t0, $zero
  li $v0, 9 # set return value
  syscall

  # return to main
  jr  $ra
################################################################################
# mm_print: Output the formatted contents of NxM matrix
# Note: Within a row, separate each value by tab
#       End each row with a newline
# Input:
#   a0 = N
#   a1 = M
#   a2 = base pointer
# Return:
#   None
################################################################################
mm_print:
  # save return address and any saved registers on the stack, if necessary
  sub $sp, $sp, 4   
  sw  $ra, 0($sp)

  # do nested loops to print out values
  add $t0, $zero, $zero # temp vars to store original inputs 
  add $t2, $a0, $zero
  add $t3, $a1, $zero
  outerloop_print: # loops through number of rows in matrix
    beq $t0, $t2, END_print
      add $t1, $zero, $zero
      j innerloop_print
  
  innerloop_print: # loops through number of cols in matrix 
    beq $t1, $t3, END2_print
      lw $a0, 0($a2) # prints value in matrix position 
      li $v0, 1
      syscall 

      li $v0, 4 # prints tab
      la $a0, tab
      syscall 

      addi $t1, $t1, 1
      addi $a2, $a2, 4

      j innerloop_print

    END_print:
      lw  $ra, 0($sp) # ends loop and exits procedure back to main 
      add $sp, $sp, 4
      jr  $ra

    END2_print: 
      li $v0, 4 # prints new line after each row 
      la $a0, newline
      syscall 

      addi $t0, $t0, 1

      j outerloop_print

  # restore stack, ra, and any saved registers, if necessary
  lw  $ra, 0($sp)
  add $sp, $sp, 4
################################################################################
# mm_mult: Perform the actual NxM matrix multiplication
# Input:
#   a0 = N for matrix 1
#   a1 = M for matrix 1 <-> N for matrix 2
#   a2 = base address for matrix 1
#   a3 = M for matrix 2
#   0($sp) = base address for matrix 2
# Return:
#   v0 = base address for result matrix
################################################################################
mm_mult:
  # save return address and any saved registers on the stack, if necessary
  # R*N + (C+1)
  # result_matrix((R*N) + (C+1)) = matrix1(N, C) * matrix2(C, M)
  sub $sp, $sp, 40
  sw  $ra, 0($sp)
  sw $s0, 4($sp)
  sw $s1, 8($sp)
  sw $s2, 12($sp) # result matrix array 
  sw $s3, 16($sp)
  sw $s4, 20($sp)
  sw $s5, 24($sp)
  sw $s6, 28($sp)
  sw $s7, 32($sp)

  # set up and call mm_alloc for result matrix
  add $s5, $a1, $zero # M for matrix 1 and N for matrix 2

  add $s0, $zero, $a0 # N for result matrix = N for matrix 1
  add $s1, $zero, $a3 # M for result matrix = M for matrix 2
  add $a0, $s0, $zero
  add $a1, $s1, $zero
  jal mm_alloc # allocates memory for result matrix 
  move $s2, $v0

  lw $s4, 40($sp) # loads base address of matrix 2 into saved address to avoid moving stack pointer
  move $t7, $s2

  li $t2, 0 # temp variables 
  li $s6, 0
  
  Loop1:
  beq $t2, $s0, loop1EXIT # loops through result matrix rows, N
     # i = row j = col -> 4*(i*size(row) + j)
     li $t3, 0
     Loop2:
     beq $t3, $s1, loop2EXIT # loops throuhg result matrix cols, R
     li $t4, 0

       Loop3:
       beq $t4, $s5, loop3EXIT # loops through matrix 1 cols
      
       add $t6, $zero, $zero # resets all temp variables 
       add $t5, $zero, $zero
       add $t1, $zero, $zero
       add $t8, $zero, $zero
       add $t9, $zero, $zero

       # result matrix 
       mul $t5, $t2, $s1 # R * num cols in result matrix 
       add $t6, $t5, $t3 # (i*(num cols in result matrix)) + (j)
       mul $t6, $t6, 4

       add $s2, $s2, $t6 # moves result matrix pointer to correct index 
       add $t1, $t6, $zero # stores offset moved for result matrix 
      
      # # matrix 1
       add $t6, $zero, $zero
       add $t5, $zero, $zero
       mul $t5, $t2, $s5 # R * num cols in matrix 1
       add $t6, $t5, $t4 # (i*(num cols in matrix 1) + (k)
       mul $t6, $t6, 4

       add $a2, $a2, $t6 # moves matrix 1 pointer to correct index 
       lw $t8, 0($a2) # stores value in index
       sub $a2, $a2, $t6 # resets matrix 1 base pointer 

      # # matrix 2
       add $t6, $zero, $zero
       add $t5, $zero, $zero
       mul $t5, $t4, $s1 # R * num cols in matrix 2
       add $t6, $t5, $t3 # + j
       mul $t6, $t6, 4

       add $s4, $s4, $t6 # moves matrix 2 pointer to correct index
       lw $t9, 0($s4)
       sub $s4, $s4, $t6

      # # procedure to calculate sums for result matrix, (results in error so commented out )
      # calulates product and adds to sum 
      mul $s6, $t8, $t9 # product 
      add $s7, $s7, $s6 # concurrent sum

      sw $s7, 0($s2) # moves sum into result matrix 
       
      sub $s2, $s2, $t1
      addi $t4, $t4, 1

      j Loop3

      loop3EXIT:
      addi $t3, $t3, 1
      add $s7, $zero, $zero # resets sum variable for next iteration 

      j Loop2
    
     loop2EXIT:
     addi $t2, $t2, 1
     
     j Loop1

   loop1EXIT:

  # set return value
  move $v0, $t7

  # restore stack, ra, and any saved registers, if necessary
  lw $ra, 0($sp)
  lw $s0, 4($sp)
  lw $s1, 8($sp)
  lw $s2, 12($sp)
  lw $s3, 16($sp)
  lw $s4, 20($sp)
  lw $s5, 24($sp)
  lw $s6, 28($sp)
  lw $s7, 32($sp)
  add $sp, $sp, 40

  # return to main
  jr  $ra
