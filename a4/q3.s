
.pos 0x1000
code:
ld $v0, r0          #r0 = &v0 
ld $i, r1           #r1 = &i 
ld 0(r1), r1        #r1 = i 
ld $s, r2           #r2 = &s 
ld (r2, r1, 4), r3  #r3 = s.x[i]
st r3, 0(r0)        $v0 = r3

ld $v1, r0          #r0 = &v1
ld $i, r1           #r1 = &i 
ld 0(r1), r1        #r1 = i 
ld $s, r2           #r2 = &s 
ld 2(r2), r3        #r3 = &s_y 
ld (r3, r1, 4), r3  #r3 = s.y[i]
st r3, 0(r0)        $v1 = r3
halt


.pos 0x2000
static:
i:          .long 0         #i = 0
v0:         .long 0         #v0 = 0
v1:         .long 0         #v1 = 0
v2:         .long 0         #v2 = 0
v3:         .long 0         #v3 = 0

s:          .long 0         # s.x[0]
            .long 0         # s.x[1]
            .long s_y       # s.y
            .long s_z       # s.z

heap:
s_y:        .long 0         # s.y[0]
            .long 0         # s.y[1]
            
s_z:        .long 0         # s.z->x[0]
            .long 0         # s.z->x[1]
            .long 0         # s.z->y
            .long s_z_z     # s.z->z

s_z_z:      .long 0         # s.z->z.x[0]
            .long 0         # s.z->z.x[1]
            .long s_z_z_y   # s.z->z->y
            .long s_z_z_z   # s.z->z->z


s_z_z_y:    .long 0         # s.y[0]
            .long 0         # s.y[1]

s_z_z_z: