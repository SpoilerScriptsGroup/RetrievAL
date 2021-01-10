#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "TGuideForm.h"
#include "TWinControl.h"
#include "SnapWindow.h"

static void __fastcall ctor(TGuideForm *this);

__declspec(naked) void __cdecl TGuideForm_ctor()
{
	__asm
	{
		#define this (ebp - 4H)

		mov     ecx, dword ptr [esp + 4]
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

#if SNAPWINDOW
	AttachSnapWindow(hWnd);
#endif

	hWnd = TWinControl_GetHandle(this->UserModeCmbBox);
	SendMessageA(hWnd, CB_ADDSTRING, 0, (LPARAM)"�� SSG�f�o�b�O���[�h - �}�E�X�I�[�o�[������");
	SendMessageA(hWnd, CB_ADDSTRING, 0, (LPARAM)"�� SSG�f�o�b�O���[�h - �}�E�X�I�[�o�[�A���Z������");
}
