MAIN:               mov               @r3,               LENGTH
m5
LOOP:                jmp             L1
m
prn                      -5
bne              W
sub              @r1,                         @r4
bne L3
L1:                         inc K
jmp W
                END:                       stop
STR:                    .string "abcdef"
LENGTH: .data   6,-9              ,15
K: .data 22
cmp -5, 5
