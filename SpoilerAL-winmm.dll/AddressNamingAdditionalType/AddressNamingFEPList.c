#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_vector_string.h"
#include "TSSGCtrl.h"

EXTERN_C void __stdcall ReplaceDefineDynamic(TSSGSubject *SSGS, string *line);

EXTERN_C void __fastcall AddressNamingFEPList(
	unsigned long  const DataSize,
	char          *const tmpC,
	vector_string *const tmpV,
	TSSGCtrl      *const SSGCtrl,
	TSSGSubject   *const SSGS)
{
	string_clear(&vector_at(tmpV, 3));
	if (DataSize <= 4 && !string_empty(&vector_at(tmpV, 5)))
	{
		vector_string *vec;
		string        FName;
		string        DefaultExt;

		string_ctor_assign(&FName, &vector_at(tmpV, 5));
		string_ctor_assign_cstr_with_length(&DefaultExt, ".LST", 4);
		vec = TSSGCtrl_GetSSGDataFile(SSGCtrl, SSGS, FName, DefaultExt, NULL);
		if (vec)
		{
			unsigned long index;
			string        *src;
			char          *endptr;
			unsigned long value;

			index =
				DataSize == 4 ? *(LPDWORD)tmpC :
				DataSize == 3 ? *(LPDWORD)tmpC & 0x00FFFFFF :
				DataSize == 2 ? *(LPWORD )tmpC :
				                *(LPBYTE )tmpC;
			index = TSSGCtrl_CheckIO_FEP(SSGCtrl, SSGS, index, FALSE);
			src = &vector_at(tmpV, 6);
			if (!string_empty(src))
			{
				value = strtoul(string_c_str(src), &endptr, 0);
				if (!*endptr)
					index -= value;
			}
			src = &vector_at(tmpV, 7);
			if (!string_empty(src))
			{
				value = strtoul(string_c_str(src), &endptr, 0);
				if (value && !*endptr)
					index /= value;
			}
			if (index < vector_size(vec))
			{
				string_assign(&vector_at(tmpV, 4), &vector_at(vec, index));
				ReplaceDefineDynamic(SSGS, &vector_at(tmpV, 4));
			}
		}
	}
	*tmpC = '\0';
}
