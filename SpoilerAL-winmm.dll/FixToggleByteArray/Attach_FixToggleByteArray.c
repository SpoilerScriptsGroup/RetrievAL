#include <windows.h>
#include "TStringDivision.h"

EXTERN_C void __cdecl FixToggleByteArray();

EXTERN_C void __cdecl Attach_FixToggleByteArray()
{
	// TSSGCtrl::LoopCodeToByteCode
#if 1
	*(LPBYTE )(0x0050F317 + 1) = dtESCAPE | dtBYTEARRAY;
#else
	*(LPDWORD)(0x0050F35A + 1) = (DWORD)FixToggleByteArray - (0x0050F35A + 1 + sizeof(DWORD));
#endif

	// TSSGCtrl::MakeDataCode
#if 1
	*(LPBYTE )(0x0050F62F + 1) = dtESCAPE | dtBYTEARRAY;
#else
	*(LPDWORD)(0x0050F675 + 1) = (DWORD)FixToggleByteArray - (0x0050F675 + 1 + sizeof(DWORD));
#endif
}
