#include <windows.h>

EXTERN_C void __cdecl FixToggleByteArray();

EXTERN_C void __cdecl Attach_FixToggleByteArray()
{
	// TSSGCtrl::MakeDataCode
	*(LPDWORD)(0x0050F675 + 1) = (DWORD)FixToggleByteArray - (0x0050F675 + 1 + sizeof(DWORD));
}
