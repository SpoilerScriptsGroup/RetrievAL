#include <windows.h>

EXTERN_C void __cdecl TSSGCtrl_GetNameString();

EXTERN_C void Attach_FormatNameString()
{
	// TSSGCtrl::AddressNaming
	*(LPDWORD)(0x0050490D + 1) = (DWORD)TSSGCtrl_GetNameString - (0x0050490D + 1 + sizeof(DWORD));

	// TSSGSubject::GetSubjectName
	*(LPDWORD)(0x0052CF93 + 1) = (DWORD)TSSGCtrl_GetNameString - (0x0052CF93 + 1 + sizeof(DWORD));
}
