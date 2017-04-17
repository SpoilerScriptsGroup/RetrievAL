#include <windows.h>
#include "bcb6_stdio.h"
#include "bcb6_float.h"
#include "bcb6_std_vector.h"
#include "bcb6_std_string.h"
#include "TSSGCtrl.h"

EXTERN_C void __stdcall AddressNamingFEPNumber(TSSGCtrl *SSGCtrl, TSSGSubject *SSGS, bcb6_std_vector *tmpV, unsigned long DataSize, char *tmpC)
{
	bcb6_std_string_clear((bcb6_std_string *)tmpV->_M_start + 3);
	if (!bcb6_std_string_empty((bcb6_std_string *)tmpV->_M_start + 5))
	{
		switch (*(((bcb6_std_string *)tmpV->_M_start + 5)->_M_finish - 1))
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
				length = bcb6__snprintf(buf, _countof(buf), !bcb6__isnan(Val) ? ((bcb6_std_string *)tmpV->_M_start + 5)->_M_start : "%f", Val);
				if (length >= _countof(buf))
					length = (int)length >= 0 ? _countof(buf) - 1 : 0;
				bcb6_std_string_assign_cstr_with_length((bcb6_std_string *)tmpV->_M_start + 4, buf, length);
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
				length = bcb6__snprintf(buf, _countof(buf), ((bcb6_std_string *)tmpV->_M_start + 5)->_M_start, Val);
				if (length >= _countof(buf))
					length = (int)length >= 0 ? _countof(buf) - 1 : 0;
				bcb6_std_string_assign_cstr_with_length((bcb6_std_string *)tmpV->_M_start + 4, buf, length);
			}
			break;
		}
	}
	*tmpC = '\0';
}
