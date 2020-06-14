TITLE 2
INCLUDE irvine32.inc
.data
CaseTable BYTE 1
DWORD Process_AND
EntrySize = ($ - CaseTable)
BYTE 2
DWORD Process_OR
BYTE 3
DWORD Process_NOT
BYTE 4
DWORD Process_XOR
BYTE 5
DWORD Process_EXIT
NumOfEntries = ($ - CaseTable) / EntrySize
prompt_menu BYTE "1. x AND y", 0dh, 0ah,
"2. x OR y", 0dh, 0ah,
"3. NOT x", 0dh, 0ah,
"4. x XOR y", 0dh, 0ah,
"5. Exit program", 0dh, 0ah, 0dh, 0ah, 0
prompt_selection BYTE "Choose Calculation Mode : ", 0
prompt_enter_x BYTE "Enter x : ", 0
prompt_enter_y BYTE "Enter y : ", 0
prompt_resultOf BYTE "Result of ", 0
prompt_AND BYTE "x AND y : ", 0
prompt_OR BYTE "x OR y : ", 0
prompt_NOT BYTE "NOT x : ", 0
prompt_XOR BYTE "x XOR y : ", 0
prompt_EXIT BYTE "Bye!", 0dh, 0ah, 0
prompt_askChange BYTE "Do you want to change the mode(Y/N)? : ", 0
x DWORD ?
y DWORD ?
sixteen DWORD 16
two BYTE 2
tempInt DWORD ?
tempStr BYTE 10 DUP (0)
.code
main PROC
_Menu:
mov EDX, OFFSET prompt_menu
call Writestring
_Selection:
mov EDX, OFFSET prompt_selection
call Writestring
call Readint
.IF AX <=0 || AX > 5
jmp _Selection
.ENDIF
mov EBX, OFFSET CaseTable
mov ECX, NumOfEntries
call Crlf	
_Match:
cmp AL, [EBX]
jne _NotFound
_Found:
push EBX
call NEAR PTR [EBX + 1]
pop EBX
call Crlf
call Crlf
_ModeChange:
mov EDX, OFFSET prompt_askChange
call Writestring
mov EDX, OFFSET tempStr
mov ECX, 2
call Readstring
cmp AX, 1
jne _ModeChange
mov AL, [EDX]
.IF AL == 'N'
call Crlf
jmp _Found
.ELSEIF AL == 'Y'	
call Crlf
jmp _Menu
.ELSE
jmp _ModeChange
.ENDIF
_NotFound:
add EBX, EntrySize
loop _Match
jmp _Selection
exit
main ENDP
Process_AND PROC
_Input_x:
mov EDX, OFFSET prompt_enter_x
call Writestring
call GetVal
cmp BL, 1
je _Input_x
mov x, EAX
_Input_y:
mov EDX, OFFSET prompt_enter_y
call Writestring
call GetVal
cmp BL, 1
je _Input_y
mov y, EAX
_Result:
mov EBX, x
and EBX, y
mov EDX, OFFSET prompt_resultOf
call Writestring
mov EDX, OFFSET prompt_AND
call Writestring
call PrintHexaDec
ret
Process_AND ENDP
Process_OR PROC
_Input_x:
mov EDX, OFFSET prompt_enter_x
call Writestring
call GetVal
cmp BL, 1
je _Input_x
mov x, EAX
_Input_y:
mov EDX, OFFSET prompt_enter_y
call Writestring
call GetVal
cmp BL, 1
je _Input_y
mov y, EAX
_Result:
mov EBX, x
or EBX, y
mov EDX, OFFSET prompt_resultOf
call Writestring
mov EDX, OFFSET prompt_OR
call Writestring
call PrintHexaDec
ret
Process_OR ENDP
Process_NOT PROC
_Input_x:
mov EDX, OFFSET prompt_enter_x
call Writestring
call GetVal
cmp BL, 1
je _Input_x
mov x, EAX
_Result:
mov EBX, x
not EBX
mov EDX, OFFSET prompt_resultOf
call Writestring
mov EDX, OFFSET prompt_NOT
call Writestring
call PrintHexaDec
ret
Process_NOT ENDP
Process_XOR PROC
_Input_x:
mov EDX, OFFSET prompt_enter_x
call Writestring
call GetVal
cmp BL, 1
je _Input_x
mov x, EAX
_Input_y:
mov EDX, OFFSET prompt_enter_y
call Writestring
call GetVal
cmp BL, 1
je _Input_y
mov y, EAX
_Result:
mov EBX, x
xor EBX, y
mov EDX, OFFSET prompt_resultOf
call Writestring
mov EDX, OFFSET prompt_XOR
call Writestring
call PrintHexaDec
ret
Process_XOR ENDP
Process_EXIT PROC
mov EDX, OFFSET prompt_EXIT
call Writestring
exit
Process_EXIT ENDP
GetVal PROC
_Input:
mov EDX, OFFSET tempStr
mov ECX, 10
call Readstring
movzx ECX, AX
mov BL, AL
.IF ECX == 0 || ECX == 9
mov BL, 1
ret
.ENDIF
mov EAX, 0
mov EBX, 16
_Convert:
push EAX
mov EAX, 0
mov AL, [EDX]
.IF AL >= '0' && AL <= '9'
sub AL, '0'
.ELSEIF AL >= 'A' && AL <= 'F'
sub AL, 'A'
add AL, 10
.ELSE
pop EAX
mov BL, 1
ret
.ENDIF
mov tempInt, EAX
pop EAX
.IF BL != 1
push EDX
mul sixteen
pop EDX
.ENDIF	
add EAX, tempInt
inc EDX
loop _Convert
mov BL, 0
ret
GetVal ENDP
PrintHexaDec PROC
mov ECX, 8
_L1:
mov AL, 0
push ECX
mov ECX, 4
_L2:
ROL EBX, 1
jc _AddOne
jmp _Continue
_AddOne:
add AL, 1
jmp _Continue
_Continue:
.IF ECX != 1
mul two
.ENDIF
loop _L2
pop ECX
.IF AL >= 0 && AL <= 9
add AL, '0'
.ELSE
add AL, ('A' - 10)
.ENDIF
call Writechar
loop _L1
ret		
PrintHexaDec ENDP
END main