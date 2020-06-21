#include <windows.h>
#include <stdbool.h>

static HANDLE hHook = NULL;
static HWND   hWnd  = NULL;

#ifndef _M_IX86
#define PtInRect inline_PtInRect
static __inline bool PtInRect(CONST RECT *lprc, POINT pt)
{
	return  pt.x >= lprc->left && pt.x < lprc->right && pt.y >= lprc->top && pt.y < lprc->bottom;
}

static LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult;

	if (GetCapture() == hWnd)
	{
		RECT rc;

		if (GetWindowRect(hWnd, &rc) && PtInRect(&rc, ((MOUSEHOOKSTRUCT *)lParam)->pt))
			return CallNextHookEx(hHook, nCode, wParam, lParam);
		ReleaseCapture();
	}
	lResult = CallNextHookEx(hHook, nCode, wParam, lParam);
	UnhookWindowsHookEx(hHook);
	hHook = NULL;
	return lResult;
}
#else
__declspec(naked) static LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	__asm
	{
		#define nCode                         (esp + 4)
		#define wParam                        (esp + 8)
		#define lParam                        (esp + 12)
		#define offsetof_MOUSEHOOKSTRUCT_pt_x 0
		#define offsetof_MOUSEHOOKSTRUCT_pt_y 4
		#define offsetof_RECT_left            0
		#define offsetof_RECT_top             4
		#define offsetof_RECT_right           8
		#define offsetof_RECT_bottom          12

		call    GetCapture                                                  // if (GetCapture() == hWnd)
		cmp     eax, dword ptr [hWnd]
		jne     L2                                                          // {
		sub     esp, 16                                                     //     if (GetWindowRect(hWnd, &rc) && PtInRect(&rc, ((MOUSEHOOKSTRUCT *)lParam)->pt))
		push    esp
		push    eax
		call    GetWindowRect
		test    eax, eax
		jz      L1
		mov     eax, dword ptr [lParam + 16]
		mov     ecx, dword ptr [eax + offsetof_MOUSEHOOKSTRUCT_pt_x]
		mov     edx, dword ptr [eax + offsetof_MOUSEHOOKSTRUCT_pt_y]
		cmp     ecx, dword ptr [esp + offsetof_RECT_left]
		jl      L1
		cmp     ecx, dword ptr [esp + offsetof_RECT_right]
		jge     L1
		cmp     edx, dword ptr [esp + offsetof_RECT_top]
		jl      L1
		cmp     edx, dword ptr [esp + offsetof_RECT_bottom]
		jge     L1
		mov     ecx, dword ptr [wParam + 16]                                //         return CallNextHookEx(hHook, nCode, wParam, lParam);
		add     esp, 16
		push    eax
		push    ecx
		mov     eax, dword ptr [nCode]
		mov     ecx, dword ptr [hHook]
		push    eax
		push    ecx
		call    CallNextHookEx
		ret     12

		align   16
	L1:
		add     esp, 16                                                     //     ReleaseCapture();
		call    ReleaseCapture                                              // }
	L2:
		mov     eax, dword ptr [lParam]                                     // lResult = CallNextHookEx(hHook, nCode, wParam, lParam);
		mov     ecx, dword ptr [wParam]
		push    eax
		push    ecx
		mov     eax, dword ptr [nCode + 8]
		mov     ecx, dword ptr [hHook]
		push    eax
		push    ecx
		call    CallNextHookEx
		mov     ecx, dword ptr [hHook]                                      // UnhookWindowsHookEx(hHook);
		push    eax
		push    ecx
		call    UnhookWindowsHookEx
		pop     eax                                                         // hHook = NULL;
		xor     ecx, ecx                                                    // return lResult;
		mov     dword ptr [hHook], ecx
		ret     12

		#undef nCode
		#undef wParam
		#undef lParam
		#undef offsetof_MOUSEHOOKSTRUCT_pt_x
		#undef offsetof_MOUSEHOOKSTRUCT_pt_y
		#undef offsetof_RECT_left
		#undef offsetof_RECT_top
		#undef offsetof_RECT_right
		#undef offsetof_RECT_bottom
	}
}
#endif

__declspec(naked) void __cdecl TTitleSelectForm_TitleDGridActivateHint()
{
	EXTERN_C const DWORD _TApplication_ActivateHint;
	EXTERN_C const DWORD _TWinControl_GetHandle;

	__asm
	{
		#define Application                          eax
		#define MousePos                             edx
		#define this                                 (ebx - 4)
		#define offsetof_TTitleSelectForm_TitleDGrid 824

		call    dword ptr [_TApplication_ActivateHint]                      // Application->ActivateHint(MousePos);
		mov     eax, dword ptr [hHook]                                      // if (hHook)
		mov     ecx, dword ptr [this]                                       //     return;
		test    eax, eax
		jnz     L1
		mov     eax, dword ptr [ecx + offsetof_TTitleSelectForm_TitleDGrid] // hWnd = this->TitleDGrid->Handle;
		call    dword ptr [_TWinControl_GetHandle]
		mov     dword ptr [hWnd], eax
		push    eax                                                         // SetCapture(hWnd);
		call    SetCapture
		call    GetCurrentThreadId                                          // hHook = SetWindowsHookExA(
		push    eax                                                         //     WH_MOUSE,
		push    0                                                           //     MouseProc,
		call    GetModuleHandleA                                            //     GetModuleHandleA(NULL),
		push    eax                                                         //     GetCurrentThreadId());
		push    offset MouseProc
		push    WH_MOUSE
		call    SetWindowsHookExA
		mov     dword ptr [hHook], eax
	L1:
		ret

		#undef Application
		#undef MousePos
		#undef this
		#undef offsetof_TTitleSelectForm_TitleDGrid
	}
}
