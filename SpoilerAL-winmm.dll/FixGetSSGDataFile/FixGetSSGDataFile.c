#include "bcb6_std_string.h"
#include "intrinsic.h"

void __stdcall FixGetSSGDataFile(bcb6_std_string *FileName)
{
	char   *begin, *end;
	size_t length;

	// it do not checking multibyte,
	// because space is not the lead and tail byte of codepage 932.

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
