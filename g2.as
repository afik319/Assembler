mov -20, label2
cmp label2   , label2
.string "random"
mcro mac1
	sub r5, JERUS
	rts
endmcro
label2: .data 4,6,-19,78
label1: add label2,YUV
sub YUV[r14] ,r2
lea label2 , r6
mac1
.entry JERUS
clr MAIN
not r8
mcro mac2
	sub r5, JERUS
	rts
endmcro
.entry 
inc r15
.entry GGG
dec label2
MAIN: jmp YUV
NY: .string "abagada"
miami: bne label2[r15]
red r9
.entry label2
clr MAIN
jsr	miami
dec GGG
mac1
YUV: .string "danzie"
prn GGG
bne label2
label2: rts
prn label2
mac2
JERUS:  add 67       ,r1
stop
red label2
.extern GGG