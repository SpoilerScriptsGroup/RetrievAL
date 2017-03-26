#include <windows.h>
#include "intrinsic.h"

#if defined(__BORLANDC__)
#define bcb6_std_string          string
#define bcb6_std_string_begin(s) (s)->begin()
#define bcb6_std_string_end(s)   (s)->end()
#else
#include "bcb6_std_string.h"
#endif

void __fastcall TrimString(bcb6_std_string *s)
{
	char *begin, *end;

	begin = bcb6_std_string_begin(s);
	while (__intrinsic_isspace(*begin))
		begin++;
	end = bcb6_std_string_end(s);
	while (--end > begin && __intrinsic_isspace(*end));
	if (++end != bcb6_std_string_end(s) || begin != bcb6_std_string_begin(s))
	{
		size_t length;

		length = end - begin;
		if (begin != bcb6_std_string_begin(s))
			memcpy(bcb6_std_string_begin(s), begin, length);
#if defined(__BORLANDC__)
		s->resize(length);
#else
		*(bcb6_std_string_end(s) = bcb6_std_string_begin(s) + length) = '\0';
#endif
	}
}

#if defined(__BORLANDC__)
#undef bcb6_std_string
#undef bcb6_std_string_begin
#undef bcb6_std_string_end
#endif
