#include <windows.h>

EXTERN_C void __cdecl Caller_TSSString_Read_UnicodeString();
EXTERN_C void __cdecl Caller_TSSString_Write_UnicodeString();
EXTERN_C void __cdecl Caller_TSSString_ToByteCode_UnicodeString();

#define JMP_REL32 (BYTE)0xE9

EXTERN_C void Attach_StringSubject()
{
	// TSSString::Read
	*(LPDWORD)(0x0052B034 + 1) = (DWORD)Caller_TSSString_Read_UnicodeString - (0x0052B034 + 1 + sizeof(DWORD));

	// TSSString::Write
	*(LPDWORD)(0x0052B54E + 1) = (DWORD)Caller_TSSString_Write_UnicodeString - (0x0052B54E + 1 + sizeof(DWORD));

	// TSSString::ToByteCode
	*(LPDWORD)(0x0052B800 + 1) = (DWORD)Caller_TSSString_ToByteCode_UnicodeString - (0x0052B800 + 1 + sizeof(DWORD));
}
