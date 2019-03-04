;file ps.as
LENGTH:      .data  6
        .entry LENGTH
          .extern W

    MAIN:        mov        r3,      LENGTH
LOOP:        jmp   L1(#-1,r6)
prn            #-5
             bne      W(r4,r5)
             sub     r1  ,  r4
             bne    L3
L1:          inc     K
.entry    LOOP
             bne LOOP(K,W)
END:         stop
STR:         .string   "abcdef"

K:           .data      22,23,    232       ,    23
.extern L3



;test1 test2 test3 test4 empty_file bad_file good_file






