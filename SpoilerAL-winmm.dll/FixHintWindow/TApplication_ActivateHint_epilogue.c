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
	RECT    rc;
	HANDLE  hPrevHook;

	if (nCode < 0)
		goto PASS_MESSAGE;
	if (GetCapture() == hWnd)
		if (GetWindowRect(hWnd, &rc) && PtInRect(&rc, ((MOUSEHOOKSTRUCT *)lParam)->pt))
		PASS_MESSAGE:
			return CallNextHookEx(hHook, nCode, wParam, lParam);
		else
			ReleaseCapture();
	hPrevHook = hHook;
	hHook = NULL;
	lResult = CallNextHookEx(hPrevHook, nCode, wParam, lParam);
	UnhookWindowsHookEx(hPrevHook);
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

		push    esi
		push    edi
		mov     esi, dword ptr [hHook]
		mov     edi, dword ptr [lParam + 8]
		cmp     dword ptr [nCode + 8], 0                                    // if (nCode < 0)
		jl      L1                                                          //     goto PASS_MESSAGE;
		call    GetCapture                                                  // if (GetCapture() == hWnd)
		mov     ecx, dword ptr [hWnd]
		sub     esp, 16
		cmp     eax, ecx
		jne     L3
		push    esp                                                         //     if (GetWindowRect(hWnd, &rc) && PtInRect(&rc, ((MOUSEHOOKSTRUCT *)lParam)->pt))
		push    eax
		call    GetWindowRect
		test    eax, eax
		jz      L2
		mov     eax, dword ptr [edi + offsetof_MOUSEHOOKSTRUCT_pt_x]
		mov     ecx, dword ptr [esp + offsetof_RECT_left]
		cmp     eax, ecx
		jl      L2
		mov     ecx, dword ptr [esp + offsetof_RECT_right]
		mov     edx, dword ptr [esp + offsetof_RECT_top]
		cmp     eax, ecx
		jge     L2
		mov     eax, dword ptr [edi + offsetof_MOUSEHOOKSTRUCT_pt_y]
		mov     ecx, dword ptr [esp + offsetof_RECT_bottom]
		cmp     eax, edx
		jl      L2
		cmp     eax, ecx
		jge     L2
		add     esp, 16

		align   16                                                          //     PASS_MESSAGE:
	L1:
		mov     eax, esi                                                    //         return CallNextHookEx(hHook, nCode, wParam, lParam);
		pop     edi
		pop     esi
		pop     ecx
		push    eax
		push    ecx
		jmp     CallNextHookEx

		align   16
	L2:
		                                                                    //     else
		call    ReleaseCapture                                              //         ReleaseCapture();
	L3:
		mov     eax, dword ptr [wParam + 24]                                // hPrevHook = hHook;
		mov     ecx, dword ptr [nCode  + 24]
		add     esp, 16
		mov     dword ptr [hHook], 0                                        // hHook = NULL;
		push    edi                                                         // lResult = CallNextHookEx(hPrevHook, nCode, wParam, lParam);
		push    eax
		push    ecx
		push    esi
		call    CallNextHookEx
		push    eax
		push    esi                                                         // UnhookWindowsHookEx(hPrevHook);
		call    UnhookWindowsHookEx
		pop     eax                                                         // return lResult;
		pop     edi
		pop     esi
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

__declspec(naked) void __cdecl TApplication_ActivateHint_epilogue()
{
	EXTERN_C const DWORD _TWinControl_GetHandle;

	__asm
	{
		#define this ebx

		mov     eax, dword ptr [hHook]
		mov     ecx, dword ptr [ebx]
		test    eax, eax
		jnz     epilogue
		mov     eax, dword ptr [ecx + 96]
		test    eax, eax
		jz      epilogue
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
	epilogue:
		pop     edi
		pop     esi
		pop     ebx
		mov     esp, ebp
		pop     ebp
		ret

		#undef this
	}
}
