mcro m
jmp L1
endmcro
mcro m1
jmp L1
endmcro
mcro m2
jmp L1
endmcro
mcro m3
jmp L1
endmcro
mcro m4
jmp L1
endmcro
mcro m5
jmp L2
jmp L3
jmp L4
jmp L5
jmp L6
jmp L7
jmp L8
endmcro
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
