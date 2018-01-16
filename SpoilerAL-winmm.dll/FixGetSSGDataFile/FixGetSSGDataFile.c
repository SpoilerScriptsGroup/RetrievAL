#include "intrinsic.h"
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"

static void __fastcall InternalFixGetSSGDataFile(string *FileName);

__declspec(naked) void __cdecl FixGetSSGDataFile()
{
	__asm
	{
		#define NextCallAddress 005D5258H
		#define FileName        (ebp - 1A8H)

		push    NextCallAddress
		lea     ecx, [FileName]
		jmp     InternalFixGetSSGDataFile

		#undef NextCallAddress
		#undef FileName
	}
}

static void __fastcall InternalFixGetSSGDataFile(string *FileName)
{
	char   *begin, *end;
	size_t length;

	// it do not checking multibyte,
	// because space is not the lead and trail byte of codepage 932.

	begin = FileName->_M_start;
	while (__intrinsic_isspace_without_return(*begin))
		begin++;
	end = FileName->_M_finish - 1;
	while (end > begin && __intrinsic_isspace_without_return(*end))
		end--;
	if (++end == FileName->_M_finish && begin == FileName->_M_start)
		return;
	length = end - begin;
	if (begin != FileName->_M_start)
		memcpy(FileName->_M_start, begin, length);
	*(FileName->_M_finish = FileName->_M_start + length) = '\0';
}
