.pos 0x100

    # tmp = 0
    ld $tmp, r0         # r0 = address of tmp
    ld $0, r2           # r2 = 0
    st r2, 0(r0)        # tmp = 0

    # tos = 0
    ld $tos, r1         # r1 = address of tos
    st r2, 0(r1)        # tos = 0

    # s[tos] = a[0]
    ld $a, r2           # r2 = address of a
    ld $s, r3           # r3 = address of s
    ld 0(r2), r4        # r4 = a[0]
    st r4, 0(r3)        # s[0] = a[0]

    # tos++
    ld 0(r1), r4        # r4 = tos
    inc r4              # r4++
    st r4, 0(r1)        # tos++

    # s[tos] = a[1]
    inca r2             # r2 = address of a[1]
    ld 0(r2), r4        # r4 = a[1]
    ld 0(r1), r5        # r5 = tos
    shl $2, r5          # r5 = r5*4
    add r3, r5          # r5 = &s[tos]
    st r4, 0(r5)        # s[tos] = a[1]


    # tos++
    ld 0(r1), r4        # r4 = tos
    inc r4              # r4++
    st r4, 0(r1)        # tos++


    # s[tos] = a[2]
    inca r2             # r2 = address of a[2]
    ld 0(r2), r4        # r4 = a[2]
    ld 0(r1), r5        # r5 = tos
    shl $2, r5          # r5 = r5*4
    add r3, r5          # r5 = &s[tos]
    st r4, 0(r5)        # s[tos] = a[2]


    # tos++
    ld 0(r1), r4        # r4 = tos
    inc r4              # r4++
    st r4, 0(r1)        # tos++


    # tos--
    ld 0(r1), r4        # r4 = tos
    dec r4              # r4--
    st r4, 0(r1)        # tos--

    # tmp = s[tos]
    ld 0(r1), r4        # r4 = tos
    shl $2, r4          # r4 = tos *4
    add r3, r4          # r4 = address of s[tos]
    ld 0(r4), r5        # r5 = s[tos]
    st r5, 0(r0)        # tmp = s[tos]

    # tos--
    ld 0(r1), r4        # r4 = tos
    dec r4              # r4--
    st r4, 0(r1)        # tos--


    # tmp = tmp + s[tos]
    ld 0(r0), r6        # r4 = tmp
    ld 0(r1), r4        # r4 = tos
    shl $2, r4          # r4 = tos *4
    add r3, r4          # r4 = address of s[tos]
    ld 0(r4), r5        # r5 = s[tos]
    add r5, r6          # r6 = tmp + s[tos]
    st r6, 0(r0)           # tmp = tmp + s[tos]

    # tos--
    ld 0(r1), r4        # r4 = tos
    dec r4              # r4--
    st r4, 0(r1)        # tos--


    # tmp = tmp + s[tos]
    ld 0(r0), r6        # r4 = tmp
    ld 0(r1), r4        # r4 = tos
    shl $2, r4          # r4 = tos *4
    add r3, r4          # r4 = address of s[tos]
    ld 0(r4), r5        # r5 = s[tos]
    add r5, r6          # r6 = tmp + s[tos]
    st r6, 0(r0)           # tmp = tmp + s[tos]
    halt

.pos 0x2000
# Data area

tos:  .long 0             # tos
tmp:  .long 0             # tmp
a:    .long 4             # a[0]
      .long 5             # a[1]
      .long 6             # a[2]

s:    .long 0             # s[0]
      .long 0             # s[1]
      .long 0             # s[2]
      .long 0             # s[3]
      .long 0             # s[4]
