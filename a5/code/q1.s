## Code - TODO: Comment and translated to C in q1()
.pos 0x1000

## C statement 1
S1:
ld    $i, r0            #r0 = &i 
ld    (r0), r0          #r0 = i
ld    $a, r1            #r1 = &a
ld    (r1), r1          #r1 = a = &d0 (&x)
ld    (r1), r1          #r1 = &d1 = x
ld    (r1, r0, 4), r2   #r2 = d0[i] = x[i]
ld    $v0, r3           #r3 = &v0 
st    r2, (r3)          #v0 = d0[i] = x[i]

## C statement 2
S2:
ld    $i, r0            #r0 = &i
ld    (r0), r0          #r0 = i
ld    $a, r1            #r1 = &a
ld    (r1), r1          #r1 = a = &d0 = &x
inca  r1                #r1 += 4 = &b
ld    (r1, r0, 4), r2   #r2 = d0[i] = a->b.y[i]
ld    $v1, r3           #r3 = &v1
st    r2, (r3)          #v1 = d0[i] = a->b.y[i]

## C statement 3
S3:
ld    $i, r0            #r0 = &i
ld    (r0), r0          #r0 = i
ld    $a, r1            #r1 = &a
ld    (r1), r1          #r1 = a = &x
ld    20(r1), r1        #r1 = d2 (value at r1 offset by 20) = a->b.a
ld    (r1), r1          #r1 = &d3 = a->b.a->x
ld    (r1, r0, 4), r2   #r2 = d3[i] = a->b.a->x[i]
ld    $v2, r3           #r2 = &v2
st    r2, (r3)          #v2 = d3[i] = a->b.a->x[i]

## C statement 4
S4:
ld    $a, r1            #r1 = &a
ld    (r1), r1          #r2 = &d0 = &x
st    r1, 20(r1)        #d0 = a->b.a = &x = a

## C statement 5
S5:
ld    $i, r0            #r0 = &i
ld    (r0), r0          #r0 = i 
ld    $a, r1            #r1 = &a
ld    (r1), r1          #r1 = a = &x
ld    20(r1), r1        #r1 = a->b.a
inca  r1                #r1 = a->b.a + 4 = a->b.a->b 
ld    (r1, r0, 4), r2   #r2 = a->b.a->b.y[i'] = a->b.y[i] = 20
ld    $v3, r3           #r3 = &v3
st    r2, (r3)          #v3 = a->b.y[i] = 20


halt


## Globals
.pos 0x2000
i:  .long 0
v0: .long 0
v1: .long 0
v2: .long 0
v3: .long 0
a:  .long d0        #struct a


## Heap (these labels represent dynamic values and are thus not available to code)
.pos 0x3000
d0: .long d1        #a->x
    .long 20        #a->b.y[0]
    .long 21        #a->b.y[1]
    .long 22        #a->b.y[2]
    .long 23        #a->b.y[3]
    .long d2        #a->b.a

d2: .long d3        #a->b.a->x
    .long 40        #a->b.a->b.y[0]
    .long 41        #a->b.a->b.y[1]
    .long 42        #a->b.a->b.y[2]
    .long 43        #a->b.a->b.y[3]
    .long 0         #a->b.a->b.a

d1: .long 10        #a->x[0] 
    .long 11        #a->x[1] 
    .long 12        #a->x[2] 
    .long 13        #a->x[3] 

d3: .long 30        #a->b.a->x[0]
    .long 31        #a->b.a->x[1]
    .long 32        #a->b.a->x[2]
    .long 33        #a->b.a->x[3]
