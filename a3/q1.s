.pos 0x100

    # i = a[3]
    ld $a, r0           # r0 = address of a
    ld $i, r1           # r1 = address of i
    ld $3, r2           # r1 = 3
    ld (r0,r2,4), r3    # r3 = address of a[3]
    ld 0(r3), r3        # r3 = a[3]
    st r3, 0(r1)        # i = r1 = a[3]

    # i = a[i]
    ld 0(r1), r2        # r2 = i
    ld (r0, r2, 4), r3   # r3 = a[i]
    st r3, 0(r1)        # i = a[i]

    # p = &j
    ld $p, r2           # r2 = address of p
    ld $j, r3           # r3  = address of j
    st r3, 0(r2)        # p = &j

    # *p = 4   
    ld $4, r3           # r3 = 4
    ld 0(r2), r4        # r4 = p
    st r3, 0(r4)        # *p = 4

    # p  = &a[a[2]];
    ld $2, r3           # r3 = 2
    ld (r0, r3, 4), r4  # r4 = a[2]
    shl $2, r4          # r4 = r4*4
    add r0, r4          # r4 = &a[a[2]]
    st r4, 0(r2)        # p = &a[a[2]]

    # *p = *p + a[4]; 
    ld 0(r2), r3        # r3 = p
    ld 0(r3), r4        # r4 = *p
    ld $4, r5           # r5 = 4
    ld (r0, r5, 4), r6  # r6 = a[4] 
    add r4, r6          # r6 = *p + a[4]
    st r6, (r3)

    halt

.pos 0x200
# Data area

i:  .long 0             # i
j:  .long 0             # j
p:  .long 0             # p
a:  .long 0             # a[0]
    .long 1             # a[1]
    .long 2             # a[2]
    .long 3             # a[3]
    .long 4             # a[4]
    .long 5             # a[5]
    .long 6             # a[6]
    .long 7             # a[7]
    .long 8             # a[8]
    .long 9             # a[9]
