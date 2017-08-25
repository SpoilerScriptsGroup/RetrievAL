#include <windows.h>

EXTERN_C void __cdecl FixGetSSGDataFile();

EXTERN_C void __cdecl Attach_FixGetSSGDataFile()
{
	// TSSGCtrl::GetSSGDataFile
	*(LPDWORD)(0x004EF9C2 + 1) = (DWORD)FixGetSSGDataFile - (0x004EF9C2 + 1 + sizeof(DWORD));
}
