#include <windows.h>

EXTERN_C void __cdecl TApplication_ActivateHint_epilog();

EXTERN_C void __cdecl Attach_FixHintWindow()
{
	// TApplication::ActivateHint
	*(LPDWORD)(0x00572526 + 1) = (DWORD)TApplication_ActivateHint_epilog;
}
