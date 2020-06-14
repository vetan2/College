TITLE 3
INCLUDE irvine32.inc
.data
prompt1 BYTE "Enter a Multiplier : ", 0
prompt2 BYTE "Enter a Multiplicand : ", 0
prompt3 BYTE "Product : ", 0
prompt4 BYTE "Bye!", 0
.code
main PROC
_Multiplier:
mov EDX, OFFSET prompt1
call Writestring
call ReadHex
jc _End
mov EBX, EAX
call BitwiseMultiply
mov EDX, OFFSET prompt3
call Writestring
call WriteHex
call Crlf
call Crlf
jmp _Multiplier
_End:
mov EDX, OFFSET prompt4
call Writestring
exit
main ENDP
BitwiseMultiply PROC
_Multiplicand:
mov EDX, OFFSET prompt2
call Writestring
call ReadHex
jc _Multiplicand
mov ECX, 32
mov EDX, 0
_Multiply:
shl EBX, 1
jc _Shift
jmp _Continue
_Shift:
push EAX
push ECX	
_L1:
shl EAX, 1
loop _L1
shr EAX, 1
pop ECX
add EDX, EAX
pop EAX
_Continue:
loop _Multiply
mov EAX, EDX
ret
BitwiseMultiply ENDP
END main