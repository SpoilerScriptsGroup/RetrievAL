#include <windows.h>
#include "bcb6_std_vector.h"
#include "bcb6_std_string.h"
#include "TSSGCtrl.h"

EXTERN_C void __stdcall ReplaceDefineDynamic(TSSGSubject *SSGS, bcb6_std_string *line);

EXTERN_C void __stdcall AddressNamingFEPList(TSSGCtrl *SSGCtrl, TSSGSubject *SSGS, bcb6_std_vector *tmpV, unsigned long DataSize, char *tmpC)
{
	bcb6_std_string_clear((bcb6_std_string *)tmpV->_M_start + 3);
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
			bcb6_std_string *src;
			char            *endptr;
			unsigned long   value;

			index =
				DataSize == 4 ? *(LPDWORD)tmpC :
				DataSize == 3 ? *(LPDWORD)tmpC & 0x00FFFFFF :
				DataSize == 2 ? *(LPWORD )tmpC :
				                *(LPBYTE )tmpC;
			index = TSSGCtrl_CheckIO_FEP(SSGCtrl, SSGS, index, FALSE);
			src = (bcb6_std_string *)tmpV->_M_start + 6;
			if (!bcb6_std_string_empty(src))
			{
				value = strtoul(src->_M_start, &endptr, 0);
				if (!*endptr)
					index -= value;
			}
			src = (bcb6_std_string *)tmpV->_M_start + 7;
			if (!bcb6_std_string_empty(src))
			{
				value = strtoul(src->_M_start, &endptr, 0);
				if (value && !*endptr)
					index /= value;
			}
			if (index < bcb6_std_vector_size(vec, bcb6_std_string))
			{
				bcb6_std_string_assign((bcb6_std_string *)tmpV->_M_start + 4, (bcb6_std_string *)vec->_M_start + index);
				ReplaceDefineDynamic(SSGS, (bcb6_std_string *)tmpV->_M_start + 4);
			}
		}
	}
	*tmpC = '\0';
}
