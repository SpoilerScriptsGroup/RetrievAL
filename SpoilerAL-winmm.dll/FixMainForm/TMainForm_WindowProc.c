#include "TMainForm.h"
#include "ApplicationMessage.h"

void __stdcall TMainForm_OnCommand(HWND hWnd, WORD wNotifyCode, WORD wID, HWND hwndCtl);
void __stdcall TMainForm_OnShowWindow(HWND hWnd, BOOL bShow, UINT nStatus);
LRESULT __stdcall DrawGuideBuffer(WNDPROC lpPrevWndFunc, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static void(__cdecl * const TSearchForm_DrawProgress)(WPARAM searchForm, unsigned long Pos) = (LPVOID)0x004948E4;

WNDPROC TMainForm_PrevWindowProc = NULL;
static DWORD TMainForm_SplitterLButtonDownX = MAXDWORD;

__declspec(naked) LRESULT CALLBACK TMainForm_WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	/*
	#define TMainForm_SplitterMoved() \
		TMainForm_CheckTreeSize(TRUE)

	if (uMsg == WM_LBUTTONDOWN || uMsg == WM_LBUTTONDBLCLK)
	{
		int x;

		x = GET_X_LPARAM(lParam);
		if (x >= MainForm->Splitter->Left && x < MainForm->Splitter->Left + MainForm->Splitter->Width)
			TMainForm_SplitterLButtonDownX = LOWORD(lParam);
		else
			TMainForm_SplitterLButtonDownX = MAXDWORD;
	}
	else if (uMsg == WM_LBUTTONUP)
	{
		DWORD x;

		x = TMainForm_SplitterLButtonDownX;
		TMainForm_SplitterLButtonDownX = MAXDWORD;
		if (x <= MAXWORD && (WORD)x != LOWORD(lParam) && GetCapture() == hWnd)
		{
			LRESULT lResult;

			lResult = CallWindowProcA(TMainForm_PrevWindowProc, hwnd, WM_LBUTTONUP, wParam, lParam);
			TMainForm_SplitterMoved();
			return lResult;
		}
	}
	else if (uMsg == WM_COMMAND)
	{
		TMainForm_OnCommand(hwnd, HIWORD(wParam), LOWORD(wParam), (HWND)lParam);
	}
	else if (uMsg == WM_SHOWWINDOW)
	{
		TMainForm_OnShowWindow(hwnd, (BOOL)wParam, (UINT)lParam);
	}
	else if (uMsg == WM_DRAW_GUIDE_BUFFER)
	{
		return DrawGuideBuffer(TMainForm_PrevWindowProc, hwnd, uMsg, wParam, lParam);
	}
	return CallWindowProcA(TMainForm_PrevWindowProc, hwnd, uMsg, wParam, lParam);
	*/

	__asm
	{
		mov     eax, dword ptr [TMainForm_PrevWindowProc]
		pop     ecx
		push    eax
		push    ecx

		#define lpPrevWndFunc (esp +  4)
		#define hWnd          (esp +  8)
		#define uMsg          (esp + 12)
		#define wParam        (esp + 16)
		#define lParam        (esp + 20)

		mov     eax, dword ptr [uMsg]
		mov     ecx, dword ptr [lParam]
		cmp     eax, WM_LBUTTONDOWN
		je      OnLButtonDown
		cmp     eax, WM_LBUTTONUP
		je      OnLButtonUp
		cmp     eax, WM_LBUTTONDBLCLK
		je      OnLButtonDown
		cmp     eax, WM_COMMAND
		je      OnCommand
		cmp     eax, WM_SHOWWINDOW
		je      OnShowWindow
		cmp     eax, WM_DRAW_GUIDE_BUFFER
		je      OnDrawGuideBuffer
		cmp     eax, WM_DRAW_PROGRESS
		je      OnDrawProgress
		jmp     CallWindowProcA

		align   16
	OnLButtonDown:
		shl     ecx, 16
		mov     eax, dword ptr ds:[_MainForm]
		sar     ecx, 16
		mov     eax, dword ptr [eax + 2FCH]
		mov     edx, dword ptr [eax + 40H]
		mov     eax, dword ptr [eax + 48H]
		cmp     ecx, edx
		jl      OnLButtonDown2
		add     edx, eax
		cmp     ecx, edx
		jge     OnLButtonDown2
		and     ecx, 0FFFFH
		jmp     OnLButtonDown3

	OnLButtonDown2:
		or      ecx, -1
	OnLButtonDown3:
		mov     dword ptr [TMainForm_SplitterLButtonDownX], ecx
		jmp     CallWindowProcA

		align   16
	OnLButtonUp:
		mov     eax, dword ptr [TMainForm_SplitterLButtonDownX]
		mov     dword ptr [TMainForm_SplitterLButtonDownX], MAXDWORD
		cmp     eax, MAXWORD
		ja      OnLButtonUp2
		cmp     ax, cx
		je      OnLButtonUp2
		call    GetCapture
		cmp     eax, dword ptr [hWnd]
		jne     OnLButtonUp2
		mov     ecx, dword ptr [lParam]
		mov     eax, dword ptr [wParam]
		push    ecx
		push    eax
		mov     ecx, dword ptr [hWnd + 8]
		mov     eax, dword ptr [TMainForm_PrevWindowProc]
		push    WM_LBUTTONUP
		push    ecx
		push    eax
		call    CallWindowProcA
		mov     ecx, dword ptr ds:[_MainForm]
		push    eax
		push    TRUE
		push    ecx
		call    TMainForm_CheckTreeSize
		mov     eax, dword ptr [esp + 8]
		add     esp, 8 + 4
		ret     20

	OnLButtonUp2:
		jmp     CallWindowProcA

		align   16
	OnCommand:
		mov     eax, dword ptr [wParam]
		mov     edx, dword ptr [hWnd]
		push    ecx
		push    eax
		shr     eax, 16
		mov     ecx, CallWindowProcA
		push    eax
		push    edx
		push    ecx
		jmp     TMainForm_OnCommand

		align   16
	OnShowWindow:
		mov     eax, dword ptr [wParam]
		mov     edx, dword ptr [hWnd]
		push    ecx
		push    eax
		mov     ecx, CallWindowProcA
		push    edx
		push    ecx
		jmp     TMainForm_OnShowWindow

		align   16
	OnDrawGuideBuffer:
		jmp     DrawGuideBuffer

		align   16
	OnDrawProgress:
		mov     eax, dword ptr [wParam]
		push    ecx
		push    eax
		call    TSearchForm_DrawProgress
		add     esp, 8
		xor     eax, eax
		ret     20

		#undef lpPrevWndFunc
		#undef hWnd
		#undef uMsg
		#undef wParam
		#undef lParam
	}
}
