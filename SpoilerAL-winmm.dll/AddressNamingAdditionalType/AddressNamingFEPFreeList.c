#include <windows.h>
#include "bcb6_std_vector.h"
#include "bcb6_std_string.h"
#include "TSSGCtrl.h"
#include "TStringDivision.h"

EXTERN_C void __stdcall ReplaceDefine(TSSGAttributeSelector *attributeSelector, bcb6_std_string *line);

EXTERN_C void __stdcall AddressNamingFEPFreeList(TSSGCtrl *SSGCtrl, TSSGSubject *SSGS, bcb6_std_vector *tmpV, unsigned long DataSize, char *tmpC)
{
	if (DataSize <= 4 && !bcb6_std_string_empty((bcb6_std_string *)tmpV->_M_start + 5))
	{
		bcb6_std_vector *vec;
		bcb6_std_string FName;
		bcb6_std_string DefaultExt;

		bcb6_std_string_ctor_assign(&FName, (bcb6_std_string *)tmpV->_M_start + 5);
		bcb6_std_string_ctor_assign_cstr_with_length(&DefaultExt, ".LST", 4);
		vec = TSSGCtrl_GetSSGDataFile(SSGCtrl, SSGS, FName, DefaultExt, NULL);
		if (vec)
		{
			unsigned long   index;
			bcb6_std_string *it;

			index =
				DataSize == 4 ? *(LPDWORD)tmpC :
				DataSize == 3 ? *(LPDWORD)tmpC & 0x00FFFFFF :
				DataSize == 2 ? *(LPWORD )tmpC :
				                *(LPBYTE )tmpC;
			index = TSSGCtrl_CheckIO_FEP(SSGCtrl, SSGS, index, FALSE);
			for (it = (bcb6_std_string *)vec->_M_start; it != (bcb6_std_string *)vec->_M_finish; it++)
			{
				bcb6_std_string Token;
				bcb6_std_string tmpS;
				char            *endptr;
				unsigned long   value;
				char            ch;

				bcb6_std_string_assign((bcb6_std_string *)tmpV->_M_start + 3, it);
				ReplaceDefine(&SSGCtrl->attributeSelector, (bcb6_std_string *)tmpV->_M_start + 3);
				bcb6_std_string_ctor_assign_cstr_with_length(&Token, "=", 1);
				TStringDivision_Half(
					&tmpS,
					&SSGCtrl->strD,
					(bcb6_std_string *)tmpV->_M_start + 3,
					Token,
					0,
					0);
				if (!bcb6_std_string_empty(&tmpS))
				{
					value = strtoul(tmpS._M_start, &endptr, 0);
					ch = *endptr;
				}
				else
				{
					ch = UCHAR_MAX;
				}
				bcb6_std_string_dtor(&tmpS);
				if (ch || index != value)
					continue;
				bcb6_std_string_assign((bcb6_std_string *)tmpV->_M_start + 4, (bcb6_std_string *)tmpV->_M_start + 3);
				break;
			}
		}
	}
	bcb6_std_string_clear((bcb6_std_string *)tmpV->_M_start + 3);
	*tmpC = '\0';
}
