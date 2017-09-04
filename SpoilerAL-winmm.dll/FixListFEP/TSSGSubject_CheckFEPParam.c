#include <windows.h>
#include "intrinsic.h"
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"
#include "TSSGSubject.h"

void __fastcall TSSGSubject_CheckFEPParam(TSSGSubject *this, const string *s);

__declspec(naked) void __cdecl TSSBitList_Setting_CheckFEPParam()
{
	__asm
	{
		#define sizeof_string 24
		#define this          ebx
		#define tmpV          (ebp - 18H)

		dec     dword ptr [esi + 1CH]
		dec     dword ptr [esi + 1CH]
		mov     edx, dword ptr [tmpV]
		mov     eax, dword ptr [tmpV + 4]
		add     edx, sizeof_string * 3
		mov     ecx, this
		cmp     edx, eax
		jae     L1
		jmp     TSSGSubject_CheckFEPParam
	L1:
		ret

		#undef sizeof_string
		#undef this
		#undef tmpV
	}
}

__declspec(naked) void __cdecl TSSBundleList_Setting_CheckFEPParam()
{
	__asm
	{
		#define sizeof_string 24
		#define this          ebx
		#define tmpV          edi

		dec     dword ptr [esi + 1CH]
		dec     dword ptr [esi + 1CH]
		mov     edx, dword ptr [tmpV]
		mov     eax, dword ptr [tmpV + 4]
		add     edx, sizeof_string * 6
		mov     ecx, this
		cmp     edx, eax
		jae     L1
		jmp     TSSGSubject_CheckFEPParam
	L1:
		ret

		#undef sizeof_string
		#undef this
		#undef tmpV
	}
}

__declspec(naked) void __cdecl TSSDoubleList_Setting_CheckFEPParam()
{
	__asm
	{
		#define sizeof_string 24
		#define this          ebx
		#define tmpV          (ebp - 18H)

		dec     dword ptr [esi + 1CH]
		dec     dword ptr [esi + 1CH]
		mov     edx, dword ptr [tmpV]
		mov     eax, dword ptr [tmpV + 4]
		add     edx, sizeof_string * 6
		mov     ecx, this
		cmp     edx, eax
		jae     L1
		jmp     TSSGSubject_CheckFEPParam
	L1 :
		ret

		#undef sizeof_string
		#undef this
		#undef tmpV
	}
}

__declspec(naked) void __cdecl TSSList_Setting_CheckFEPParam()
{
	__asm
	{
		#define sizeof_string 24
		#define this          ebx
		#define tmpV          edi

		dec     dword ptr [esi + 1CH]
		dec     dword ptr [esi + 1CH]
		mov     edx, dword ptr [tmpV]
		mov     eax, dword ptr [tmpV + 4]
		add     edx, sizeof_string * 4
		mov     ecx, this
		cmp     edx, eax
		jae     L1
		jmp     TSSGSubject_CheckFEPParam
	L1:
		ret

		#undef sizeof_string
		#undef this
		#undef tmpV
	}
}

void __fastcall TSSGSubject_CheckFEPParam(TSSGSubject *this, const string *s)
{
	if (!string_empty(s))
	{
		const char *p = s->_M_start;
		while (__intrinsic_isspace(*p))
			p++;
		size_t length = s->_M_finish - p;
		if (length >= 3)
		{
			while (__intrinsic_isspace(*(p + length - 1)) && --length >= 3);
			if (length == 3 && p[0] == 'f' && p[1] == 'e' && p[2] == 'p')
				this->isFEP = TRUE;
		}
	}
}
