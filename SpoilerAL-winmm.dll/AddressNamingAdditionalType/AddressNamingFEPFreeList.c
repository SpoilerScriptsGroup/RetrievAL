#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_vector.h"
#include "bcb6_std_string.h"
#include "TSSGCtrl.h"
#include "TStringDivision.h"

EXTERN_C void __stdcall ReplaceDefineDynamic(TSSGSubject *SSGS, string *line);

EXTERN_C void __fastcall AddressNamingFEPFreeList(
	unsigned long  const DataSize,
	char          *const tmpC,
	vector_string *const tmpV,
	TSSGCtrl      *const SSGCtrl,
	TSSGSubject   *const SSGS)
{
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
			string        *it;

			index =
				DataSize == 4 ? *(LPDWORD)tmpC :
				DataSize == 3 ? *(LPDWORD)tmpC & 0x00FFFFFF :
				DataSize == 2 ? *(LPWORD )tmpC :
				                *(LPBYTE )tmpC;
			index = TSSGCtrl_CheckIO_FEP(SSGCtrl, SSGS, index, FALSE);
			for (it = vector_begin(vec); it != vector_end(vec); it++)
			{
				string        Token;
				string        tmpS;
				char          *endptr;
				unsigned long value;
				char          ch;

				string_assign(&vector_at(tmpV, 3), it);
				ReplaceDefineDynamic(SSGS, &vector_at(tmpV, 3));
				string_ctor_assign_cstr_with_length(&Token, "=", 1);
				TStringDivision_Half(
					&tmpS,
					&SSGCtrl->strD,
					&vector_at(tmpV, 3),
					Token,
					0,
					0);
				if (!string_empty(&tmpS))
				{
					value = strtoul(string_c_str(&tmpS), &endptr, 0);
					ch = *endptr;
				}
				else
				{
					ch = UCHAR_MAX;
				}
				string_dtor(&tmpS);
				if (ch || index != value)
					continue;
				string_assign(&vector_at(tmpV, 4), &vector_at(tmpV, 3));
				break;
			}
		}
	}
	string_clear(&vector_at(tmpV, 3));
	*tmpC = '\0';
}
