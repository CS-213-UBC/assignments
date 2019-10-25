.pos 0x100
        ld $4, r0       #r0 = number of grades
        ld $s, r2       #r2 = &s
        ld (r2), r2     #r2 = s
        ld $n , r3      #r3 = &n
        ld (r3), r3     #r3 = n
        ld $24, r4      #r4 = 24

loop_student_list:
        beq r3, end_student_list     #goto end_student if(r3 == 0)
        ld $0, r1       #r1 = 0 = sum
        ld $4, r0       #r0 = 4 = x = number of grades left to check

loop_get_average:   
        beq r0, end_loop_get_average    #goto end_loop_get_average if(x==0)
        mov r0, r7              #r7 = x
        ld (r2, r7, 4), r7      #r7 = grade[x]
        add r7, r1              #r1 += grade[x] = sum
        dec r0                  #x--
        br loop_get_average     #goto loop_get_average

end_loop_get_average:
        shr $2, r1              #r1 = sum/4
        st r1, 20(r2)           #average = sum/4
        add r4, r2              #r2 += 24, point to next student
        dec r3                  #n--
        br loop_student_list    #goto loop_student_list

end_student_list:
        ld $n, r0       #r0 = &n
        ld (r0), r0     #r0 = n 
        ld $s, r2       #r2 = &s
        ld (r2), r2     #r2 = s
sort:
        dec r0               #r0 = r0-1 = i
        beq r0, end_sort     #goto end if (r0 == 0)
        ld $1, r1            #r1 = 1 = j
        ld $s, r2            #r2 = &s
        ld (r2), r2          #r2 = s
loop:
        mov r0, r3      #r3 = i
        not r3          #r3 = !i
        inc r3          #r3 = -i
        add r1, r3      #r3 = j - i
        bgt r3, sort    #goto sort if((j-i) > 0)
        mov r1, r4      #r4 = r1 = j
        dec r4          #r4 = j - 1   
        ld 20(r2), r4   #r4 = s[j-1] -> average
        ld $24, r7
        add r7, r2      #r3 = s[j]
        ld 20(r2), r5   #r5 = s[j] -> average
        not r5          #r5 = !s[j]
        inc r5          #r5 = -s[j]
        add r4, r5      #r5 = s[j-1] - s[j]
        bgt r5, swap    #goto swap if ((s[j-1] - s[j]) > 0)
        inc r1          #r1 = j++
        br loop         #branch to loop

swap:
        ld $6, r5               #r5 = 6 = z = elements to swap
start_swap:
        beq r5, end_swap        #goto end_swap if(r5==0)
        dec r5                  #z--
        mov r2, r3              #r3 = r2
        ld $-24, r4             #r4 = -24
        add r4, r3              #r3 = j-24
        ld (r2,r5, 4), r6       #r6 = s[j + z]
        ld (r3,r5, 4), r7       #r7 = s[j - 24 + z]
        st r6, (r3, r5, 4)      #s[j - 24 + z] = s[j + z] 
        st r7, (r2, r5, 4)      #s[j + z] = s[j - 24 + z]
        br start_swap           #goto start_swap

end_swap:
        inc r1          #j++
        br loop         #goto loop
end_sort:
        ld $n, r0       #r0 = &n
        ld (r0), r0     #r0 = n
        ld $s, r2       #r2 = &s
        ld (r2), r2     #r2 = s
        shr $1, r0      #r0 = n/2
        ld $24, r1      #r1 = 24
get_mid:
        beq r0, end_program #goto end_program if(r0==0)
        add r1, r2      #r2 +=r1
        dec r0          #r0--
        br get_mid      #branch get_mid

end_program:
        ld $m, r4       #r4 = &m
        ld (r2), r5     #r5 = s[24+(n/2)]
        st r5, (r4)     #m  = s[24+(n/2)]
        halt
        


.pos 0x1000

n:      .long 5 # just one student
m:      .long 0 # put the answer here
s:      .long base # address of the array
base:   .long 1234 # student ID
        .long 80 # grade 0
        .long 60 # grade 1
        .long 78 # grade 2
        .long 90 # grade 3
        .long 0 # computed average
base2:  
        .long 5678 # student ID
        .long 10 # grade 0
        .long 20 # grade 1
        .long 30 # grade 2
        .long 40 # grade 3
        .long 0 # computed average
base3:  
        .long 2183 # student ID
        .long 99 # grade 0
        .long 99 # grade 1
        .long 99 # grade 2
        .long 99 # grade 3
        .long 0 # computed average
base4:  
        .long 3712 # student ID
        .long 90 # grade 0
        .long 30 # grade 1
        .long 78 # grade 2
        .long 50 # grade 3
        .long 0 # computed average
base5:
        .long 1321 # student ID
        .long 90 # grade 0
        .long 80 # grade 1
        .long 80 # grade 2
        .long 90 # grade 3
        .long 0 # computed average
