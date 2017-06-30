.486
.model flat, c

extrn F005E0D50:dword
extrn F0055E418:dword
extrn F005E0EA8:dword

public TCanvas_TextOut@16

.code

align 16

;EXTERN_C void __stdcall TCanvas_TextOut(TCanvas *Canvas, int X, int Y, LPCSTR Text)
;{
;	Canvas->TextOut(X, Y, Text);
;}
TCanvas_TextOut@16 proc near

	Canvas equ <esp +  4>
	X      equ <esp +  8>
	Y      equ <esp + 12>
	Text   equ <esp + 16>

	mov     edx, dword ptr [Text]
	push    eax
	mov     eax, esp
	call    dword ptr [F005E0D50]
	mov     ecx, dword ptr [eax]
	push    ecx
	mov     ecx, dword ptr [Y + 4]
	mov     edx, dword ptr [X + 4]
	mov     eax, dword ptr [Canvas + 4]
	call    dword ptr [F0055E418]
	mov     edx, 2
	mov     eax, esp
	call    dword ptr [F005E0EA8]
	pop     eax
	ret     16

TCanvas_TextOut@16 endp

end
