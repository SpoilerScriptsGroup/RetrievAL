#include <windows.h>
#include "intrinsic.h"

#if defined(__BORLANDC__)
#define string          string
#define string_begin(s) (s)->begin()
#define string_end(s)   (s)->end()
#else
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"
#endif

void __fastcall TrimString(string *s)
{
	char *begin, *end;

	begin = string_begin(s);
	while (__intrinsic_isspace(*begin))
		begin++;
	end = string_end(s);
	while (--end > begin && __intrinsic_isspace(*end));
	if (++end != string_end(s) || begin != string_begin(s))
	{
		size_t length;

		length = end - begin;
		if (begin != string_begin(s))
			memcpy(string_begin(s), begin, length);
#if defined(__BORLANDC__)
		s->resize(length);
#else
		*(string_end(s) = string_begin(s) + length) = '\0';
#endif
	}
}

#if defined(__BORLANDC__)
#undef string
#undef string_begin
#undef string_end
#endif
