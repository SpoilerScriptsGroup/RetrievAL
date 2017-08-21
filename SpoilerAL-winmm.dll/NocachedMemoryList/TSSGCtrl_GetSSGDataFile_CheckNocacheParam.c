#include <windows.h>
#include "intrinsic.h"
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_vector_string.h"

static BOOL __fastcall TSSGCtrl_GetSSGDataFile_CheckNocacheParam(vector_string *tmpV);

EXTERN_C __declspec(naked) void __cdecl Caller_TSSGCtrl_GetSSGDataFile_CheckNocacheParam()
{
	__asm
	{
		lea     ecx, [ebp - 30H]
		call    TSSGCtrl_GetSSGDataFile_CheckNocacheParam
		mov     dword ptr [esp + 8], eax
		mov     ecx, dword ptr [esp + 4]
		jmp     string_ctor
	}
}

static BOOL __fastcall TSSGCtrl_GetSSGDataFile_CheckNocacheParam(vector_string *tmpV)
{
	if (vector_size(tmpV) >= 8)
	{
		char *p = ((string *)tmpV->_M_start + 7)->_M_start;
		while (__intrinsic_isspace(*p))
			p++;
		size_t length = ((string *)tmpV->_M_start + 7)->_M_finish - p;
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
