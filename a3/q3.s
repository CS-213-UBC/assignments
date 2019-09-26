.pos 0x100

#a = 3;
ld $a, r0               #r0 = &a
ld $3, r1               #r1 = 3
st r1, 0(r0)            #a = 3               

#p = &a;
ld $p, r2               #r2 = &p
st r0, 0(r2)            #p = &a

#*p = *p - 1; #decrement the current value at address a by 1
ld 0(r2), r0            #r0 = &a
ld 0(r0), r3            #r3 = value of a
dec r3                  #r3 = r3 - 1
st r3, 0(r0)            #a = r3


#p = &b[0]; p now points to b[0]
ld $b, r3               #r3 = &b 
st r3, 0(r2)            #p = r3


#p++ p now points to the next integer in array --> b[1]
ld 0(r2), r4            #r4 = value of p
inca r4                  #r4 = r4 + 1
st r4, 0(r2)            #p = r4

#p[a] = b[a];
ld 0(r0), r4            #r4 = a = 2
ld (r3, r4, 4), r7      #r4 = b[a] = b[2] = 2

ld 0(r2), r6            #r6 = address p is pointing to

st r7, (r6, r4, 4)      #p[a] = r4 (p[2] = b[1] = 2)

# *(p+3) = b[0]   
ld 0(r3), r5            #r5 = b[0] = 0
ld $p, r2				#r2 = address of p
ld 0(r2), r6            #r6 = value of p
st r5, 12(r6)           #p[3] = r5

halt

.pos 0x200
a:  .long 0             # a
p:  .long 0             # *p
b:  .long 0             # b[0]
    .long 1             # b[1]
    .long 2             # b[2]
    .long 3             # b[3]
    .long 4             # b[4]
