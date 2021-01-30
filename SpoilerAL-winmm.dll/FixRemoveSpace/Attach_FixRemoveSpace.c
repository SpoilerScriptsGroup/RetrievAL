#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"

EXTERN_C void __cdecl TSSGCtrl_TrimStringByRef();
EXTERN_C void __cdecl TSSGCtrl_TrimString();
EXTERN_C void __cdecl TSSGCtrl_ByteArrayRemoveSpace();

static string* __cdecl TSSGCtrl_EnumReadSSR_tmpS_assign(
	string     *const tmpS,
	char       *const _M_start,
	char       *const _M_finish,
	char const *const _M_end_of_storage/*__false_type*/)
{
	string_dtor(tmpS);
	string_begin(tmpS) = _M_start;
	string_end(tmpS) = _M_finish;
	string_end_of_storage(tmpS) = _M_end_of_storage;
	return tmpS;// Trimming is delegated to "FixRepeat/Attach_FixRepeat.c"
}

static string* __cdecl TSSGCtrl_EnumReadSSR_Remove_tmpS(
	string     *const Remove,
	void const *const strD,
	string     *const Src)
{
	*Remove = *Src;
	string_ctor_null(Src);
	return Remove;// Trimming is delegated to "FixRepeat/Attach_FixRepeat.c"
}

#define TSSGCtrl_TrimString TSSGCtrl_EnumReadSSR_Remove_tmpS

#define NOP       (BYTE)0x90
#define JMP_REL32 (BYTE)0xE9
#define JMP_SHORT (BYTE)0xEB

EXTERN_C void __cdecl Attach_FixRemoveSpace()
{
	// TSSGCtrl::EnumReadSSR
	*(LPBYTE )(0x004FFCBB + 1) = 0;

	// TSSGCtrl::EnumReadSSR
#if 1
	*(LPWORD )(0x004FFD20 + 0) = BSWAP16(0x8B87);// mov eax, [edi + ...]
	*(LPDWORD)(0x004FFD20 + 2) = offsetof(string, _M_end_of_storage);
	*(LPDWORD)(0x004FFD47 + 1) = (DWORD)TSSGCtrl_EnumReadSSR_tmpS_assign - (0x004FFD47 + 1 + sizeof(DWORD));
	*(LPBYTE )(0x004FFD75 + 0) = JMP_SHORT;// omit dtor Half
#else
	*(LPBYTE )0x004FFD9C = JMP_REL32;
	*(LPDWORD)0x004FFD9D = (DWORD)TSSGCtrl_TrimStringByRef - (0x004FFD9D + sizeof(DWORD));
	*(LPBYTE )0x004FFDA1 = NOP;
#endif

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
