#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_vector_string.h"
#include "TSSGCtrl.h"

EXTERN_C void __stdcall ReplaceDefineDynamic(TSSGSubject *SSGS, string *line);

EXTERN_C void __stdcall AddressNamingFEPList(TSSGCtrl *SSGCtrl, TSSGSubject *SSGS, vector *tmpV, unsigned long DataSize, char *tmpC)
{
	string_clear((string *)tmpV->_M_start + 3);
	if (DataSize <= 4 && !string_empty((string *)tmpV->_M_start + 5))
	{
		vector_string *vec;
		string        FName;
		string        DefaultExt;

		string_ctor_assign(&FName, (string *)tmpV->_M_start + 5);
		string_ctor_assign_cstr_with_length(&DefaultExt, ".LST", 4);
		vec = TSSGCtrl_GetSSGDataFile(SSGCtrl, SSGS, FName, DefaultExt, NULL);
		if (vec)
		{
			unsigned long   index;
			string *src;
			char            *endptr;
			unsigned long   value;

			index =
				DataSize == 4 ? *(LPDWORD)tmpC :
				DataSize == 3 ? *(LPDWORD)tmpC & 0x00FFFFFF :
				DataSize == 2 ? *(LPWORD )tmpC :
				                *(LPBYTE )tmpC;
			index = TSSGCtrl_CheckIO_FEP(SSGCtrl, SSGS, index, FALSE);
			src = (string *)tmpV->_M_start + 6;
			if (!string_empty(src))
			{
				value = strtoul(src->_M_start, &endptr, 0);
				if (!*endptr)
					index -= value;
			}
			src = (string *)tmpV->_M_start + 7;
			if (!string_empty(src))
			{
				value = strtoul(src->_M_start, &endptr, 0);
				if (value && !*endptr)
					index /= value;
			}
			if (index < vector_size(vec))
			{
				string_assign((string *)tmpV->_M_start + 4, (string *)vec->_M_start + index);
				ReplaceDefineDynamic(SSGS, (string *)tmpV->_M_start + 4);
			}
		}
	}
	*tmpC = '\0';
}
