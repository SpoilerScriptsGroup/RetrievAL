#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "TMainForm.h"

#pragma warning(disable:4733)

#define ssShift  0x0001
#define ssAlt    0x0002
#define ssCtrl   0x0004
#define ssLeft   0x0008
#define ssRight  0x0010
#define ssMiddle 0x0020
#define ssDouble 0x0040

static WNDPROC TMainForm_PrevNewValProc = NULL;

static LRESULT CALLBACK TMainForm_NewValProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static unsigned long __cdecl NewVal_OnPaste();

/*
	call    TMainForm_LoadCLD_Footer                ; 004026C8 _ E8, 0019E587
	nop                                             ; 004026CD _ 90
*/
void __cdecl TMainForm_FormClose_Header()
{
	extern const DWORD _TWinControl_GetHandle;

	/*
		if (TMainForm_PrevNewValProc)
		{
			SetWindowLongA(calcImage->valBox[1].edit->Handle, GWL_WNDPROC, (long)TMainForm_PrevNewValProc);
			TMainForm_PrevNewValProc = NULL;
		}
	*/
	__asm
	{
		#define this                         esi
		#define offsetof_TMainForm_calcImage 1320
		#define offsetof_TCalcImage_valBox   440
		#define sizeof_TCalcValBox           20
		#define offsetof_TCalcValBox_edit    0

		mov     eax, dword ptr [TMainForm_PrevNewValProc]
		test    eax, eax
		jz      L1
		push    eax
		mov     eax, dword ptr [this + offsetof_TMainForm_calcImage]
		push    GWLP_WNDPROC
		mov     eax, dword ptr [eax + offsetof_TCalcImage_valBox]
		mov     eax, dword ptr [eax + sizeof_TCalcValBox]
		call    dword ptr[_TWinControl_GetHandle]
		push    eax
		mov     dword ptr [TMainForm_PrevNewValProc], 0
		call    SetWindowLongPtrA
	L1:
		lea     edx, [esi + 5D8H]
		ret

		#undef this
		#undef offsetof_TMainForm_calcImage
		#undef offsetof_TCalcImage_valBox
		#undef sizeof_TCalcValBox
		#undef offsetof_TCalcValBox_edit
	}
}

/*
	call    TMainForm_LoadCLD_Footer                       ; 0045622F _ E8, 0019E587
*/
__declspec(naked) void __cdecl TMainForm_LoadCLD_Footer()
{
	extern const DWORD _TWinControl_GetHandle;

	/*
		TMainForm_PrevNewValProc = (WNDPROC)SetWindowLongPtrA(CalcImage->valBox[1].edit->Handle, GWLP_WNDPROC, (LONG_PTR)TMainForm_NewValProc);
	*/
	__asm
	{
		#define CalcImage                  (ebp + 36)
		#define offsetof_TCalcImage_valBox 440
		#define sizeof_TCalcValBox         20
		#define offsetof_TCalcValBox_edit  0

		mov     eax, dword ptr [CalcImage]
		push    offset TMainForm_NewValProc
		push    GWLP_WNDPROC
		mov     eax, dword ptr [eax + offsetof_TCalcImage_valBox]
		mov     eax, dword ptr [eax + sizeof_TCalcValBox]
		call    dword ptr[_TWinControl_GetHandle]
		push    eax
		call    SetWindowLongPtrA
		mov     dword ptr [TMainForm_PrevNewValProc], eax
		dec     dword ptr [ebx + 1CH]
		dec     dword ptr [ebx + 1CH]
		ret

		#undef CalcImage
		#undef offsetof_TCalcImage_valBox
		#undef sizeof_TCalcValBox
		#undef offsetof_TCalcValBox_edit
	}
}

__declspec(naked) LRESULT CALLBACK TMainForm_NewValProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	/*
		if (uMsg == WM_PASTE)
		{
			NewVal_OnPaste();
			return 0;
		}
		return CallWindowProcA((WNDPROC)TMainForm_PrevNewValProc, hwnd, uMsg, wParam, lParam);
	*/
	__asm
	{
		#define ReturnAddress (esp)
		#define hwnd          (esp + 4)
		#define uMsg          (esp + 8)
		#define wParam        (esp + 12)
		#define lParam        (esp + 16)

		mov     ecx, dword ptr [uMsg]
		mov     eax, dword ptr [ReturnAddress]
		cmp     ecx, WM_PASTE
		je      L1
		mov     ecx, dword ptr [TMainForm_PrevNewValProc]
		push    eax
		mov     dword ptr [esp + 4], ecx
		jmp     CallWindowProcA
	L1:
		mov     dword ptr[esp + 16], eax
		add     esp, 16
		jmp     NewVal_OnPaste

		#undef ReturnAddress
		#undef hwnd
		#undef uMsg
		#undef wParam
		#undef lParam
	}
}

