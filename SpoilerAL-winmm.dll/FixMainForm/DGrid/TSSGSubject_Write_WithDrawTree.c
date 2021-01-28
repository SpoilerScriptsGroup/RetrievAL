#include "TMainForm.h"
#include "ShowErrorMessage/ErrorMessage.h"

#define SHOW_ERROR_MESSAGE 1

__declspec(naked) unsigned long __stdcall TSSGSubject_Write_WithDrawTree(TSSGSubject *this, TSSGCtrl *SSGC, TSSArg *Arg)
{
	static const char szEmpty[] = "";
	__asm
	{
		#define _NULL 0

#if SHOW_ERROR_MESSAGE
		mov     dword ptr [dwErrorMessageId], 0
#endif
		mov     eax, ds:[_MainForm]
		cmp     [eax]TMainForm.userMode, 4
		jne     WRITE
		push    GUIDE_IS_CLEAR | GUIDE_IS_NOT_LINE
		push    offset szEmpty
		call    TMainForm_Guide
	WRITE:
		mov     edx, [esp]
		push    dword ptr [esp + 0x0C]
		push    dword ptr [esp + 0x0C]
		mov     ecx,      [esp + 0x0C]
		push    ecx
		mov     dword ptr [esp + 24], edx
		mov     eax, [ecx]TSSGSubject.VTable
		call    [eax]SubjectVtbl.Write
		mov     ecx, dword ptr ds:[_MainForm]
		mov     dword ptr [esp + 20], eax
		mov     dword ptr [esp + 16], TRUE
#if DRAWTREE_CONVENTION
		mov     dword ptr [esp + 12], 0
#else
		mov     dword ptr [esp + 12], LONG_MIN
#endif
		mov     dword ptr [esp +  8], 0
		mov     dword ptr [esp +  4], _NULL
		mov     dword ptr [esp     ], ecx
		call    TMainForm_DrawTree
#if !DRAWTREE_CONVENTION
		add     esp, 20
#endif
		pop     eax
		ret

		#undef _NULL
	}
}
