.486
.model flat

includelib user32.lib

extrn __imp__CallWindowProcA@20:dword
extrn _TMainForm_OnDGridLButtonDown@8:proc
extrn _TMainForm_OnDGridLButtonUp@8:proc
extrn @_TMainForm_FormMouseWheel@24:proc

public _TMainForm_DGridProc@16
public _TMainForm_PrevDGridProc
public _TMainForm_DGridLButtonDblClk

.data

_TMainForm_PrevDGridProc      dword 0
_TMainForm_DGridLButtonDblClk dword 0

.code

align 16

;EXTERN_C void __stdcall TMainForm_OnDGridLButtonDown(TMainForm *_this, LPARAM lParam);
;EXTERN_C void __stdcall TMainForm_OnDGridLButtonUp(TMainForm *_this, LPARAM lParam);
;EXTERN_C void __msfastcall _TMainForm_FormMouseWheel(TMainForm *_this, LPVOID Sender, int Shift, BOOLEAN *Handled, POINT *MousePos, int WheelDelta);
;#define TMainForm_FormMouseWheel(_this, Sender, Shift, Handled, MousePos, WheelDelta) TMainForm_FormMouseWheel(_this, Sender, Shift, WheelDelta, MousePos, Handled)
;
;WNDPROC TMainForm_PrevDGridProc      = NULL;
;BOOLEAN TMainForm_DGridLButtonDblClk = FALSE;
;
;LRESULT CALLBACK TMainForm_DGridProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
;{
;	if (uMsg == WM_LBUTTONDOWN)
;	{
;		TMainForm_DGridLButtonDblClk = FALSE;
;		TMainForm_OnDGridLButtonDown(MainForm, lParam);
;	}
;	else if (uMsg == WM_LBUTTONUP)
;	{
;		TMainForm_OnDGridLButtonUp(MainForm, lParam);
;	}
;	else if (uMsg == WM_LBUTTONDBLCLK)
;	{
;		TMainForm_OnDGridLButtonDblClk = TRUE;
;	}
;	else if (uMsg == WM_MOUSEWHEEL)
;	{
;		BOOLEAN Handled;
;
;		Handled = FALSE;
;		TMainForm_FormMouseWheel(
;			MainForm,
;			NULL,
;			0,
;			GET_WHEEL_DELTA_WPARAM(wParam),
;			(POINT){ GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) },
;			&Handled);
;		return 0;
;	}
;	else if (uMsg == WM_KEYDOWN)
;	{
;		Handled = FALSE;
;		TMainForm_HotKeyEditKeyDown(
;			MainForm,
;			NULL,
;			(LPWORD)&wParam,
;			0);
;		return 0;
;	}
;	return CallWindowProcA(TMainForm_PrevDGridProc, hwnd, uMsg, wParam, lParam);
;}
_TMainForm_DGridProc@16 proc near

	FALSE            equ 0
	TRUE             equ 1
	WM_KEYDOWN       equ 0100H
	WM_LBUTTONDOWN   equ 0201H
	WM_LBUTTONUP     equ 0202H
	WM_LBUTTONDBLCLK equ 0203H
	WM_MOUSEWHEEL    equ 020AH

	TMainForm_HotKeyEditKeyDown equ 00443054H
	MainForm                    equ 0064CE2CH

	mov     ecx, dword ptr [_TMainForm_PrevDGridProc]
	pop     eax
	push    ecx
	push    eax

	lpPrevWndFunc equ <esp +  4>
	hWnd          equ <esp +  8>
	uMsg          equ <esp + 12>
	wParam        equ <esp + 16>
	lParam        equ <esp + 20>

	mov     ecx, dword ptr [uMsg]
	mov     eax, dword ptr [__imp__CallWindowProcA@20]
	cmp     ecx, WM_LBUTTONDOWN
	je      L1
	cmp     ecx, WM_LBUTTONUP
	je      L2
	cmp     ecx, WM_LBUTTONDBLCLK
	je      L3
	cmp     ecx, WM_MOUSEWHEEL
	je      L4
	cmp     ecx, WM_KEYDOWN
	je      L5
	jmp     eax
L1:
	mov     byte ptr [_TMainForm_DGridLButtonDblClk], FALSE
	mov     ecx, MainForm
	mov     edx, dword ptr [lParam]
	mov     ecx, dword ptr [ecx]
	push    edx
	push    ecx
	push    eax
	jmp     _TMainForm_OnDGridLButtonDown@8
L2:
	mov     ecx, MainForm
	mov     edx, dword ptr [lParam]
	mov     ecx, dword ptr [ecx]
	push    edx
	push    ecx
	push    eax
	jmp     _TMainForm_OnDGridLButtonUp@8
L3:
	mov     byte ptr [_TMainForm_DGridLButtonDblClk], TRUE
	jmp     eax
L4:
	mov     ecx, dword ptr [wParam]
	mov     dx, word ptr [lParam]
	sar     ecx, 16
	sub     esp, 12
	shl     edx, 16
	mov     eax, esp
	sar     edx, 16
	push    ecx
	mov     ecx, dword ptr [lParam + 16]
	push    eax
	sar     ecx, 16
	mov     dword ptr [eax], edx
	mov     dword ptr [eax + 4], ecx
	add     eax, 8
	xor     edx, edx
	mov     ecx, MainForm
	push    eax
	push    edx
	mov     dword ptr [eax], edx
	mov     ecx, dword ptr [ecx]
	call    @_TMainForm_FormMouseWheel@24
	add     esp, 12
	xor     eax, eax
	ret     20
L5:
	mov     ecx, esp
	mov     eax, MainForm
	add     ecx, 16
	xor     edx, edx
	mov     eax, dword ptr [eax]
	push    edx
	push    offset L6
	push    TMainForm_HotKeyEditKeyDown
	ret
	align   16
L6:
	xor     eax, eax
	ret     20

_TMainForm_DGridProc@16 endp

end
