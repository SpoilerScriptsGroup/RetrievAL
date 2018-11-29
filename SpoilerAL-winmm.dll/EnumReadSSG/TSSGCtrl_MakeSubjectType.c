#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"
#include "TSSGSubject.h"

unsigned long __cdecl TSSGCtrl_MakeSubjectType(const string *Tag)
{
	size_t     length;
	const char *p;

	if ((length = (string_end(Tag) - (p = string_c_str(Tag)))) >= 3)
		switch (*(LPDWORD)p)
		{
		case BSWAP32('calc'):	// calc
			if (length != 4)
				break;
			return stCALC;
		case BSWAP32('togg'):	// toggle
			if (length != 6 || *(LPWORD)(p + 4) != BSWAP16('le'))
				break;
			return stTOGGLE;
		case BSWAP32('list'):	// list
			if (length != 4)
				break;
			return stLIST;
		case BSWAP32('dir\0'):	// dir
			return stDIR;
		case BSWAP32('b_ca'):	// b_calc
			if (length != 6 || *(LPWORD)(p + 4) != BSWAP16('lc'))
				break;
			return stB_CALC;
		case BSWAP32('b_to'):	// b_toggle
			if (length != 8 || *(LPDWORD)(p + 4) != BSWAP32('ggle'))
				break;
			return stB_TOGGLE;
		case BSWAP32('d_li'):	// d_list
			if (length != 6 || *(LPWORD)(p + 4) != BSWAP16('st'))
				break;
			return stD_LIST;
		case BSWAP32('trac'):	// trace
			if (length != 5 || p[4] != 'e')
				break;
			return stTRACE;
		case BSWAP32('stri'):	// string
			if (length != 6 || *(LPWORD)(p + 4) != BSWAP16('ng'))
				break;
			return stSTRING;
		case BSWAP32('b_li'):	// b_list
			if (length != 6 || *(LPWORD)(p + 4) != BSWAP16('st'))
				break;
			return stB_LIST;
		case BSWAP32('d_to'):	// d_toggle
			if (length != 8 || *(LPDWORD)(p + 4) != BSWAP32('ggle'))
				break;
			return stD_TOGGLE;
		case BSWAP32('bitl'):	// bitlist
			if (length != 7 || *(LPDWORD)(p + 4) != BSWAP32('ist\0'))
				break;
			return stBITLIST;
		case BSWAP32('copy'):	// copy
			if (length != 4)
				break;
			return stCOPY;
		case BSWAP32('f_ca'):	// f_calc
			if (length != 6 || *(LPWORD)(p + 4) != BSWAP16('lc'))
				break;
			return stFLOAT_CALC;
		case BSWAP32('bf_c'):	// bf_calc
			if (length != 7 || *(LPDWORD)(p + 4) != BSWAP32('alc\0'))
				break;
			return stB_FLOAT_CALC;
		case BSWAP32('spli'):	// split
			if (length != 5 || p[4] != 't')
				break;
			return stSPLIT;
		}
	return 0;
}
