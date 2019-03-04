.entry LOOP
.entry LENGTH
.extern L3

MAIN:   mov r5 ,LENGTH
        add r2,STR
LOOP:   jmp END
        prn #-5
        sub r1, r4
        inc K
        bne L3
        mov r5 ,r3
        bne LOOP
END:    stop
STR:   .string "abcdef"
LENGTH: .data 6,-9,15
K: .data 22

