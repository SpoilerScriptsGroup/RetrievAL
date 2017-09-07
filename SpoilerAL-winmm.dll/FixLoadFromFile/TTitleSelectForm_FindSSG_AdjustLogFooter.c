#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_vector_string.h"

static void __fastcall AdjustLogFooter(vector_string *LogFile);

__declspec(naked) void __cdecl TTitleSelectForm_FindSSG_AdjustLogFooter()
{
	__asm
	{
		#define LogFile (ebp - 38H)

		dec     dword ptr [ebx + 1CH]
		dec     dword ptr [ebx + 1CH]
		lea     ecx, [LogFile]
		jmp     AdjustLogFooter

		#undef LogFile
	}
}

static void __fastcall AdjustLogFooter(vector_string *LogFile)
{
	size_t footerBlank = 2;
	if (!vector_empty(LogFile))
	{
		string *it = vector_end(LogFile) - 1;
		if (string_empty(it))
		{
			footerBlank--;
			if (it-- != vector_begin(LogFile) && string_empty(it))
				footerBlank--;
		}
	}
	if (footerBlank)
		vector_string_resize(LogFile, vector_size(LogFile) + footerBlank);
}
