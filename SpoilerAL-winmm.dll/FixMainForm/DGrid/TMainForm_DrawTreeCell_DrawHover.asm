.486
.model flat, c

extrn _imp__BitBlt@36:dword
extrn _imp__AlphaBlend@44:dword

public TMainForm_DrawTreeCell_DrawHover

.code

align 16

;void __stdcall TMainForm_DrawTreeCell_DrawHover(int ARow, int invertGridRow, Graphics::TBitmap *invertImage, long RectW, long RectH, TCanvas *BSCanvas)
;{
;	if ((ARow == invertGridRow) && (invertImage != NULL))
;	{
;		HDC           invertImageHandle;
;		BLENDFUNCTION blendFunction;
;
;		invertImage->Width = RectW;
;		invertImage->Height = RectH;
;		invertImageHandle = invertImage->Canvas->Handle;
;		::BitBlt(invertImageHandle, 0, 0, RectW, RectH, BSCanvas->Handle, 0, 0, NOTSRCCOPY);
;		blendFunction.BlendOp             = AC_SRC_OVER;
;		blendFunction.BlendFlags          = 0;
;		blendFunction.SourceConstantAlpha = 48;
;		blendFunction.AlphaFormat         = 0;
;		::AlphaBlend(BSCanvas->Handle, 0, 0, RectW, RectH, invertImageHandle, 0, 0, RectW, RectH, blendFunction);
;	}
;}
TMainForm_DrawTreeCell_DrawHover proc near

	NOTSRCCOPY  equ 00330008H
	AC_SRC_OVER equ 0

	ARow          equ <ebp + 10H>
	invertGridRow equ <ebx + 558H>
	invertImage   equ <ebx + 51CH>
	RectW         equ <ebp - 188H>
	RectH         equ <ebp - 18CH>
	BSCanvas      equ <esi>

	mov     eax, dword ptr [ARow]
	mov     ecx, dword ptr [invertGridRow]
	cmp     eax, ecx
	jne     L1
	mov     eax, dword ptr [invertImage]
	test    eax, eax
	jz      L1
	jmp     L2
	align   16
L1:
	mov     eax, 00444C12H
	mov     word ptr [ebp - 174H], 8
	jmp     eax
	align   16
L2:
	mov     ecx, dword ptr [eax]
	mov     edx, dword ptr [RectW]
	call    dword ptr [ecx + 40H]
	mov     eax, dword ptr [invertImage]
	mov     edx, dword ptr [RectH]
	mov     ecx, dword ptr [eax]
	call    dword ptr [ecx + 34H]
	mov     ecx, 00562CF0H
	mov     eax, dword ptr [invertImage]
	call    ecx
	mov     ecx, 0055E74CH
	sub     esp, 88
	call    ecx
	mov     dword ptr [esp +  4], eax
	mov     dword ptr [esp + 64], eax
	mov     ecx, 0055E74CH
	mov     eax, BSCanvas
	call    ecx
	mov     dword ptr [esp + 24], eax
	mov     dword ptr [esp + 44], eax
	mov     edx, dword ptr [_imp__AlphaBlend@44]
	mov     eax, dword ptr [RectW]
	mov     ecx, dword ptr [RectH]
	mov     dword ptr [esp     ], edx
	mov     dword ptr [esp +  8], 0
	mov     dword ptr [esp + 12], 0
	mov     dword ptr [esp + 16], eax
	mov     dword ptr [esp + 20], ecx
	mov     dword ptr [esp + 28], 0
	mov     dword ptr [esp + 32], 0
	mov     dword ptr [esp + 36], NOTSRCCOPY
	mov     dword ptr [esp + 40], offset L1
	mov     dword ptr [esp + 48], 0
	mov     dword ptr [esp + 52], 0
	mov     dword ptr [esp + 56], eax
	mov     dword ptr [esp + 60], ecx
	mov     dword ptr [esp + 68], 0
	mov     dword ptr [esp + 72], 0
	mov     dword ptr [esp + 76], eax
	mov     dword ptr [esp + 80], ecx
	mov     eax, dword ptr [_imp__BitBlt@36]
	mov     dword ptr [esp + 84], AC_SRC_OVER or (0 shl 8) or (48 shl 16) or (0 shl 24)
	jmp     eax

TMainForm_DrawTreeCell_DrawHover endp

end
