#include <windows.h>
#include "intrinsic.h"
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_vector_string.h"
#include "bcb6_std_map.h"
#include "TSSGCtrl.h"

static BOOL __fastcall TSSGCtrl_GetSSGDataFile_CheckNocacheParam(vector_string *tmpV)
{
	if (vector_size(tmpV) >= 8)
	{
		char *p = string_begin(&vector_at(tmpV, 7));
		while (__intrinsic_isspace(*p))
			p++;
		size_t length = string_end(&vector_at(tmpV, 7)) - p;
		if (length >= 7)
		{
			while (length >= 7 && __intrinsic_isspace(*(p + length - 1)))
				length--;
			if (length == 7 && *(LPDWORD)p == BSWAP32('noca') && *(LPWORD)(p + 4) == BSWAP16('ch') && *(p + 6) == 'e')
				return TRUE;
		}
	}
	return FALSE;
}

static map_iterator __fastcall TSSGCtrl_GetSSGDataFile_findCache(TSSGCtrl *const SSGC, const string *const FName)
{
	map_iterator it = map_find(&SSGC->dataFileMap, FName);
	return it == map_end(&SSGC->dataFileMap) ? NULL : it;
}

EXTERN_C __declspec(naked) void __cdecl Caller_TSSGCtrl_GetSSGDataFile_CheckNocacheParam(string* __x)
{
	__asm
	{
		lea     ecx, [ebp - 0x30]// tmpV
		call    TSSGCtrl_GetSSGDataFile_CheckNocacheParam
		mov     [ebp - 0x0304], eax// ReadSize => IsNocache
		test    eax, eax
		jnz     NO_CACHE

		lea     edx, [ebp + 0x10]// FName
		mov     ecx, [ebp + 0x08]// SSGC
		call    TSSGCtrl_GetSSGDataFile_findCache
		test    eax, eax
		jz      NO_CACHE

		mov     [ebp - 0x02B8], eax// FNameIt
		mov     dword ptr [esp], 0x004EF748
		ret     4// goto FNameIt->second.find(GroupName)

		align   16
	NO_CACHE:
		mov     ecx, [esp + 4]
		jmp     string_ctor
	}
}
