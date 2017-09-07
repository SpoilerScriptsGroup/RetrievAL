#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_vector_string.h"

static void __fastcall AdjustLogFooter(vector_string *NewLogVec);

__declspec(naked) void __cdecl TSSGCtrl_GetSSGScriptVec_AdjustLogFooter()
{
	__asm
	{
		#define NewLogVec (ebp - 60H)

		lea     ecx, [NewLogVec]
		call    AdjustLogFooter
		pop     ecx
		mov     eax, dword ptr [ebp + 10H]
		push    eax
		mov     eax, dword ptr [eax]
		jmp     ecx

		#undef NewLogVec
	}
}

static void __fastcall AdjustLogFooter(vector_string *NewLogVec)
{
	vector_byte_resize(NewLogVec, vector_byte_size(NewLogVec) + sizeof(string));
	string_ctor(vector_end(NewLogVec) - 1);
}
