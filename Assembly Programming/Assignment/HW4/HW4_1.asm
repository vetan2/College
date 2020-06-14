TITLE HW4 Task 1

INCLUDE irvine32.inc

.data
string BYTE 42 DUP(0)
stringLen BYTE ?
encrypted BYTE 41 DUP(0)
decrypted BYTE 41 DUP(0)
key SBYTE 10 DUP(0)
keyLen BYTE ?
tempStr BYTE 51 DUP(0)
tempStrLen BYTE ?

prompt1 BYTE "Enter a plain text : ", 0
prompt2 BYTE "Enter a key : ", 0
prompt3 BYTE "Original Text : ", 0
prompt4 BYTE "Encrypted Text : ", 0
prompt5 BYTE "Decrypted Text : ", 0
prompt6 BYTE "Bye!", 0

.code
main PROC
	_String:
		mov edx, OFFSET prompt1
		call Writestring

		mov edx, OFFSET string
		mov ecx, 42
		call Readstring
		mov stringLen, al

		.IF stringLen == 0
			jmp EndProgram
		.ELSEIF stringLen > 40
			jmp _String
		.ENDIF
		
	_Key::
		mov edx, OFFSET prompt2
		call Writestring

		mov esi, OFFSET tempStr
		mov edi, OFFSET key
		call InputKey

		mov eax, edi
		sub eax, OFFSET Key
		dec eax
		mov keyLen, al
		
		mov al, keyLen
		.IF al > stringLen
			jmp _Key
		.ENDIF

	_Print:
		call PrintResult
		call Init
		jmp _String
		
	EndProgram:
		mov edx, OFFSET prompt6
		call Writestring
		exit
main ENDP

; --------------------------------------------------------------------
InputKey PROC
; Read the key
; Receives : esi = OFFSET of temp string, edi = OFFSET of key
; Returns : key = entered key
; --------------------------------------------------------------------
mov edx, OFFSET tempStr
mov ecx, 51
call Readstring
mov tempStrLen, al

mov ecx, 10
L1:
	mov bh, [esi]
	.IF bh == ' '
		inc esi
		jmp L1
	.ELSEIF bh == 0
		.IF ecx == 10
			jmp _Key
		.ENDIF
		ret
	.ENDIF

	push ecx
	.IF bh == '-'
		mov cl, 1
		mov bl, 0
	.ELSE
		mov cl, 0
		sub bh, '0'
		mov bl, bh
	.ENDIF

	L2:
		inc esi
		mov bh, [esi]

		.IF bh == ' ' || bh == 0
			.IF cl == 1
				neg bl
			.ENDIF
			mov [edi], bl
			inc edi
			inc esi
			pop ecx
			loop L1
		.ELSEIF bh == 10
			.IF cl == 1
				neg bl
			.ENDIF
			mov [edi], bl
			inc edi
			inc esi
			pop ecx
			loop L1
		.ENDIF

		mov al, bl
		mov ch, 10
		imul ch
		mov bl, al
		sub bh, '0'
		add bl, bh
		jmp L2
ret
InputKey ENDP

; --------------------------------------------------------------------
PrintResult PROC
; Print the result
; Receives : Nothing
; Returns : Nothing
; --------------------------------------------------------------------
call Crlf

mov edx, OFFSET prompt3
call Writestring
mov edx, OFFSET string
call Writestring
call Crlf

call Encrypt
mov edx, OFFSET prompt4
call Writestring
mov edx, OFFSET encrypted
call Writestring
call Crlf

call Decrypt
mov edx, OFFSET prompt5
call Writestring
mov edx, OFFSET decrypted
call Writestring

call Crlf
call Crlf
ret
PrintResult ENDP

; --------------------------------------------------------------------
Encrypt PROC USES esi edi ecx bx
; Encrypt the string with the key
; Receives : Nothing
; Returns : encrypted = encrypted string
; --------------------------------------------------------------------
mov esi, 0
mov edi, OFFSET key
movzx ecx, stringLen
mov bl, 1

L1:
	mov eax, edi
	sub eax, OFFSET key
	.IF al >= keyLen
		mov edi, OFFSET key
	.ENDIF

	mov bh, string[esi]
	mov bl, [edi]
	TEST bl, 10000000b
	jz L2
	jmp L4

	L2:
		push ecx
		movzx ecx, bl
		.IF ecx == 0
			pop ecx
			jmp L
		.ENDIF

		L3:
			ror bh, 1
			loop L3
		pop ecx
		jmp L

	L4:
		neg bl
		push ecx
		movzx ecx, bl
		.IF ecx == 0
			pop ecx
			jmp L
		.ENDIF

		L5:
			rol bh, 1
			loop L5
		pop ecx
	L:
		mov encrypted[esi], bh
		inc esi
		inc edi
		loop L1
ret
Encrypt ENDP

; --------------------------------------------------------------------
Decrypt PROC USES esi edi ecx bx
; Decrypt the encrypted string with the key
; Receives : Nothing
; Returns : decrypted = decrypted string
; --------------------------------------------------------------------
mov esi, 0
mov edi, OFFSET key
movzx ecx, stringLen
mov bl, 1

L1:
	mov eax, edi
	sub eax, OFFSET key
	.IF al >= keyLen
		mov edi, OFFSET key
	.ENDIF

	mov bh, encrypted[esi]
	mov bl, [edi]
	TEST bl, 10000000b
	jz L2
	jmp L4

	L2:
		push ecx
		movzx ecx, bl
		.IF ecx == 0
			pop ecx
			jmp L
		.ENDIF

		L3:
			rol bh, 1
			loop L3
		pop ecx
		jmp L

	L4:
		neg bl
		push ecx
		movzx ecx, bl
		.IF ecx == 0
			pop ecx
			jmp L
		.ENDIF

		L5:
			ror bh, 1
			loop L5
		pop ecx
	L:
		mov decrypted[esi], bh
		inc esi
		inc edi
		loop L1
ret
Decrypt ENDP

Init PROC
	mov ECX, 42
	mov esi, 0
	L1:
		mov string[esi], 0
		mov encrypted[esi], 0
		mov decrypted[esi], 0
		inc esi
		loop L1
	mov stringLen, 0

	mov ECX, 10
	mov esi, 0
	L2:
		mov key[esi], 0
		inc esi
		loop L2
	mov keyLen, 0

	mov ECX, 51
	mov esi, 0
	L3:
		mov tempStr[esi], 0
		inc esi
		loop L3
ret
Init ENDP
END main