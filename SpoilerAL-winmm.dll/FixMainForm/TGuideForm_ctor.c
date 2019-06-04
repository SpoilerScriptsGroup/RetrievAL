#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "TGuideForm.h"
#include "TWinControl.h"
#include "..\SnapWindow\Attach_SnapWindow.h"
#if SNAP_WINDOW
#include "..\SnapWindow\SnapWindow.h"
#endif

static void __fastcall ctor(TGuideForm *this);

__declspec(naked) void __cdecl TGuideForm_ctor()
{
	__asm
	{
		#define this (ebp - 4H)

		mov     ecx, dword ptr[esp + 4]
		call    string_ctor
		mov     ecx, dword ptr [this]
		jmp     ctor

		#undef this
	}
}

static void __fastcall ctor(TGuideForm *this)
{
	HWND hWnd;

	hWnd = TWinControl_GetHandle(this);
	SetWindowLongA(hWnd, GWL_STYLE, GetWindowLongA(hWnd, GWL_STYLE) & ~WS_MINIMIZEBOX);

#if SNAP_WINDOW
	AttachSnapWindow(hWnd);
#endif

	hWnd = TWinControl_GetHandle(this->UserModeCmbBox);
	SendMessageA(hWnd, CB_ADDSTRING, 0, (LPARAM)"→ SSGデバッグモード - マウスオーバーを除く");
	SendMessageA(hWnd, CB_ADDSTRING, 0, (LPARAM)"→ SSGデバッグモード - マウスオーバー、演算を除く");
}
