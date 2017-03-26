#include <windows.h>

EXTERN_C void __cdecl FixAdjustByValue();

EXTERN_C void Attach_FixAdjustByValue()
{
	// TSSList::ToByteCode
	*(LPDWORD)(0x0052A513 + 1) = (DWORD)FixAdjustByValue - (0x0052A513 + 1 + sizeof(DWORD));
}
