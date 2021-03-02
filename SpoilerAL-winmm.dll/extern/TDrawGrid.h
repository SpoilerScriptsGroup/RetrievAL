#pragma once

#include "TWinControl.h"
#include "TCanvas.h"

#define DEFINE_TCUSTOMCONTROL                   \
    DEFINE_TWINCONTROL;                         \
    TCanvas       *Canvas;                      \
    BYTE          padding_TCustomControl1[4]

#define DEFINE_TCUSTOMGRID                      \
    DEFINE_TCUSTOMCONTROL;                      \
    BYTE          padding_TCustomGrid1[32];     \
    const int     DefaultColWidth;              \
    const int     DefaultRowHeight;             \
    BYTE          padding_TCustomGrid2[20];     \
    const int     RowCount;                     \
    BYTE          padding_TCustomGrid3[12];     \
    const int     TopRow;                       \
    BYTE          padding_TCustomGrid4[56]

#define DEFINE_TCUSTOMDRAWGRID                  \
    DEFINE_TCUSTOMGRID;                         \
    BYTE          padding_TCustomDrawGrid1[64]

#define DEFINE_TDRAWGRID                        \
    DEFINE_TCUSTOMDRAWGRID

#pragma pack(push, 1)
typedef struct _TDrawGrid
{
	DEFINE_TDRAWGRID;
} TDrawGrid;
#pragma pack(pop)

EXTERN_C int __fastcall TDrawGrid_GetClientWidth(TDrawGrid *DrawGrid);
EXTERN_C int __fastcall TDrawGrid_GetClientHeight(TDrawGrid *DrawGrid);
EXTERN_C void __fastcall TDrawGrid_SetDefaultRowHeight(TDrawGrid *DrawGrid, const int RowHeight);
EXTERN_C void __fastcall TDrawGrid_SetDefaultColWidth(TDrawGrid *DrawGrid, const int ColWidth);
EXTERN_C void __fastcall TDrawGrid_SetRowCount(TDrawGrid *DrawGrid, const int RowCount);
EXTERN_C int __fastcall TDrawGrid_GetRowHeights(TDrawGrid *DrawGrid, const int Index);
EXTERN_C void __fastcall TDrawGrid_SetRowHeights(TDrawGrid *DrawGrid, const int Index, const int RowHeights);
EXTERN_C void __fastcall TDrawGrid_SetRow(TDrawGrid *DrawGrid, const int Row);
EXTERN_C void __fastcall TDrawGrid_SetTopRow(TDrawGrid *DrawGrid, const int TopRow);
EXTERN_C void __fastcall _TDrawGrid_MouseToCell(TDrawGrid *DrawGrid, int X, int Y, int *ARow, int *ACol);
#define TDrawGrid_MouseToCell(DrawGrid, X, Y, ACol, ARow) _TDrawGrid_MouseToCell(DrawGrid, X, Y, ARow, ACol)
EXTERN_C void __fastcall TDrawGrid_Repaint(TDrawGrid *DrawGrid);
