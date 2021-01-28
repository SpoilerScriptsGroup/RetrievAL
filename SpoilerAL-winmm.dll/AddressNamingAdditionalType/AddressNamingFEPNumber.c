#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#define _NO_CRT_STDIO_INLINE
#include <stdio.h>
#include <float.h>
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_vector_string.h"
#include "TSSGCtrl.h"

EXTERN_C void __fastcall AddressNamingFEPNumber(
	unsigned long  const DataSize,
	char          *const tmpC,
	vector_string *const tmpV,
	TSSGCtrl      *const SSGCtrl,
	TSSGSubject   *const SSGS)
{
	string_clear(&vector_at(tmpV, 3));
	if (!string_empty(&vector_at(tmpV, 5)))
	{
		switch (*(string_end(&vector_at(tmpV, 5)) - 1))
		{
		case 'e': case 'E': case 'f': case 'g': case 'G': case 'a': case 'A':
			if (DataSize <= sizeof(double))
			{
				char   buf[256];
				double Val;
				UINT   length;

				Val =
					DataSize == sizeof(double) ? *(double *)tmpC :
					DataSize >= sizeof(float ) ? *(float  *)tmpC :
					0;
				Val = TSSGCtrl_CheckIO_FEPDouble(SSGCtrl, SSGS, Val, FALSE);
				length = _snprintf(buf, _countof(buf), !_isnan(Val) ? string_c_str(&vector_at(tmpV, 5)) : "%f", Val);
				if (length >= _countof(buf))
					length = (int)length >= 0 ? _countof(buf) - 1 : 0;
				string_assign_cstr_with_length(&vector_at(tmpV, 4), buf, length);
			}
			break;
		case 'n':
			break;
		default:
			if (DataSize <= 4)
			{
				char          buf[256];
				unsigned long Val;
				UINT          length;

				Val =
					DataSize == 4 ? *(LPDWORD)tmpC :
					DataSize == 3 ? *(LPDWORD)tmpC & 0x00FFFFFF :
					DataSize == 2 ? *(LPWORD )tmpC :
					                *(LPBYTE )tmpC;
				Val = TSSGCtrl_CheckIO_FEP(SSGCtrl, SSGS, Val, FALSE);
				length = _snprintf(buf, _countof(buf), string_c_str(&vector_at(tmpV, 5)), Val);
				if (length >= _countof(buf))
					length = (int)length >= 0 ? _countof(buf) - 1 : 0;
				string_assign_cstr_with_length(&vector_at(tmpV, 4), buf, length);
			}
			break;
		}
	}
	*tmpC = '\0';
}
