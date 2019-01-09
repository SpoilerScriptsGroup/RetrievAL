#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"
#include "TSSGCtrl.h"
#include "TProcessCtrl.h"

EXTERN_C BOOLEAN __stdcall TSSGCtrl_AddressNaming_OneRead(string *NameStr, HANDLE SHandle, DWORD Address, char *tmpC, DWORD DataSize)
{
	do	/* do { ... } while (0); */
	{
		const char *p;

		p = string_c_str(NameStr);
		while (__intrinsic_isspace(*p))
			p++;
		if (*p == '_')
		{
			p++;
			while (__intrinsic_isspace(*p))
				p++;
			if (p[0] == 'f' && p[1] == 'm' && p[2] == 't')
			{
				p += 3;
				while (__intrinsic_isspace(*p))
					p++;
				if (*p == ',')
					break;
			}
		}
		return Address && TProcessCtrl_OneRead(SHandle, Address, tmpC, DataSize);
	} while (0);
	switch (DataSize)
	{
	case 4:
		*(LPDWORD)tmpC = Address;
		break;
	case 3:
		*(LPDWORD)tmpC = Address & 0x00FFFFFF;
		break;
	case 2:
		*(LPWORD)tmpC = (WORD)Address;
		break;
	case 1:
		*(LPBYTE)tmpC = (BYTE)Address;
		break;
	default:
		return FALSE;
	}
	return TRUE;
}
