.486
.model flat, c

include ApplicationMessage.inc

includelib user32.lib

extrn _imp__CallWindowProcA@20:dword
extrn _imp__GetCapture@0:dword
extrn TMainForm_CheckTreeSize:proc
extrn TMainForm_OnCommand@16:proc
extrn DrawGuideBuffer@20:proc

public TMainForm_WindowProc@16
public TMainForm_PrevWindowProc

.data

TMainForm_PrevWindowProc       dword 0
TMainForm_SplitterLButtonDownX dword 0

.code

align 16

;EXTERN_C void __stdcall TMainForm_OnCommand(HWND hWnd, WORD wNotifyCode, WORD wID, HWND hwndCtl);
;EXTERN_C LRESULT __stdcall DrawGuideBuffer(WNDPROC lpPrevWndFunc, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
;
;WNDPROC TMainForm_PrevWindowProc       = NULL;
;DWORD   TMainForm_SplitterLButtonDownX = MAXDWORD;
;
;#define TMainForm_SplitterMoved() \
;	TMainForm_CheckTreeSize(TRUE)
;
;LRESULT CALLBACK TMainForm_WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
;{
;	if (uMsg == WM_LBUTTONDOWN || uMsg == WM_LBUTTONDBLCLK)
;	{
;		int x;
;
;		x = GET_X_LPARAM(lParam);
;		if (x >= MainForm->Splitter->Left && x <= MainForm->Splitter->Left + MainForm->Splitter->Width)
;			TMainForm_SplitterLButtonDownX = LOWORD(lParam);
;		else
;			TMainForm_SplitterLButtonDownX = MAXDWORD;
;	}
;	else if (uMsg == WM_LBUTTONUP)
;	{
;		DWORD x;
;
;		x = TMainForm_SplitterLButtonDownX;
;		TMainForm_SplitterLButtonDownX = MAXDWORD;
;		if (x <= MAXWORD && (WORD)x != LOWORD(lParam) && GetCapture() == hWnd)
;		{
;			LRESULT lResult = CallWindowProcA(TMainForm_PrevWindowProc, hwnd, WM_LBUTTONUP, wParam, lParam);
;			TMainForm_SplitterMoved();
;			return lResult;
;		}
;	}
;	else if (uMsg == WM_COMMAND)
;	{
;		TMainForm_OnCommand(hwnd, HIWORD(wParam), LOWORD(wParam), (HWND)lParam);
;	}
;	else if (uMsg == WM_DRAW_GUIDE_BUFFER)
;	{
;		return DrawGuideBuffer(TMainForm_PrevWindowProc, hwnd, uMsg, wParam, lParam);
;	}
;	return CallWindowProcA(TMainForm_PrevWindowProc, hwnd, uMsg, wParam, lParam);
;}
TMainForm_WindowProc@16 proc near

	FALSE            equ 0
	TRUE             equ 1
	MAXWORD          equ 0FFFFH
	MAXDWORD         equ 0FFFFFFFFH
	WM_COMMAND       equ 0111H
	WM_LBUTTONDOWN   equ 0201H
	WM_LBUTTONUP     equ 0202H
	WM_LBUTTONDBLCLK equ 0203H
	MainForm         equ 0064CE2CH

	mov     ecx, dword ptr [TMainForm_PrevWindowProc]
	pop     eax
	push    ecx
	push    eax

	lpPrevWndFunc equ <esp +  4>
	hWnd          equ <esp +  8>
	uMsg          equ <esp + 12>
	wParam        equ <esp + 16>
	lParam        equ <esp + 20>

	mov     ecx, dword ptr [uMsg]
	mov     eax, dword ptr [_imp__CallWindowProcA@20]
	cmp     ecx, WM_LBUTTONDOWN
	je      OnLButtonDown
	cmp     ecx, WM_LBUTTONUP
	je      OnLButtonUp
	cmp     ecx, WM_LBUTTONDBLCLK
	je      OnLButtonDown
	cmp     ecx, WM_COMMAND
	je      OnCommand
	cmp     ecx, WM_DRAW_GUIDE_BUFFER
	je      OnDrawGuideBuffer
	jmp     eax

align 16

OnLButtonDown:
	mov     ax, word ptr [lParam]
	mov     edx, MainForm
	shl     eax, 16
	mov     edx, dword ptr [edx]
	sar     eax, 16
	mov     edx, dword ptr [edx + 2FCH]
	mov     ecx, dword ptr [edx + 40H]
	mov     edx, dword ptr [edx + 48H]
	cmp     eax, ecx
	jl      OnLButtonDown2
	add     ecx, edx
	cmp     eax, ecx
	jg      OnLButtonDown2
	and     eax, 0FFFFH
	jmp     OnLButtonDown3
OnLButtonDown2:
	mov     eax, MAXDWORD
OnLButtonDown3:
	mov     dword ptr [TMainForm_SplitterLButtonDownX], eax
	jmp     dword ptr [_imp__CallWindowProcA@20]

align 16

OnLButtonUp:
	mov     ecx, dword ptr [TMainForm_SplitterLButtonDownX]
	mov     dx, word ptr [lParam]
	mov     dword ptr [TMainForm_SplitterLButtonDownX], MAXDWORD
	cmp     ecx, MAXWORD
	ja      OnLButtonUp2
	cmp     cx, dx
	je      OnLButtonUp2
	call    dword ptr [_imp__GetCapture@0]
	cmp     eax, dword ptr [hWnd]
	jne     OnLButtonUp3
	mov     ecx, dword ptr [lParam]
	mov     eax, dword ptr [wParam]
	push    ecx
	push    eax
	mov     ecx, dword ptr [hWnd + 8]
	mov     eax, dword ptr [TMainForm_PrevWindowProc]
	push    WM_LBUTTONUP
	push    ecx
	push    eax
	call    dword ptr [_imp__CallWindowProcA@20]
	mov     ecx, MainForm
	push    eax
	mov     ecx, dword ptr [ecx]
	push    TRUE
	push    ecx
	call    TMainForm_CheckTreeSize
	mov     eax, dword ptr [esp + 8]
	add     esp, 8 + 4
	ret     20
OnLButtonUp2:
	jmp     eax
OnLButtonUp3:
	jmp     dword ptr [_imp__CallWindowProcA@20]

align 16

OnCommand:
	mov     ecx, dword ptr [lParam]
	mov     edx, dword ptr [wParam]
	push    ecx
	push    edx
	shr     edx, 16
	mov     ecx, dword ptr [hWnd + 8]
	push    edx
	push    ecx
	push    eax
	jmp     TMainForm_OnCommand@16

align 16

OnDrawGuideBuffer:
	jmp     DrawGuideBuffer@20

TMainForm_WindowProc@16 endp

end
