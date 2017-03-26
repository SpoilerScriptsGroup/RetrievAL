#include <windows.h>

EXTERN_C void __cdecl AddressNamingAdditionalType();

EXTERN_C void Attach_AddressNamingAdditionalType()
{
	// TSSGCtrl::AddressNaming
	*(LPDWORD)(0x00505E11 + 2) = (DWORD)AddressNamingAdditionalType - (0x00505E11 + 2 + sizeof(DWORD));
}
