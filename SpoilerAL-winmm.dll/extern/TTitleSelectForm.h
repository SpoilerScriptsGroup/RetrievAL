#pragma once

#include <windows.h>
#include "bcb6_std_vector.h"
#include "bcb6_std_string.h"
#include "TDrawGrid.h"
#include "TSSGScriptStruct.h"

#pragma pack(push, 1)
typedef struct _TTitleSelectForm
{
	LPVOID           *VTable;
	BYTE             padding1[748];
	LPVOID           Panel_T;
	LPVOID           Panel_C;
	LPVOID           Splitter1;
	LPVOID           Panel_TR;
	LPVOID           CancelBtn;
	LPVOID           OKBtn;
	LPVOID           Panel_B;
	LPVOID           Splitter2;
	LPVOID           Panel_TC;
	LPVOID           SortCmbBox;
	LPVOID           Label3;
	LPVOID           Label2;
	LPVOID           Panel_BT;
	LPVOID           FindUDBtn;
	LPVOID           Label1;
	LPVOID           FindEdit;
	LPVOID           FindLBox;
	LPVOID           FindStopBtn;
	TDrawGrid        *TitleDGrid;
	TDrawGrid        *KeyDGrid;
	LPVOID           BegginerBtn;
	LPVOID           Panel_BB;
	LPVOID           SSGPathEdit;
	LPVOID           SSGPathBtn;
	bcb6_std_string  sortKey;
	LCID             sortLCID;
	bcb6_std_vector  *keyToIndexMap;
	bcb6_std_vector  *scriptVec;
	bcb6_std_vector  *toScriptVec;
	long             findMode;
	BYTE             padding2[4];
	bcb6_std_vector  findIndexVec;
	LPVOID           nowOnMouseScript;
	TSSGScriptStruct *selectScript;
} TTitleSelectForm;
#pragma pack(pop)

EXTERN_C bcb6_std_string * __cdecl TTitleSelectForm_CnvString(bcb6_std_string *Result, TTitleSelectForm *this, const bcb6_std_string *Src);
EXTERN_C BOOLEAN __cdecl TTitleSelectForm_SortFunc(TTitleSelectForm *this, const TSSGScriptStruct *SS1, const TSSGScriptStruct *SS2);

