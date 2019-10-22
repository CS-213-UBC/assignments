.pos 0x100
                ld   $i, r0            # r0 = &i
                ld   $0, r1            # r1 = 0 = i'
                ld $c, r5              # r5 = &c
                ld (r5), r7            # r7 = c'
                ld $n, r2              # r2 = &n
                ld (r2), r2            # r2 = n
                not r2                 # r2 = !n
                inc r2                 # r2 = -n

loop:           mov r1, r3            # r3 = r1 = i'
                add r2, r3            # r3 += r2
                beq r3, end_loop      # goto end_loop if((i'- n == 0)
                ld $a, r4             # r4 = $a
                ld (r4,r1, 4), r4     # r4 = a[i']
                ld $b, r3             # r3 = &b
                ld (r3, r1, 4), r3    # r3 = b[i']
                not r3                  # r3 = !b[i']
                inc r3                  # r3 = -b[i']
                add r4, r3        # r3 += a[i']
                bgt r3, then      # goto then if ((a[i] - b[i']) > 0) 
                inc r1            # r1 = i'
                br loop           # branch to loop
                 

then:
                inc r1            # i'++
                inc r7            # c'++
                br loop           # branch to loop

end_loop:
                st r7, (r5)     # c = c'
                st r1, (r0)     # i = i'
                halt


.pos 0x1000
i:               .long -1         # i
.pos 0x1008
a:               .long 10         # a[0]
                 .long 20         # a[1]
                 .long 30         # a[2]
                 .long 40         # a[3]
                 .long 50         # a[4]

.pos 0x2000
b:               .long 11         # b[0]
                 .long 20         # b[1]
                 .long 28         # b[2]
                 .long 44         # b[3]
                 .long 48         # b[4]

.pos 0x2020
c:               .long 0          # c
.pos 0x2024
n:               .long 5          # c
