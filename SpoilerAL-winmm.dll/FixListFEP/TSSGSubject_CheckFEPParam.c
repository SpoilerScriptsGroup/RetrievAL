#include <windows.h>
#include "intrinsic.h"
#include "bcb6_std_string.h"
#include "TSSGSubject.h"

void __fastcall TSSGSubject_CheckFEPParam(TSSGSubject *_this, const bcb6_std_string *s);

__declspec(naked) void __cdecl TSSBitList_Setting_CheckFEPParam()
{
	__asm
	{
		#define sizeof_string 24
		#define _this         ebx
		#define tmpV          (ebp - 18H)

		dec     dword ptr [esi + 1CH]
		dec     dword ptr [esi + 1CH]
		mov     edx, dword ptr [tmpV]
		mov     eax, dword ptr [tmpV + 4]
		add     edx, sizeof_string * 3
		mov     ecx, _this
		cmp     edx, eax
		jae     L1
		jmp     TSSGSubject_CheckFEPParam
	L1:
		ret

		#undef sizeof_string
		#undef _this
		#undef tmpV
	}
}

__declspec(naked) void __cdecl TSSBundleList_Setting_CheckFEPParam()
{
	__asm
	{
		#define sizeof_string 24
		#define _this         ebx
		#define tmpV          edi

		dec     dword ptr [esi + 1CH]
		dec     dword ptr [esi + 1CH]
		mov     edx, dword ptr [tmpV]
		mov     eax, dword ptr [tmpV + 4]
		add     edx, sizeof_string * 6
		mov     ecx, _this
		cmp     edx, eax
		jae     L1
		jmp     TSSGSubject_CheckFEPParam
	L1:
		ret

		#undef sizeof_string
		#undef _this
		#undef tmpV
	}
}

__declspec(naked) void __cdecl TSSDoubleList_Setting_CheckFEPParam()
{
	__asm
	{
		#define sizeof_string 24
		#define _this         ebx
		#define tmpV          (ebp - 18H)

		dec     dword ptr [esi + 1CH]
		dec     dword ptr [esi + 1CH]
		mov     edx, dword ptr [tmpV]
		mov     eax, dword ptr [tmpV + 4]
		add     edx, sizeof_string * 6
		mov     ecx, _this
		cmp     edx, eax
		jae     L1
		jmp     TSSGSubject_CheckFEPParam
	L1 :
		ret

		#undef sizeof_string
		#undef _this
		#undef tmpV
	}
}

__declspec(naked) void __cdecl TSSList_Setting_CheckFEPParam()
{
	__asm
	{
		#define sizeof_string 24
		#define _this         ebx
		#define tmpV          edi

		dec     dword ptr [esi + 1CH]
		dec     dword ptr [esi + 1CH]
		mov     edx, dword ptr [tmpV]
		mov     eax, dword ptr [tmpV + 4]
		add     edx, sizeof_string * 4
		mov     ecx, _this
		cmp     edx, eax
		jae     L1
		jmp     TSSGSubject_CheckFEPParam
	L1:
		ret

		#undef sizeof_string
		#undef _this
		#undef tmpV
	}
}

void __fastcall TSSGSubject_CheckFEPParam(TSSGSubject *_this, const bcb6_std_string *s)
{
	if (!bcb6_std_string_empty(s))
	{
		const char *p = s->_M_start;
		while (__intrinsic_isspace(*p))
			p++;
		size_t length = s->_M_finish - p;
		if (length >= 3)
		{
			while (__intrinsic_isspace(*(p + length - 1)) && --length >= 3);
			if (length == 3 && p[0] == 'f' && p[1] == 'e' && p[2] == 'p')
				_this->isFEP = TRUE;
		}
	}
}
