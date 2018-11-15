#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "string.h"
#include "TWinControl.h"
#include "TMainForm.h"
#include "TCalcImage.h"

#pragma warning(disable:4733)

#define ssShift  0x0001
#define ssAlt    0x0002
#define ssCtrl   0x0004
#define ssLeft   0x0008
#define ssRight  0x0010
#define ssMiddle 0x0020
#define ssDouble 0x0040

WNDPROC TMainForm_NewValProc = NULL;

static void __fastcall FormClose(TMainForm *this, void *Sender, void *Action);
static unsigned long __cdecl LoadCLD_SuccessReturn(string destructed, TCalcImage *CalcImage, long *STransX, long *STransY, string *SFreeFName, string *SPushFName);
static LRESULT CALLBACK TMainForm_NewValProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static void __fastcall NewVal_OnPaste(TMainForm *this);
void __fastcall HotKeyEditKeyDown_Paste(TMainForm *this, void *Sender, WORD *Key, WORD Shift);

/*
	jmp     TMainForm_FormClose_Header              ; 00402688 _ E9, ????????
	nop                                             ; 0040268D _ 90
	nop                                             ; 0040268E _ 90
	nop                                             ; 0040268F _ 90
	nop                                             ; 00402690 _ 90
*/
void __cdecl TMainForm_FormClose_Header(void *Sender, void *Action)
{
	static const DWORD L00402691 = 00402691;

	__asm
	{
		push    ebp
		push    eax
		push    ecx
		push    edx
		push    ecx
		mov     ecx, eax
		call    FormClose
		pop     edx
		pop     ecx
		mov     ebp, esp
		mov     eax, 4
		jmp     dword ptr [L00402691]
	}
}

static void __fastcall FormClose(TMainForm *this, void *Sender, void *Action)
{
	if (TMainForm_PrevNewValProc)
	{
		SetWindowLongPtrA(TWinControl_GetHandle(vector_at(&this->calcImage->valBox, 1).edit), GWLP_WNDPROC, (LONG_PTR)TMainForm_PrevNewValProc);
		TMainForm_PrevNewValProc = NULL;
	}
}

/*
	call    TMainForm_LoadCLD_Footer                ; 0045622F _ E8, 0019E587
*/
__declspec(naked) unsigned long __fastcall TMainForm_LoadCLD_Footer()
{
	__asm
	{
		mov     esp, ebp
		pop     ebp
		test    eax, eax
		jnz     L1
		jmp     LoadCLD_SuccessReturn
	L1:
		ret
	}
}

static unsigned long __cdecl LoadCLD_SuccessReturn(string destructed, TCalcImage *CalcImage, long *STransX, long *STransY, string *SFreeFName, string *SPushFName)
{
	TMainForm_PrevNewValProc = (WNDPROC)SetWindowLongPtrA(TWinControl_GetHandle(vector_at(&CalcImage->valBox, 1).edit), GWLP_WNDPROC, (LONG_PTR)TMainForm_NewValProc);
	return 0;
}

static LRESULT CALLBACK TMainForm_NewValProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_PASTE)
	{
		NewVal_OnPaste(TMainForm);
		return 0;
	}
	return CallWindowProcA((FARPROC)TMainForm_PrevNewValProc, hwnd, uMsg, wParam, lParam);
}

static void __fastcall NewVal_OnPaste(TMainForm *this)
{
	if (OpenClipboard(NULL))
	{
		HGLOBAL handle;
		char    *p, c;

		if ((handle = ::GetClipboardData(CF_TEXT)) && (p = (char *)GlobalLock(handle)))
		{
			for (; ; )
			{
				switch (c = *(p++))
				{
				case '\0':
					break;
				case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
					c -= 'a' - 'A';
				case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
				case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
					{
						WORD        Key;
						TShiftState Shift;

						Key = (unsigned char)c;
						MainForm->HotKeyEditKeyDown(MainForm, Key, Shift);
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
}

/*
	jmp     TMainForm_HotKeyEditKeyDown_Header      ; 00443054 _ E9, ????????
	nop                                             ; 0044305A _ 90
*/
__declspec(naked) void __cdecl TMainForm_HotKeyEditKeyDown_Header()
{
	static const DWORD address = 0x0044305A;

	__asm
	{
		test    ecx, ssShift or ssAlt or ssCtrl
		jnz     L1
		push    ebp
		mov     ebp, esp
		sub     esp, 16
		jmp     dword ptr [address]
	L1:
		mov     dword ptr [esp - 8], eax
		mov     eax, dword ptr [esp]
		mov     dword ptr [esp], ecx
		push    eax
		mov     ecx, dword ptr [esp - 4]
		jmp     HotKeyEditKeyDown_KeyPaste
	}
}

void __fastcall HotKeyEditKeyDown_Paste(TMainForm *this, void *Sender, WORD *Key, WORD Shift)
{
	if ((Shift & ssCtrl) && *Key == 'V')
		NewVal_OnPaste(this);
}
