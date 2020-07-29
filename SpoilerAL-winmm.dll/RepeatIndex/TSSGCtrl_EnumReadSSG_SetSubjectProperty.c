#include <windows.h>
#include "TSSGSubject.h"
#include "SubjectProperty\SSGSubjectProperty.h"

extern DWORD RepeatDepth;

void __fastcall SetSubjectProperty(TSSGSubject *SSGS, DWORD RepeatIndex, DWORD OuterRepeat);

__declspec(naked) void __cdecl TSSGCtrl_EnumReadSSG_SetSubjectProperty1()
{
	__asm
	{
		#define SSGS         (ebp - 7C8H)
		#define RepeatIndex  (ebp + 18H)
		#define OuterRepeat  (ebp + 1CH)

		mov     eax, dword ptr [OuterRepeat]
		mov     edx, dword ptr [RepeatIndex]
		mov     ecx, dword ptr [SSGS]
		push    eax
		push    004EB508H
		jmp     SetSubjectProperty

		#undef SSGS
		#undef RepeatIndex
		#undef OuterRepeat
	}
}

__declspec(naked) void __cdecl TSSGCtrl_EnumReadSSG_SetSubjectProperty2()
{
	__asm
	{
		#define SSGS         (ebp - 0EF4H)
		#define RepeatIndex  (ebp + 18H)
		#define OuterRepeat  (ebp + 1CH)

		mov     eax, dword ptr [OuterRepeat]
		mov     edx, dword ptr [RepeatIndex]
		mov     ecx, dword ptr [SSGS]
		push    eax
		push    004EB508H
		jmp     SetSubjectProperty

		#undef SSGS
		#undef RepeatIndex
		#undef OuterRepeat
	}
}

static void __fastcall SetSubjectProperty(TSSGSubject *SSGS, DWORD RepeatIndex, DWORD OuterRepeat)
{
	TSSGSubjectProperty *prop = GetSubjectProperty(SSGS);
	if (prop)
	{
		prop->RepeatDepth  = RepeatDepth;
		prop->RepeatIndex  = RepeatIndex;
		prop->OuterRepeat  = OuterRepeat;
	}
}
