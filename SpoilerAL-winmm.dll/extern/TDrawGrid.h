#pragma once

#include <windows.h>

typedef struct
{
	LPVOID    *VTable;
	BYTE      padding1[68];
	const int Width;
	const int Height;
	BYTE      padding2[440];
	LPVOID    Canvas;
	BYTE      padding3[36];
	const int DefaultColWidth;
	const int DefaultRowHeight;
	BYTE      padding4[20];
	const int RowCount;
	BYTE      padding5[12];
	const int TopRow;
	BYTE      padding6[120];
} TDrawGrid;

EXTERN_C int __fastcall TDrawGrid_GetClientWidth(TDrawGrid *DrawGrid);
EXTERN_C int __fastcall TDrawGrid_GetClientHeight(TDrawGrid *DrawGrid);
EXTERN_C void __fastcall TDrawGrid_SetDefaultRowHeight(TDrawGrid *DrawGrid, const int RowHeight);
EXTERN_C void __fastcall TDrawGrid_SetDefaultColWidth(TDrawGrid *DrawGrid, const int ColWidth);
EXTERN_C void __fastcall TDrawGrid_SetRowCount(TDrawGrid *DrawGrid, const int RowCount);
EXTERN_C int __fastcall TDrawGrid_GetRowHeights(TDrawGrid *DrawGrid, const int Index);
EXTERN_C void __fastcall TDrawGrid_SetRowHeights(TDrawGrid *DrawGrid, const int Index, const int RowHeights);
EXTERN_C void __fastcall TDrawGrid_SetTopRow(TDrawGrid *DrawGrid, const int TopRow);
EXTERN_C void __fastcall _TDrawGrid_MouseToCell(TDrawGrid *DrawGrid, int X, int Y, int *ARow, int *ACol);
#define TDrawGrid_MouseToCell(DrawGrid, X, Y, ACol, ARow) _TDrawGrid_MouseToCell(DrawGrid, X, Y, ARow, ACol)
EXTERN_C void __fastcall TDrawGrid_Repaint(TDrawGrid *DrawGrid);
