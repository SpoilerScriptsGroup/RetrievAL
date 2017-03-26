.486
.model flat, c

public TCanvas_TextOut@16

.code

align 16

;EXTERN_C void __stdcall TCanvas_TextOut(TCanvas *Canvas, int X, int Y, LPCSTR Text)
;{
;	Canvas->TextOut(X, Y, Text);
;}
TCanvas_TextOut@16 proc near

	mov     ecx, 005E0D50H
	push    eax

	Canvas equ <esp +  8>
	X      equ <esp + 12>
	Y      equ <esp + 16>
	Text   equ <esp + 20>

	mov     edx, dword ptr [Text]
	mov     eax, esp
	call    ecx
	mov     ecx, dword ptr [eax]
	push    ecx
	mov     ecx, dword ptr [Y]
	mov     edx, dword ptr [X]
	mov     eax, dword ptr [Canvas]
	push    offset L1
	push    0055E418H
	ret
	align   16
L1:
	mov     ecx, 005E0EA8H
	mov     edx, 2
	mov     eax, esp
	call    ecx
	pop     eax
	ret     16

TCanvas_TextOut@16 endp

end
