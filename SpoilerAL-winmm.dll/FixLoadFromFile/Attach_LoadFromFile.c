#include <windows.h>

EXTERN_C void __cdecl TStringFiler_LoadFromFile();

#define JMP_REL32 (BYTE )0xE9
#define NOP_X4    (DWORD)0x90909090

EXTERN_C void Attach_LoadFromFile()
{
	// TStringFiler::LoadFromFile
	*(LPBYTE )0x004B65D0 = JMP_REL32;
	*(LPDWORD)0x004B65D1 = (DWORD)TStringFiler_LoadFromFile - (0x004B65D1 + sizeof(DWORD));
	*(LPDWORD)0x004B65D5 = NOP_X4;
}
