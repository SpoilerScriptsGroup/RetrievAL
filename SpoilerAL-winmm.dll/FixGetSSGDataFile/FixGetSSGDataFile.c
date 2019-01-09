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

	begin = string_begin(FileName);
	while (__intrinsic_isspace_without_return(*begin))
		begin++;
	end = string_end(FileName) - 1;
	while (end > begin && __intrinsic_isspace_without_return(*end))
		end--;
	if (++end == string_end(FileName) && begin == string_begin(FileName))
		return;
	length = end - begin;
	if (begin != string_begin(FileName))
		memcpy(string_begin(FileName), begin, length);
	*(string_end(FileName) = string_begin(FileName) + length) = '\0';
}