__declspec(naked) void __fastcall TMainForm_HotKeyEditKeyDown(TMainForm *this, void *Sender, WORD *Key, int Shift)
{
	void __cdecl TMainForm_HotKeyEditKeyDown_Header();

	__asm
	{
		mov     eax, dword ptr [esp]
		mov     dword ptr [esp], ecx
		mov     ecx, dword ptr [esp + 4]
		mov     dword ptr [esp + 4], eax
		pop     eax
		jmp     TMainForm_HotKeyEditKeyDown_Header
	}
}

/*
	jmp     TMainForm_HotKeyEditKeyDown_Header      ; 00443054 _ E9, ????????
	nop                                             ; 0044305A _ 90
*/
__declspec(naked) void __cdecl TMainForm_HotKeyEditKeyDown_Header()
{
	extern const DWORD _TWinControl_GetHandle;
	static const DWORD L0044305A = 0x0044305A;

	/*
		if (Shift.Contains(ssShift) || Shift.Contains(ssAlt) || Shift.Contains(ssCtrl)) {
			if (Shift.Contains(ssCtrl))
				if (Key == 'V')
					NewVal_OnPaste();
				else if (Key == 'A')
					SendMessageA(calcImage->valBox[1].edit->Handle, EM_SETSEL, 0, LONG_MAX);
			return;
		}
	*/
	__asm
	{
		#define this                         eax
		#define offsetof_TMainForm_calcImage 1320
		#define offsetof_TCalcImage_valBox   440
		#define sizeof_TCalcValBox           20
		#define offsetof_TCalcValBox_edit    0

		push    eax
		mov     ax, word ptr [esp + 8]
		test    ax, ssShift or ssAlt or ssCtrl
		jnz     L1
		mov     eax, dword ptr [esp]
		sub     esp, 16
		mov     dword ptr [esp + 16], ebp
		lea     ebp, [esp + 16]
		jmp     dword ptr [L0044305A]
	L1:
		test    ax, ssCtrl
		jz      L3
		mov     ax, word ptr [ecx]
		cmp     ax, 'V'
		jne     L2
		pop     ecx
		pop     eax
		mov     dword ptr [esp], eax
		jmp     NewVal_OnPaste
	L2:
		cmp     ax, 'A'
		jne     L3
		pop     eax
		pop     ecx
		mov     dword ptr [esp], LONG_MAX
		mov     eax, dword ptr [this + offsetof_TMainForm_calcImage]
		push    0
		mov     eax, dword ptr [eax + offsetof_TCalcImage_valBox]
		push    EM_SETSEL
		mov     eax, dword ptr [eax + sizeof_TCalcValBox]
		push    ecx
		push    ecx
		call    dword ptr[_TWinControl_GetHandle]
		mov     dword ptr [esp + 4], eax
		jmp     SendMessageA
	L3:
		pop     eax
		ret     4

		#undef this
		#undef offsetof_TMainForm_calcImage
		#undef offsetof_TCalcImage_valBox
		#undef sizeof_TCalcValBox
		#undef offsetof_TCalcValBox_edit
	}
}

static unsigned long __cdecl NewVal_OnPaste()
{
	if (OpenClipboard(NULL))
	{
		HGLOBAL    handle;
		const char *p;

		if ((handle = GetClipboardData(CF_TEXT)) && (p = (const char *)GlobalLock(handle)))
		{
			for (; ; )
			{
				char c;

				switch (c = *(p++))
				{
				case '\0':
					break;
				case '-':
				case '.':
					c += VK_SUBTRACT - '-';
					goto INPUT_VIRTUAL_KEY;
				case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
					c -= 'a' - 'A';
				case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
				case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
				INPUT_VIRTUAL_KEY:
					{
						TMainForm *this;
						WORD      Key;

						this = MainForm;
						Key = (unsigned char)c;
						TMainForm_HotKeyEditKeyDown(this, this, &Key, 0);
					}
				default:
					continue;
				}
				break;
			}
			GlobalUnlock(handle);
		}
		CloseClipboard();
	}
	return 0;
}
