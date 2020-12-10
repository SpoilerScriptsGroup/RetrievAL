#include <windows.h>

EXTERN_C void __cdecl FixGetModuleFromName();

EXTERN_C void __cdecl Attach_FixGetModuleFromName()
{
	// TProcessCtrl::GetModuleFromName
	*(LPDWORD)(0x004A6258 + 1) = (DWORD)FixGetModuleFromName - (0x004A6258 + 1 + sizeof(DWORD));
}
