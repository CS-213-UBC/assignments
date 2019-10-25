.pos 0x0
                ld   $sb, r5           # r5 = &sb (stack bottom)
                inca r5                # r5 = address of element after stack
                gpc  $6, r6            # r6 = pc + 6     
                j    0x300             # call main function
                halt                   
.pos 0x100
arrayPointer:    .long 0x00001000       # array pointer points to array

.pos 0x200
foo:
                ld   (r5), r0          # r0 = a
                ld   4(r5), r1          # r1 = b
                ld   $0x100, r2         # r2 = address of array pointer
                ld   (r2), r2           # r2 = value of array pointer
                ld   (r2, r1, 4), r3    #  r3 = * (array pointer + b)
                add  r3, r0            # r0 = * (array pointer + b) + a
                st   r0, (r2, r1, 4)    # * (array pointer + b) = * (array pointer + b) + a
                j    (r6)              # jump back to return address (ie main function)
.pos 0x300
main:
                ld   $-12, r0          # r0 = -12 = -(size of main function frame)
                add  r0, r5            # allocate main function frame       
                st   r6, 8(r5)         # store return address to stack
                
                ld   $1, r0            # r0 = 1
                st   r0, (r5)          # store 1 at the top of the stack (x = 1)
                ld   $2, r0            # r0 = 2
                st   r0, 4(r5)         # store 2 at the second spot (y = 2)
                ld   $-8, r0           # r0 = -8 = -(size of frame for foo)
                add  r0, r5            # allocate foo frame (size = 8)
                ld   $3, r0            # r0 = 3
                st   r0, (r5)          # store 3 into stack (argument1 for foo)
                ld   $4, r0            # r0 = 4
                st   r0, 4(r5)         # store 4 into stack (argument2 for foo)
                gpc  $6, r6            # store return address in r6        
                j    0x200             # jump to 0x200 (foo)

                ld   $8, r0            # r0 = 8
                add  r0, r5            # deallocate frame for foo
                ld   (r5), r1          # r1 = botton of stack
                ld   4(r5), r2         # load value of r2 into second bottom of the stack
                ld   $-8, r0           # r0 = -8
                add  r0, r5            # deallocating call from stack    
                st   r1, (r5)          # store a arg to stack
                st   r2, 4(r5)         # store b arg to stack
                gpc  $6, r6            # store return address to r6   
                j    0x200             # jump to next method (0x200)

                ld   $8, r0             # r0 = 8
                add  r0, r5             # deallocating frame for foo
                ld   8(r5), r6          # r6 = return address from stack
                ld   $12, r0            # r0 = 12 = size of main frame
                add  r0, r5             # deallocate main frame
                j    (r6)               # return
.pos 0x1000
array:           .long 0
                 .long 0
                 .long 0
                 .long 0
                 .long 0
                 .long 0
                 .long 0
                 .long 0
                 .long 0
                 .long 0
.pos 0x8000
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
sb: .long 0
