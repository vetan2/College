TITLE HW3

INCLUDE irvine32.inc

.data
INCLUDE hw3.inc
CODE_A BYTE '1'
CODE_B BYTE '01'
CODE_C BYTE '000'
CODE_D BYTE '0011'
CODE_E BYTE '0010'

.code
main PROC
	mov ebx, CODE01
	call decompress
	call crlf

	mov ebx, CODE02
	call decompress
	call crlf

	mov ebx, CODE03
	call decompress
	call crlf

	mov ebx, CODE04
	call decompress
	call crlf

	mov ebx, CODE05
	call decompress
	ret
main ENDP

; --------------------------------------------------------------------
decompress PROC
; Decompress the data
; Receives : DWORD data in ebx
; Returns : print the result
; --------------------------------------------------------------------

pushad
mov ecx, 32
L1 :
	shl ebx, 1
	jnc L2
	
	mov al, 'A'
	call Writechar
	loop L1
popad
ret

L2 :
	dec ecx
	shl ebx, 1
	jnc L3

	mov al, 'B'
	call Writechar
	Loop L1
popad
ret

L3 :
	dec ecx
	shl ebx, 1
	jc L4

	mov al, 'C'
	call Writechar
	Loop L1
popad
ret

L4 :
	dec ecx
	shl ebx, 1
	jnc L5

	mov al, 'D'
	call Writechar
	Loop L1
popad
ret

L5 :
	mov al, 'E'
	call Writechar
	Loop L1
popad
ret
decompress ENDP
END main