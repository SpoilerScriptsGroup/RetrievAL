#include <windows.h>
#include <dhcpsapi.h>
#include "intrinsic.h"
#define USING_NAMESPACE_BCB6_STD
#include "TSSGSubject.h"

DWORD_DWORD __fastcall TSSGCtrl_ReadSSRFile_CheckSignedParam(
	vector_string const *const tmpV,
	TSSGSubject   const *const SSGS,
	string              *const __x,
	char          const *const __s,
	void          const *const __a)
{
	string const *const psz4 = &vector_at(tmpV, 4);
	string_clear(__x);// "_" => ""
	return (DWORD_DWORD)
	{
		vector_size(tmpV), psz4 >= vector_end(tmpV)
			|| string_size(psz4) != 6
			|| *(LPDWORD)string_begin(psz4) != BSWAP32('sign')
			|| *(LPWORD)&string_at(psz4, 4) != BSWAP16('ed')
	};// TRUE means unsigned because optimizing for JECXZ
}

