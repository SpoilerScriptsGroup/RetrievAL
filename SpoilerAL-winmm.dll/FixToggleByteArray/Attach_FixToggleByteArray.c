#include <windows.h>
#include "TStringDivision.h"

EXTERN_C void __cdecl FixToggleByteArray();

EXTERN_C void __cdecl Attach_FixToggleByteArray()
{
	// TSSGCtrl::LoopCodeToByteCode
	*(LPBYTE )(0x0050F317 + 1) = dtESCAPE | dtBYTEARRAY;
	*(LPDWORD)(0x0050F35A + 1) = (DWORD)TStringDivision_Half - (0x0050F35A + 1 + sizeof(DWORD));

	// TSSGCtrl::MakeDataCode
	*(LPBYTE )(0x0050F62F + 1) = dtESCAPE | dtBYTEARRAY;
	*(LPDWORD)(0x0050F675 + 1) = (DWORD)TStringDivision_Half - (0x0050F675 + 1 + sizeof(DWORD));
}
