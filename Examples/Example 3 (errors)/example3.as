.entry LOOP
.entry LENGTH
.extern W
MAIN: mov r5[r3],W
 add r2,STR
LOOP: jump W
 prn #-5g
 sub r1, r4
 inc K

 mov r7[r6],r3
 bne L3
END: stop
STR: .string "abcdef
1LENGTH: .data 6,-9,1ng5
K: .data 22
