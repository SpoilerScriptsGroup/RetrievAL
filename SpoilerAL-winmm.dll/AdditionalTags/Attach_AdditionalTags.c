#include <windows.h>

EXTERN_C void __cdecl Caller_AdditionalTags();

#define JMP_REL32 (BYTE)0xE9
#define NOP       (BYTE)0x90

EXTERN_C void __cdecl Attach_AdditionalTags()
{
	// TSSGCtrl::EnumReadSSG
	*(LPBYTE )0x004EB4FA = 0x26;
	*(LPBYTE )0x004EB4FB = JMP_REL32;
	*(LPDWORD)0x004EB4FC = (DWORD)Caller_AdditionalTags - (0x004EB4FC + sizeof(DWORD));
	*(LPBYTE )0x004EB500 = NOP;
	*(LPBYTE )0x004EB501 = NOP;
}
