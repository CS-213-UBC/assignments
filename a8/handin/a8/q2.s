.pos 0x0
                 ld   $0x1028, r5         # r5 = end of stack  
                 ld   $0xfffffff4, r0     # r0 = -12
                 add  r0, r5              # allocate frame
                 ld   $0x200, r0          # r0 = &a
                 ld   0x0(r0), r0         # r0 = a
                 st   r0, 0x0(r5)         # put a on stack
                 ld   $0x204, r0          # r0 = &b
                 ld   0x0(r0), r0         # r0 = b
                 st   r0, 0x4(r5)         # put b on stack
                 ld   $0x208, r0          # r0 = &c
                 ld   0x0(r0), r0         # r0 = c
                 st   r0, 0x8(r5)         # put c on stack
                 gpc  $6, r6              # r6 = return address
                 j    0x300               # jump to foo
                 ld   $0x20c, r1          # r1 = &d
                 st   r0, 0x0(r1)         # d = value of r0
                 halt                     
.pos 0x200
a:               .long 0x0000000a         
b:               .long 0x0000000a         
c:               .long 0x00000005         
d:               .long 0x00000000         
.pos 0x300
foo:
                 ld   0x0(r5), r0         # r0 = a
                 ld   0x4(r5), r1         # r1 = b
                 ld   0x8(r5), r2         # r2 = c
                 ld   $0xfffffff6, r3     # r3 = -10
                 add  r3, r0              # r0 = a - 10
                 mov  r0, r3              # r3 = a - 10
                 not  r3                  # r3 = !(a-10)
                 inc  r3                  # r3 = 10 - a
                 bgt  r3, L6              # goto L6 if((10-a) > 0)
                 mov  r0, r3              # r3 = r0
                 ld   $0xfffffff8, r4     # r4 = -8
                 add  r4, r3              # r3 -= 8
                 bgt  r3, L6              # goto L6 if((a - 18) > 0)
                 ld   $0x400, r3          # r3 = &jt
                 j    *(r3, r0, 4)        # jump to jt[a-18]
.pos 0x330
I10:             add  r1, r2              # r2 = b + c --- jt[0]
                 br   L7                  # branch L7  --- CONT
I12:             not  r2                  # r2 = !c    --- jt[2] 
                 inc  r2                  # r2 = -c
                 add  r1, r2              # r2 = b - c
                 br   L7                  # branch L7  --- CONT
I14:             not  r2                  # r2 = !c    --- jt[4]
                 inc  r2                  # r2 = -c
                 add  r1, r2              # r2 = b - c
                 bgt  r2, L0              # goto L0 if((b-c) > 0) 
                 ld   $0x0, r2            # r2 = 0
                 br   L1                  # goto L1
L0:              ld   $0x1, r2            # r2 = 1
L1:              br   L7                  # branch L7 --- CONT
I16:             not  r1                  # r1 = !b   --- jt[6]
                 inc  r1                  # r1 = -b
                 add  r2, r1              # r2 = c - b
                 bgt  r1, L2              # goto L2 if((c-b) > 0)
                 ld   $0x0, r2            # r2 = 0
                 br   L3                  # goto L3
L2:              ld   $0x1, r2            # r2 = 1
L3:              br   L7                  # goto L7 --- CONT
I18:             not  r2                  # r2 = !c --- jt[8]           
                 inc  r2                  # r2 = -c
                 add  r1, r2              # r2 = b - c 
                 beq  r2, L4              # goto L4 if((b-c) == 0)
                 ld   $0x0, r2            # r2 = 0
                 br   L5                  # goto L5
L4:              ld   $0x1, r2            # r2 = 1
L5:              br   L7                  # branch L7 --- CONT
L6:              ld   $0x0, r2            # r2 = 0
                 br   L7                  # goto L7  --- CONT
L7:              mov  r2, r0              # r0 = r2
                 j    0x0(r6)             # pc = r6
.pos 0x400
jt:              .long I10         
                 .long L6         
                 .long I12         
                 .long L6         
                 .long I14         
                 .long L6         
                 .long I16         
                 .long L6         
                 .long I18         
.pos 0x1000
stack:           .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
