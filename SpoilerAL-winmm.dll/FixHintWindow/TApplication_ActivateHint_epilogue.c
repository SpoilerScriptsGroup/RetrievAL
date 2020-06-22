#include <windows.h>

#if WM_MOUSELAST < 0x020E
#undef WM_MOUSELAST
#define WM_MOUSELAST 0x020E
#endif

static HANDLE hHook = NULL;
static HWND   hWnd  = NULL;

#ifndef _M_IX86
#define PtInRect(lprc, pt) ((pt).x >= (lprc)->left && (pt).x < (lprc)->right && (pt).y >= (lprc)->top && (pt).y < (lprc)->bottom)

static LRESULT CALLBACK GetMsgProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult;
	RECT    rc;
	HANDLE  hOldHook;

	if (nCode < 0)
		goto PASS;
	if (GetCapture() != hWnd)
		goto UNHOOK;
	if ((UINT)(((MSG *)lParam)->message - WM_MOUSEFIRST) < WM_MOUSELAST - WM_MOUSEFIRST + 1)
		if (GetWindowRect(hWnd, &rc) && PtInRect(&rc, ((MSG *)lParam)->pt))
			goto PASS;
		else
			goto RELEASE;
	else if (((MSG *)lParam)->message == WM_ACTIVATE)
		goto RELEASE;
PASS:
	return CallNextHookEx(hHook, nCode, wParam, lParam);

RELEASE:
	ReleaseCapture();
UNHOOK:
	hOldHook = hHook;
	hHook = NULL;
	lResult = CallNextHookEx(hOldHook, nCode, wParam, lParam);
	UnhookWindowsHookEx(hOldHook);
	return lResult;
}
#else
__declspec(naked) static LRESULT CALLBACK GetMsgProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	__asm
	{
	    #define nCode                   (esp + 4)
	    #define wParam                  (esp + 8)
	    #define lParam                  (esp + 12)
	    #define offsetof_MSG_message    4
	    #define offsetof_MSG_pt_x       20
	    #define offsetof_MSG_pt_y       24
	    #define offsetof_RECT_left      0
	    #define offsetof_RECT_top       4
	    #define offsetof_RECT_right     8
	    #define offsetof_RECT_bottom    12

	    cmp     dword ptr [nCode], 0                                        // if (nCode < 0)
	    jl      PASS                                                        //     goto PASS;
	    call    GetCapture                                                  // if (GetCapture() != hWnd)
	    mov     ecx, dword ptr [hWnd]                                       //     goto UNHOOK;
	    mov     edx, dword ptr [lParam]
	    cmp     eax, ecx
	    jne     UNHOOK
	    mov     eax, dword ptr [edx + offsetof_MSG_message]                 // if ((UINT)(((MSG *)lParam)->message - WM_MOUSEFIRST) < WM_MOUSELAST - WM_MOUSEFIRST + 1)
	    mov     ecx, eax
	    sub     eax, WM_MOUSEFIRST
	    cmp     eax, WM_MOUSELAST - WM_MOUSEFIRST + 1
	    jae     ISACTIVATE
	    sub     esp, 16                                                     //     if (GetWindowRect(hWnd, &rc) && PtInRect(&rc, ((MSG *)lParam)->pt))
	    mov     eax, dword ptr [hWnd]                                       //         goto PASS;
	    push    esp                                                         //     else
	    push    eax                                                         //         goto RELEASE;
	    call    GetWindowRect
	    test    eax, eax
	    jz      NOWHERE
	    mov     eax, dword ptr [lParam + 16]
	    mov     ecx, dword ptr [esp + offsetof_RECT_left]
	    mov     eax, dword ptr [eax + offsetof_MSG_pt_x]
	    mov     edx, dword ptr [esp + offsetof_RECT_right]
	    cmp     eax, ecx
	    jl      NOWHERE
	    cmp     eax, edx
	    jge     NOWHERE
	    mov     eax, dword ptr [lParam + 16]
	    mov     ecx, dword ptr [esp + offsetof_RECT_top]
	    mov     eax, dword ptr [eax + offsetof_MSG_pt_y]
	    mov     edx, dword ptr [esp + offsetof_RECT_bottom]
	    cmp     eax, ecx
	    jl      NOWHERE
	    cmp     eax, edx
	    jge     NOWHERE
	    add     esp, 16
	    jmp     PASS

	ISACTIVATE:
	    cmp     ecx, WM_ACTIVATE                                            // else if (((MSG *)lParam)->message == WM_ACTIVATE)
	    je      RELEASE                                                     //     goto RELEASE;
	PASS:                                                                   // PASS:
	    mov     eax, dword ptr [hHook]                                      //     return CallNextHookEx(hHook, nCode, wParam, lParam);
	    pop     ecx
	    push    eax
	    push    ecx
	    jmp     CallNextHookEx

	NOWHERE:
	    add     esp, 16
	RELEASE:                                                                // RELEASE:
	    call    ReleaseCapture                                              //     ReleaseCapture();
	UNHOOK:                                                                 // UNHOOK:
	    mov     eax, dword ptr [lParam]
	    mov     ecx, dword ptr [wParam]
	    push    esi
	    push    eax
	    mov     eax, dword ptr [nCode + 8]
	    mov     esi, dword ptr [hHook]                                      // hOldHook = hHook;
	    push    ecx
	    push    eax
	    push    esi
	    mov     dword ptr [hHook], 0                                        // hHook = NULL;
	    call    CallNextHookEx                                              // lResult = CallNextHookEx(hOldHook, nCode, wParam, lParam);
	    push    eax
	    push    esi                                                         // UnhookWindowsHookEx(hOldHook);
	    call    UnhookWindowsHookEx
	    pop     eax                                                         // return lResult;
	    pop     esi
	    ret     12

	    #undef nCode
	    #undef wParam
	    #undef lParam
	    #undef offsetof_MSG_message
	    #undef offsetof_MSG_pt_x
	    #undef offsetof_MSG_pt_y
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
		push    eax                                                         //     WH_GETMESSAGE,
		push    0                                                           //     GetMsgProc,
		call    GetModuleHandleA                                            //     GetModuleHandleA(NULL),
		push    eax                                                         //     GetCurrentThreadId());
		push    offset GetMsgProc
		push    WH_GETMESSAGE
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
