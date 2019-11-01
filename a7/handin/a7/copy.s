.pos 0x100 
main:       ld   $sb, r5             # r5 = address of last word
            inca r5                  # r5 = address of first word
            gpc  $0x6, r6            # store return address in r6  
            j    copy                # call copy
            halt

copy:       deca r5                 # allocate frame                    
            st r6, (r5)             # store return address on stack
            ld $-8, r0              # r0 = -8
            add r0, r5              # allocate copy frame
            ld $0, r0               # r0 = i' = 0
            ld $src, r1             # r1 = &src
while:      ld (r1, r0, 4), r2      # r2 = src[i']
            beq r2, end_while       # goto end_while if src[i'] == 0
            st r2, (r5, r0, 4)      # dst[i] = src[i]
            inc r0                  # i'++
            br while                # go to while

end_while:
            ld $8, r0               # r0 = 8
            add r0, r5              # dealloate frame
            ld (r5), r6             # r6 = return address
            inca r5                 # reset pointer
            j (r6)                  # goto stored address

.pos 0x1000
# set the value of every register to 1 and then halt
src:        .long 0x1
            .long 0x1
            .long 0x100c
            .long 0xFF000000
            .long 0xFFFFFFFF
            .long 0xFF000100    
            .long 0xFFFFFFFF
            .long 0xFF000200
            .long 0xFFFFFFFF
            .long 0xFF000300
            .long 0xFFFFFFFF
            .long 0xFF000400
            .long 0xFFFFFFFF
            .long 0xFF000500
            .long 0xFFFFFFFF
            .long 0xFF000600
            .long 0xFFFFFFFF
            .long 0xFF000700
            .long 0xFFFFFFFF
            .long 0xF000F000
            
.pos 0x2000
            .long 0x00000000
            .long 0x00000000         
            .long 0x00000000         
            .long 0x00000000         
            .long 0x00000000         
            .long 0x00000000               
            .long 0x00000000         
            .long 0x00000000         
sb:         .long 0x00000000
