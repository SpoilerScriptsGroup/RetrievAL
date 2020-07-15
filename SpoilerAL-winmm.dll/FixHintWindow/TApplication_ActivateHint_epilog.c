#include <windows.h>

static HANDLE hHook = NULL;
static HWND   hWnd;

#ifndef _M_IX86
static LRESULT CALLBACK GetMsgProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult;

	if (nCode < 0)
		goto PASS;
	if (GetCapture() == hWnd) {
		if (WindowFromPoint(((MSG *)lParam)->pt) == hWnd)
		PASS:
			return CallNextHookEx(hHook, nCode, wParam, lParam);
		ReleaseCapture();
	}
	lResult = CallNextHookEx(hHook, nCode, wParam, lParam);
	UnhookWindowsHookEx(hHook);
	hHook = NULL;
	return lResult;
}
#else
__declspec(naked) static LRESULT CALLBACK GetMsgProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	__asm
	{
		#define nCode               (esp + 4)
		#define wParam              (esp + 8)
		#define lParam              (esp + 12)
		#define offsetof_MSG_pt_x   20
		#define offsetof_MSG_pt_y   24

		cmp     dword ptr [nCode], 0                        // if (nCode < 0)
		jl      pass                                        //     goto PASS;
		call    GetCapture                                  // if (GetCapture() == hWnd) {
		mov     ecx, dword ptr [hWnd]
		mov     edx, dword ptr [lParam]
		cmp     eax, ecx
		jne     unhook
		mov     eax, dword ptr [edx + offsetof_MSG_pt_y]    //     if (WindowFromPoint(((MSG *)lParam)->pt) == hWnd)
		mov     ecx, dword ptr [edx + offsetof_MSG_pt_x]
		push    eax
		push    ecx
		call    WindowFromPoint
		cmp     dword ptr [hWnd], eax
		jne     release
	pass:
		mov     eax, dword ptr [hHook]                      //     PASS:
		pop     ecx                                         //         return CallNextHookEx(hHook, nCode, wParam, lParam);
		push    eax
		push    ecx
		jmp     CallNextHookEx

		align   16
	release:
		call    ReleaseCapture                              //     ReleaseCapture();
	unhook:
		mov     eax, dword ptr [lParam]                     // }
		mov     ecx, dword ptr [wParam]                     // lResult = CallNextHookEx(hHook, nCode, wParam, lParam);
		push    eax
		push    ecx
		mov     eax, dword ptr [nCode + 8]
		mov     ecx, dword ptr [hHook]
		push    eax
		push    ecx
		call    CallNextHookEx
		mov     ecx, dword ptr [hHook]
		push    eax
		push    ecx                                         // UnhookWindowsHookEx(hHook);
		call    UnhookWindowsHookEx
		xor     ecx, ecx                                    // hHook = NULL;
		pop     eax                                         // return lResult;
		mov     dword ptr [hHook], ecx
		ret     12

		#undef nCode
		#undef wParam
		#undef lParam
		#undef offsetof_MSG_pt_x
		#undef offsetof_MSG_pt_y
	}
}
#endif

__declspec(naked) void __cdecl TApplication_ActivateHint_epilog()
{
	EXTERN_C const DWORD _TWinControl_GetHandle;

	__asm
	{
		#define this ebx

		mov     eax, dword ptr [hHook]                      // if (hHook)
		mov     ecx, dword ptr [ebx]                        //     return;
		test    eax, eax
		jnz     epilog
		mov     eax, dword ptr [ecx + 96]                   // Control = *(TWinControl **)((char *)this + 96);
		test    eax, eax                                    // if (!Control)
		jz      epilog                                      //     return;
		call    dword ptr [_TWinControl_GetHandle]          // hWnd = Control->Handle;
		mov     dword ptr [hWnd], eax
		push    eax                                         // SetCapture(hWnd);
		call    SetCapture
		call    GetCurrentThreadId                          // hHook = SetWindowsHookExA(
		push    eax                                         //     WH_GETMESSAGE,
		push    0                                           //     GetMsgProc,
		call    GetModuleHandleA                            //     GetModuleHandleA(NULL),
		push    eax                                         //     GetCurrentThreadId());
		push    offset GetMsgProc
		push    WH_GETMESSAGE
		call    SetWindowsHookExA
		mov     dword ptr [hHook], eax
	epilog:
		pop     edi
		pop     esi
		pop     ebx
		mov     esp, ebp
		pop     ebp
		ret

		#undef this
	}
}
