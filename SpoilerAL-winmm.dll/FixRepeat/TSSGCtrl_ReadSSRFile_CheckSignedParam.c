#include <windows.h>
#include "intrinsic.h"
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_vector_string.h"
#include "TStringDivision.h"

static BOOL __stdcall CheckSignedParam(
	TStringDivision *StrD,
	const string    *Src,
	const string    Token,// destructed
	vector_string   *tmpV,
	unsigned long   Option)
{
	const string *const psz4 = &vector_at(tmpV, 4);
	return vector_size(tmpV) <= 4
		|| string_size(psz4) != 6
		|| *(LPDWORD)string_begin(psz4) != BSWAP32('sign')
		|| *(LPWORD)&string_at(psz4, 4) != BSWAP16('ed')
		;// TRUE means unsigned because optimizing for JECXZ
}

__declspec(naked) BOOL __cdecl TSSGCtrl_ReadSSRFile_CheckSignedParam()
{
	__asm
	{
		dec     dword ptr [ebx + 0x1C]
		jmp     CheckSignedParam
	}
}
