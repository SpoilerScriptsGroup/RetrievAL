#include <windows.h>
#include "TSSGSubject.h"
#include "SubjectProperty\SSGSubjectProperty.h"

extern DWORD RepeatDepth;

void __fastcall SetSubjectProperty(TSSGSubject *SSGS, DWORD RepeatIndex, DWORD ParentRepeat);

__declspec(naked) void __cdecl TSSGCtrl_EnumReadSSG_SetSubjectProperty1()
{
	__asm
	{
		#define SSGS         (ebp - 7C8H)
		#define RepeatIndex  (ebp + 18H)
		#define ParentRepeat (ebp + 1CH)

		mov     eax, dword ptr [ParentRepeat]
		mov     edx, dword ptr [RepeatIndex]
		mov     ecx, dword ptr [SSGS]
		push    eax
		push    004EB508H
		jmp     SetSubjectProperty

		#undef SSGS
		#undef RepeatIndex
		#undef ParentRepeat
	}
}

__declspec(naked) void __cdecl TSSGCtrl_EnumReadSSG_SetSubjectProperty2()
{
	__asm
	{
		#define SSGS         (ebp - 0EF4H)
		#define RepeatIndex  (ebp + 18H)
		#define ParentRepeat (ebp + 1CH)

		mov     eax, dword ptr [ParentRepeat]
		mov     edx, dword ptr [RepeatIndex]
		mov     ecx, dword ptr [SSGS]
		push    eax
		push    004EB508H
		jmp     SetSubjectProperty

		#undef SSGS
		#undef RepeatIndex
		#undef ParentRepeat
	}
}

static void __fastcall SetSubjectProperty(TSSGSubject *SSGS, DWORD RepeatIndex, DWORD ParentRepeat)
{
	TSSGSubjectProperty *prop = GetSubjectProperty(SSGS);
	if (prop)
	{
		prop->RepeatDepth  = RepeatDepth;
		prop->RepeatIndex  = RepeatIndex;
		prop->ParentRepeat = ParentRepeat;
	}
}
