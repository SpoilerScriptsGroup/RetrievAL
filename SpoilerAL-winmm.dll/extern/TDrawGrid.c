#include "TDrawGrid.h"

__declspec(naked) int __fastcall TDrawGrid_GetClientWidth(TDrawGrid *DrawGrid)
{
	__asm
	{
		mov     edx, 005806A0H
		mov     eax, ecx
		jmp     edx
	}
}

__declspec(naked) int __fastcall TDrawGrid_GetClientHeight(TDrawGrid *DrawGrid)
{
	__asm
	{
		mov     edx, 005806E4H
		mov     eax, ecx
		jmp     edx
	}
}

__declspec(naked) void __fastcall TDrawGrid_SetDefaultRowHeight(TDrawGrid *DrawGrid, const int RowHeight)
{
	__asm
	{
		mov     eax, ecx
		mov     ecx, 00559068H
		jmp     ecx
	}
}

__declspec(naked) void __fastcall TDrawGrid_SetDefaultColWidth(TDrawGrid *DrawGrid, const int ColWidth)
{
	__asm
	{
		mov     eax, ecx
		mov     ecx, 0055902CH
		jmp     ecx
	}
}

__declspec(naked) void __fastcall TDrawGrid_SetRowCount(TDrawGrid *DrawGrid, const int RowCount)
{
	__asm
	{
		mov     eax, ecx
		mov     ecx, 005592B4H
		jmp     ecx
	}
}

__declspec(naked) int __fastcall TDrawGrid_GetRowHeights(TDrawGrid *DrawGrid, const int Index)
{
	__asm
	{
		mov     eax, ecx
		mov     ecx, 00558E78H
		jmp     ecx
	}
}

__declspec(naked) void __fastcall TDrawGrid_SetRowHeights(TDrawGrid *DrawGrid, const int Index, const int RowHeights)
{
	__asm
	{
		mov     eax, dword ptr [esp]
		push    ecx
		mov     ecx, dword ptr [esp + 8]
		mov     dword ptr [esp + 4], 005592F0H
		mov     dword ptr [esp + 8], eax
		pop     eax
		ret
	}
}

__declspec(naked) void __fastcall TDrawGrid_SetTopRow(TDrawGrid *DrawGrid, const int TopRow)
{
	__asm
	{
		mov     eax, ecx
		mov     ecx, 005593B8H
		jmp     ecx
	}
}

__declspec(naked) void __fastcall _TDrawGrid_MouseToCell(TDrawGrid *DrawGrid, int X, int Y, int *ARow, int *ACol)
{
	__asm
	{
		mov     eax, dword ptr [esp]
		push    ecx
		mov     ecx, dword ptr [esp + 8]
		mov     dword ptr [esp + 4], 00559DB8H
		mov     dword ptr [esp + 8], eax
		pop     eax
		ret
	}
}

__declspec(naked) void __fastcall TDrawGrid_Repaint(TDrawGrid *DrawGrid)
{
	__asm
	{
		mov     eax, ecx
		mov     ecx, dword ptr [ecx]
		jmp     dword ptr [ecx + 80H]
	}
}
