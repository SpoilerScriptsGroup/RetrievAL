#include <windows.h>

#if WM_MOUSELAST < 0x020E
#undef WM_MOUSELAST
#define WM_MOUSELAST 0x020E
#endif

static HANDLE hHook = NULL;
static HWND   hWnd;

#ifndef _M_IX86
#define PtInRect(lprc, pt) ((pt).x >= (lprc)->left && (pt).x < (lprc)->right && (pt).y >= (lprc)->top && (pt).y < (lprc)->bottom)

static LRESULT CALLBACK GetMsgProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult;
	HANDLE  hOldHook;
	RECT    rc;

	if (nCode < 0)
		goto pass;
	if (GetCapture() != hWnd)
		goto unhook;
	if ((UINT)(((MSG *)lParam)->message - WM_MOUSEFIRST) < WM_MOUSELAST - WM_MOUSEFIRST + 1)
		if (GetWindowRect(hWnd, &rc) && PtInRect(&rc, ((MSG *)lParam)->pt))
			goto pass;
		else
			goto release;
	else if (((MSG *)lParam)->message == WM_ACTIVATE)
		goto release;
pass:
	return CallNextHookEx(hHook, nCode, wParam, lParam);

release:
	ReleaseCapture();
unhook:
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

	    cmp     dword ptr [nCode], 0                        //     if (nCode < 0)
	    jl      pass                                        //         goto pass;
	    call    GetCapture                                  //     if (GetCapture() != hWnd)
	    mov     ecx, dword ptr [hWnd]                       //         goto unhook;
	    mov     edx, dword ptr [lParam]
	    cmp     eax, ecx
	    jne     unhook
	    mov     eax, dword ptr [edx + offsetof_MSG_message] //     if ((UINT)(((MSG *)lParam)->message - WM_MOUSEFIRST) < WM_MOUSELAST - WM_MOUSEFIRST + 1)
	    mov     edx, eax
	    sub     eax, WM_MOUSEFIRST
	    cmp     eax, WM_MOUSELAST - WM_MOUSEFIRST + 1
	    jae     isactivate
	    sub     esp, 16                                     //         if (GetWindowRect(hWnd, &rc) && PtInRect(&rc, ((MSG *)lParam)->pt))
	    push    esp                                         //             goto pass;
	    push    ecx                                         //         else
	    call    GetWindowRect                               //             goto release;
	    test    eax, eax
	    jz      nowhere
	    mov     eax, dword ptr [lParam + 16]
	    mov     ecx, dword ptr [esp + offsetof_RECT_left]
	    mov     eax, dword ptr [eax + offsetof_MSG_pt_x]
	    mov     edx, dword ptr [esp + offsetof_RECT_right]
	    cmp     eax, ecx
	    jl      nowhere
	    cmp     eax, edx
	    jge     nowhere
	    mov     eax, dword ptr [lParam + 16]
	    mov     ecx, dword ptr [esp + offsetof_RECT_top]
	    mov     eax, dword ptr [eax + offsetof_MSG_pt_y]
	    mov     edx, dword ptr [esp + offsetof_RECT_bottom]
	    cmp     eax, ecx
	    jl      nowhere
	    cmp     eax, edx
	    jge     nowhere
	    add     esp, 16
	    jmp     pass

	isactivate:
	    cmp     edx, WM_ACTIVATE                            //     else if (((MSG *)lParam)->message == WM_ACTIVATE)
	    je      release                                     //         goto release;
	pass:                                                   // pass:
	    mov     eax, dword ptr [hHook]                      //     return CallNextHookEx(hHook, nCode, wParam, lParam);
	    pop     ecx
	    push    eax
	    push    ecx
	    jmp     CallNextHookEx

		align   16
	nowhere:
	    add     esp, 16
	release:                                                // release:
	    call    ReleaseCapture                              //     ReleaseCapture();
	unhook:                                                 // unhook:
	    mov     eax, dword ptr [lParam]
	    mov     ecx, dword ptr [wParam]
	    push    esi
	    push    eax
	    mov     eax, dword ptr [nCode + 8]
	    mov     esi, dword ptr [hHook]                      //     hOldHook = hHook;
	    push    ecx
	    push    eax
	    push    esi
	    mov     dword ptr [hHook], 0                        //     hHook = NULL;
	    call    CallNextHookEx                              //     lResult = CallNextHookEx(hOldHook, nCode, wParam, lParam);
	    push    eax
	    push    esi                                         //     UnhookWindowsHookEx(hOldHook);
	    call    UnhookWindowsHookEx
	    pop     eax                                         //     return lResult;
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

	    mov     eax, dword ptr [hHook]                      //     if (hHook)
	    mov     ecx, dword ptr [ebx]                        //         return;
	    test    eax, eax
	    jnz     epilogue
	    mov     eax, dword ptr [ecx + 96]                   //     Control = (TWinControl *)((char *)this + 96);
	    test    eax, eax                                    //     if (!Control)
	    jz      epilogue                                    //         return;
	    call    dword ptr [_TWinControl_GetHandle]          //     hWnd = Control->Handle;
	    mov     dword ptr [hWnd], eax
	    push    eax                                         //     SetCapture(hWnd);
	    call    SetCapture
	    call    GetCurrentThreadId                          //     hHook = SetWindowsHookExA(
	    push    eax                                         //         WH_GETMESSAGE,
	    push    0                                           //         GetMsgProc,
	    call    GetModuleHandleA                            //         GetModuleHandleA(NULL),
	    push    eax                                         //         GetCurrentThreadId());
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
