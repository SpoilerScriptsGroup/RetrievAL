#include <windows.h>
#if defined(_WIN32_WINNT) && _WIN32_WINNT >= _WIN32_WINNT_NT4
#pragma comment(lib, "msimg32.lib")
#endif

extern const DWORD F0055E74C;
extern const DWORD F00562CF0;

/*
void __stdcall TMainForm_DrawTreeCell_DrawHover(int ARow, int invertGridRow, Graphics::TBitmap *invertImage, long RectW, long RectH, TCanvas *BSCanvas)
{
	if ((ARow == invertGridRow) && (invertImage != NULL))
	{
		HDC           invertImageHandle;
		BLENDFUNCTION blendFunction;

		invertImage->Width = RectW;
		invertImage->Height = RectH;
		invertImageHandle = invertImage->Canvas->Handle;
		::BitBlt(invertImageHandle, 0, 0, RectW, RectH, BSCanvas->Handle, 0, 0, NOTSRCCOPY);
		blendFunction.BlendOp             = AC_SRC_OVER;
		blendFunction.BlendFlags          = 0;
		blendFunction.SourceConstantAlpha = 48;
		blendFunction.AlphaFormat         = 0;
		::AlphaBlend(BSCanvas->Handle, 0, 0, RectW, RectH, invertImageHandle, 0, 0, RectW, RectH, blendFunction);
	}
}
*/
__declspec(naked) void __cdecl TMainForm_DrawTreeCell_DrawHover()
{
	static const DWORD X00444C12 = 0x00444C12;

	__asm
	{
		#define _NOTSRCCOPY   0x00330008
		#define ARow          (ebp + 10H)
		#define invertGridRow (ebx + 558H)
		#define invertImage   (ebx + 51CH)
		#define RectW         (ebp - 188H)
		#define RectH         (ebp - 18CH)
		#define BSCanvas      esi

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
		mov     word ptr [ebp - 174H], 8
		jmp     dword ptr [X00444C12]

		align   16
	L2:
		mov     ecx, dword ptr [eax]
		mov     edx, dword ptr [RectW]
		call    dword ptr [ecx + 40H]
		mov     eax, dword ptr [invertImage]
		mov     edx, dword ptr [RectH]
		mov     ecx, dword ptr [eax]
		call    dword ptr [ecx + 34H]
		mov     eax, dword ptr [invertImage]
		call    dword ptr [F00562CF0]
		sub     esp, 88
		call    dword ptr [F0055E74C]
		mov     dword ptr [esp +  4], eax
		mov     dword ptr [esp + 64], eax
		mov     eax, BSCanvas
		call    dword ptr [F0055E74C]
		mov     dword ptr [esp + 24], eax
		mov     dword ptr [esp + 44], eax
		mov     edx, AlphaBlend
		mov     eax, dword ptr [RectW]
		mov     ecx, dword ptr [RectH]
		mov     dword ptr [esp     ], edx
		mov     dword ptr [esp +  8], 0
		mov     dword ptr [esp + 12], 0
		mov     dword ptr [esp + 16], eax
		mov     dword ptr [esp + 20], ecx
		mov     dword ptr [esp + 28], 0
		mov     dword ptr [esp + 32], 0
		mov     dword ptr [esp + 36], _NOTSRCCOPY
		mov     dword ptr [esp + 40], offset L1
		mov     dword ptr [esp + 48], 0
		mov     dword ptr [esp + 52], 0
		mov     dword ptr [esp + 56], eax
		mov     dword ptr [esp + 60], ecx
		mov     dword ptr [esp + 68], 0
		mov     dword ptr [esp + 72], 0
		mov     dword ptr [esp + 76], eax
		mov     dword ptr [esp + 80], ecx
		mov     dword ptr [esp + 84], AC_SRC_OVER or (0 shl 8) or (48 shl 16) or (0 shl 24)
		jmp     BitBlt

		#undef _NOTSRCCOPY
		#undef ARow
		#undef invertGridRow
		#undef invertImage
		#undef RectW
		#undef RectH
		#undef BSCanvas
	}
}
