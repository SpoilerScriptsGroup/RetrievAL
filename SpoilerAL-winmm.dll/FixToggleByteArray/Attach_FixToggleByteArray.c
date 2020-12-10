#include <windows.h>

EXTERN_C void __cdecl FixToggleByteArray();

EXTERN_C void __cdecl Attach_FixToggleByteArray()
{
	// TSSGCtrl::LoopCodeToByteCode
	*(LPDWORD)(0x0050F35A + 1) = (DWORD)FixToggleByteArray - (0x0050F35A + 1 + sizeof(DWORD));

	// TSSGCtrl::MakeDataCode
	*(LPDWORD)(0x0050F675 + 1) = (DWORD)FixToggleByteArray - (0x0050F675 + 1 + sizeof(DWORD));
}
