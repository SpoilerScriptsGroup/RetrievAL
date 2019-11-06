#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "TWinControl.h"
#include "TMainForm.h"
#undef MainForm
#include "verbose.h"
#include "PageSize.h"

extern void __cdecl ClearGuideBuffer();
extern void __cdecl DeleteWaitCursor();
extern void __cdecl DeleteProcessMonitor();
extern void __cdecl SubjectStringTable_dtor();

extern HANDLE  hHeap;
extern WNDPROC TMainForm_PrevWindowProc;
extern WNDPROC TMainForm_PrevDGridProc;
extern string  ProcessAttachCode;
extern string  ProcessDetachCode;
extern LPBYTE  lpReadOnlyBuffer;
extern size_t  nSizeOfReadOnlyBuffer;

static void __fastcall dtor(TMainForm *this);

__declspec(naked) void __cdecl TMainForm_dtor()
{
	__asm
	{
		mov     ecx, dword ptr ds:[_MainForm]
		call    dtor
		mov     eax, 0045FDF2H
		dec     dword ptr [ebx + 1CH]
		lea     edx, [esi + 0C10H]
		jmp     eax
	}
}

static void __fastcall dtor(TMainForm *this)
{
	verbose(VRB_INFO, "TMainForm::dtor - begin");

	ClearGuideBuffer();
	DeleteWaitCursor();
	SetWindowLongPtrA(TWinControl_GetHandle(this->DGrid), GWLP_WNDPROC, (LONG_PTR)TMainForm_PrevDGridProc);
	SetWindowLongPtrA(TWinControl_GetHandle(this), GWLP_WNDPROC, (LONG_PTR)TMainForm_PrevWindowProc);
	DeleteProcessMonitor();
	SubjectStringTable_dtor();
	string_dtor(&ProcessAttachCode);
	string_dtor(&ProcessDetachCode);
	if (lpReadOnlyBuffer)
	{
		HeapFree(hHeap, 0, lpReadOnlyBuffer);
		lpReadOnlyBuffer = NULL;
		nSizeOfReadOnlyBuffer = 0;
	}

	verbose(VRB_INFO, "TMainForm::dtor - end");
}
