#include <windows.h>

EXTERN_C void __cdecl Caller_GetSSGDataFile();

EXTERN_C void Attach_FixGetSSGDataFile()
{
	// TSSGCtrl::GetSSGDataFile
	*(LPDWORD)(0x004EF9C2 + 1) = (DWORD)Caller_GetSSGDataFile - (0x004EF9C2 + 1 + sizeof(DWORD));
}
