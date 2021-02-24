#include <windows.h>
#include "TStringDivision.h"

EXTERN_C void __cdecl SplitElement();

EXTERN_C void __cdecl Attach_FixSplitElement()
{
	// TSSDoubleList::Read
	*(LPBYTE )(0x004C4590 + 1) = dtBYTEARRAY;
	*(LPDWORD)(0x004C45EB + 1) = (DWORD)TStringDivision_Half - (0x004C45EB + 1 + sizeof(DWORD));

	// TSSDoubleList::Write
	*(LPBYTE )(0x004C5690 + 1) = dtBYTEARRAY;
	*(LPDWORD)(0x004C56EB + 1) = (DWORD)TStringDivision_Half - (0x004C56EB + 1 + sizeof(DWORD));

	// TSSDoubleList::ToByteCode
	*(LPBYTE )(0x004C6B39 + 1) = dtBYTEARRAY;
	*(LPDWORD)(0x004C6B9D + 1) = (DWORD)TStringDivision_Half - (0x004C6B9D + 1 + sizeof(DWORD));

	// TSSDoubleToggle::Read
	*(LPBYTE )(0x004C9A60 + 1) = dtBYTEARRAY;
	*(LPDWORD)(0x004C9AC4 + 1) = (DWORD)TStringDivision_Half - (0x004C9AC4 + 1 + sizeof(DWORD));
	*(LPBYTE )(0x004C9B60 + 1) = dtBYTEARRAY;
	*(LPDWORD)(0x004C9BC4 + 1) = (DWORD)TStringDivision_Half - (0x004C9BC4 + 1 + sizeof(DWORD));

	// TSSDoubleToggle::Write
	*(LPBYTE )(0x004CC406 + 1) = dtBYTEARRAY;
	*(LPDWORD)(0x004CC467 + 1) = (DWORD)TStringDivision_Half - (0x004CC467 + 1 + sizeof(DWORD));
	*(LPBYTE )(0x004CC4F2 + 1) = dtBYTEARRAY;
	*(LPDWORD)(0x004CC553 + 1) = (DWORD)TStringDivision_Half - (0x004CC553 + 1 + sizeof(DWORD));
	
	*(LPBYTE )(0x004CC9DC + 1) = dtBYTEARRAY;
	*(LPDWORD)(0x004CCA35 + 1) = (DWORD)TStringDivision_Half - (0x004CCA35 + 1 + sizeof(DWORD));
	*(LPBYTE )(0x004CCAC0 + 1) = dtBYTEARRAY;
	*(LPDWORD)(0x004CCB19 + 1) = (DWORD)TStringDivision_Half - (0x004CCB19 + 1 + sizeof(DWORD));
}
	