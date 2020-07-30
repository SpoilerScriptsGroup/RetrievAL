#include <windows.h>
#include "TMainForm.h"
#include "TDrawGrid.h"
#undef MainForm

extern const DWORD F0055E74C;

/*
void __stdcall TMainForm_DrawTreeCell_DrawFocusRect(int ARow, int nowSelectSubjectIndex, long TMainForm *MainForm, long RectH, TCanvas *BSCanvas)
{
	if (ARow == nowSelectSubjectIndex + 1)
	{
		TDrawGrid *DGrid;
		RECT      rcItem;

		DGrid = MainForm->DGrid;
		rcItem.left = 2;
		rcItem.top = 2;
		rcItem.right = DGrid->DefaultColWidth - 2;
		rcItem.bottom = DGrid->DefaultRowHeight - 2;
		::DrawFocusRect(BSCanvas->Handle, &rcItem);
	}
}
*/
__declspec(naked) void __cdecl TMainForm_DrawTreeCell_DrawFocusRect()
{
	static const DWORD X004460A0 = 0x004460A0;

	__asm
	{
		#define MainForm ebx
		#define ARow     (ebp + 16)
		#define BSCanvas esi

		mov     eax, dword ptr [MainForm + TMainForm.nowSelectSubjectIndex]
		mov     ecx, dword ptr [ARow]
		inc     eax
		cmp     eax, ecx
		jne     L3

		sub     esp, size RECT

		#define rcItem esp

		mov     ecx, dword ptr [MainForm + TMainForm.DGrid]
		mov     eax, dword ptr [ecx + TDrawGrid.DefaultColWidth]
		mov     ecx, dword ptr [ecx + TDrawGrid.DefaultRowHeight]
		sub     eax, 2
		sub     ecx, 2
		mov     dword ptr [rcItem + RECT.left  ], 2
		mov     dword ptr [rcItem + RECT.top   ], 2
		mov     dword ptr [rcItem + RECT.right ], eax
		mov     dword ptr [rcItem + RECT.bottom], ecx
		mov     eax, BSCanvas
		call    dword ptr [F0055E74C]
		push    rcItem
		push    eax
		call    DrawFocusRect
		add     esp, size RECT
	L3:
		push    00CC0020H
		jmp     dword ptr [X004460A0]

		#undef rcItem

		#undef MainForm
		#undef ARow
		#undef BSCanvas
	}
}
