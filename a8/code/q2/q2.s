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
a:               .long 0x00000000         
b:               .long 0x00000000         
c:               .long 0x00000000         
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
                 add  r1, r2              
                 br   L7                  
                 not  r2                  
                 inc  r2                  
                 add  r1, r2              
                 br   L7                  
                 not  r2                  
                 inc  r2                  
                 add  r1, r2              
                 bgt  r2, L0              
                 ld   $0x0, r2            
                 br   L1                  
L0:              ld   $0x1, r2            
L1:              br   L7                  
                 not  r1                  
                 inc  r1                  
                 add  r2, r1              
                 bgt  r1, L2              
                 ld   $0x0, r2            
                 br   L3                  
L2:              ld   $0x1, r2            
L3:              br   L7                  
                 not  r2                  
                 inc  r2                  
                 add  r1, r2              
                 beq  r2, L4              
                 ld   $0x0, r2            
                 br   L5                  
L4:              ld   $0x1, r2            
L5:              br   L7                  
L6:              ld   $0x0, r2            
                 br   L7                  
L7:              mov  r2, r0              
                 j    0x0(r6)             
.pos 0x400
jt:              .long 0x00000330  # &&       
                 .long 0x00000384  # &&default       
                 .long 0x00000334         
                 .long 0x00000384  # &&default       
                 .long 0x0000033c         
                 .long 0x00000384  # &&default       
                 .long 0x00000354         
                 .long 0x00000384  # &&default       
                 .long 0x0000036c         
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
