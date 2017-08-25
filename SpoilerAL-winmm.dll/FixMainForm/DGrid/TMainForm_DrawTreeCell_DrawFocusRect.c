#include <windows.h>

extern const DWORD F0055E74C;

__declspec(naked) void __cdecl TMainForm_DrawTreeCell_DrawFocusRect()
{
	/*
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
	*/

	__asm
	{
		#define sizeof_RECT                     16
		#define MainForm                        ebx
		#define ARow                            (ebp + 16)
		#define nowSelectSubjectIndex           (MainForm + 1364)
		#define DGrid                           (MainForm + 932)
		#define offsetof_DGrid_DefaultColWidth  560
		#define offsetof_DGrid_DefaultRowHeight 564
		#define BSCanvas                        esi

		mov     eax, dword ptr [nowSelectSubjectIndex]
		mov     ecx, dword ptr [ARow]
		inc     eax
		cmp     eax, ecx
		jne     L3

		sub     esp, sizeof_RECT

		#define rcItem        esp
		#define rcItem_left   esp
		#define rcItem_top    (esp +  4)
		#define rcItem_right  (esp +  8)
		#define rcItem_bottom (esp + 12)

		mov     ecx, dword ptr [DGrid]
		mov     eax, dword ptr [ecx + offsetof_DGrid_DefaultColWidth]
		mov     ecx, dword ptr [ecx + offsetof_DGrid_DefaultRowHeight]
		sub     eax, 2
		sub     ecx, 2
		mov     dword ptr [rcItem_left  ], 2
		mov     dword ptr [rcItem_top   ], 2
		mov     dword ptr [rcItem_right ], eax
		mov     dword ptr [rcItem_bottom], ecx
		mov     eax, BSCanvas
		call    dword ptr [F0055E74C]
		push    rcItem
		push    eax
		call    DrawFocusRect
		add     esp, sizeof_RECT
	L3:
		mov     eax, 004460A0H
		push    00CC0020H
		jmp     eax

		#undef rcItem
		#undef rcItem_left
		#undef rcItem_top
		#undef rcItem_right
		#undef rcItem_bottom

		#undef sizeof_RECT
		#undef MainForm
		#undef ARow
		#undef nowSelectSubjectIndex
		#undef DGrid
		#undef offsetof_DGrid_DefaultColWidth
		#undef offsetof_DGrid_DefaultRowHeight
		#undef BSCanvas
	}
}
