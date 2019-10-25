.pos 0x100
start:
    ld $sb, r5          # r5 = &sb
    inca    r5          # r5 = address of element after stack
    gpc $6, r6          # r6 = pc + 6  
    j main              # call main function
    halt

f:
    deca r5             # allocate f stack frame
    ld $0, r0           # r0 = i = 0
    ld 4(r5), r1        # r1 = argument value of f (a)
    ld $0x80000000, r2  # r2 = 0x800000000
f_loop: 
    beq r1, f_end       # if a == 0, goto f_end
    mov r1, r3          # r3 = r1
    and r2, r3          # r3 = 0x80000000 & a
    beq r3, f_if1       # if 0x80000000 & a == 0, goto fif1
    inc r0              # i++
f_if1:
    shl $1, r1          # a << 1
    br f_loop           # goto f_loop
f_end:
    inca r5             # deallocate f function frame
    j(r6)               # return i

main:
    deca r5             # allocate main function frame
    deca r5             # allocate main function frame (size 8 total)
    st r6, 4(r5)        # store return address to stack
    ld $8, r4           # r4 = 8 = i

main_loop:
    beq r4, main_end    # if i = 0, goto main_end
    dec r4              # i --
    ld $x, r0           # r0 = address of x
    ld (r0,r4,4), r0    # r0 = x[i]
    deca r5             # allocate f function frame 
    st r0, (r5)         # store x[i] to stack of f (argument)
    gpc $6, r6          # store return address in r6
    j f                 # jump to function f
    inca r5             # deallocate frame for f
    ld $y, r1           # f1 = &y
    st r0, (r1,r4,4)    # y[i] = return value of f(x[i])
    br main_loop        # go to main_loop
main_end:
    ld 4(r5), r6        # r6 = return address
    inca r5             # deallocate frame for function main (size 8)
    inca r5             # deallocate frame for function main (size 8)
    j (r6)              # return

.pos 0x2000
x:
    .long 1
    .long 2
    .long 3
    .long -1
    .long -2
    .long 0
    .long 184
    .long 340057058

y:
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0

.pos 0x8000
# These are here so you can see (some of) the stack contents.
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

