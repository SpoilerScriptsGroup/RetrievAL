#include <windows.h>

EXTERN_C void __cdecl TSSGScriptStruct_EscapeDistraction();

EXTERN_C void __cdecl Attach_FixGetDistractionString()
{
	// TSSGScriptStruct::GetDistractionString
	*(LPDWORD)(0x0051FED1 + 1) = (DWORD)TSSGScriptStruct_EscapeDistraction - (0x0051FED1 + 1 + sizeof(DWORD));
	*(LPDWORD)(0x0051FF78 + 1) = (DWORD)TSSGScriptStruct_EscapeDistraction - (0x0051FF78 + 1 + sizeof(DWORD));
}
