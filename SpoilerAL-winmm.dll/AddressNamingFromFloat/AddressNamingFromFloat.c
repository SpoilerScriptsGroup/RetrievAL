#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_vector_string.h"

const char* __fastcall AddressNamingFromFloat(
	vector_string const *const tmpV,
	double              *const _Dst,
	double              *const Val,
	double        const *const tmpC,
	unsigned long        const DataSize)
{
	switch (DataSize)
	{
	case 4:
		*_Dst = *(const float *)tmpC;
		break;
	case 8:
		*_Dst = *tmpC;
		break;
	default:
		return NULL;
	}
	return string_c_str(&vector_at(tmpV, 5));
}
