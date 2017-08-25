#include <windows.h>

EXTERN_C void __cdecl TSSGCtrl_TrimStringByRef();
EXTERN_C void __cdecl TSSGCtrl_TrimString();
EXTERN_C void __cdecl TSSGCtrl_ByteArrayRemoveSpace();

#define NOP       (BYTE)0x90
#define JMP_REL32 (BYTE)0xE9

EXTERN_C void __cdecl Attach_FixRemoveSpace()
{
	// TSSGCtrl::EnumReadSSR
	*(LPBYTE )(0x004FFCBB + 1) = 0;

	// TSSGCtrl::EnumReadSSR
	*(LPBYTE )0x004FFD9C = JMP_REL32;
	*(LPDWORD)0x004FFD9D = (DWORD)TSSGCtrl_TrimStringByRef - (0x004FFD9D + sizeof(DWORD));
	*(LPBYTE )0x004FFDA1 = NOP;

	// TSSGCtrl::EnumReadSSR
	*(LPDWORD)(0x004FFE3F + 1) = (DWORD)TSSGCtrl_TrimString - (0x004FFE3F + 1 + sizeof(DWORD));

	// TSSGCtrl::EnumReadSSR
	*(LPDWORD)(0x005000B8 + 1) = (DWORD)TSSGCtrl_TrimString - (0x005000B8 + 1 + sizeof(DWORD));

	// TSSGCtrl::EnumReadSSR
	*(LPDWORD)(0x005002ED + 1) = (DWORD)TSSGCtrl_TrimString - (0x005002ED + 1 + sizeof(DWORD));

	// TSSGCtrl::EnumReadSSR
	*(LPDWORD)(0x00500596 + 1) = (DWORD)TSSGCtrl_TrimString - (0x00500596 + 1 + sizeof(DWORD));

	// TSSGCtrl::EnumReadSSR
	*(LPDWORD)(0x00500807 + 1) = (DWORD)TSSGCtrl_TrimString - (0x00500807 + 1 + sizeof(DWORD));

	// TSSGCtrl::EnumReadSSR
	*(LPDWORD)(0x00500C65 + 1) = (DWORD)TSSGCtrl_TrimString - (0x00500C65 + 1 + sizeof(DWORD));

	// TSSGCtrl::StrToProcessAccessElementVec
	*(LPDWORD)(0x0050834B + 1) = (DWORD)TSSGCtrl_ByteArrayRemoveSpace - (0x0050834B + 1 + sizeof(DWORD));
}
