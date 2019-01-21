#include <windows.h>

EXTERN_C void __cdecl TStringFiler_LoadFromFile();
EXTERN_C void __cdecl TTitleSelectForm_FindSSG_AdjustLogFooter();
EXTERN_C void __cdecl TSSGCtrl_GetSSGScriptVec_AdjustLogFooter();

#define NOP        (BYTE )0x90
#define NOP_X4     (DWORD)0x90909090
#define CALL_REL32 (BYTE )0xE8
#define JMP_REL32  (BYTE )0xE9

EXTERN_C void __cdecl Attach_LoadFromFile()
{
	static const char* const EndWord = "[/script]";

	// TTitleSelectForm::FindSSG
	*(LPBYTE )0x0047180D = CALL_REL32;
	*(LPDWORD)0x0047180E = (DWORD)TTitleSelectForm_FindSSG_AdjustLogFooter - (0x0047180E + sizeof(DWORD));
	*(LPBYTE )0x00471812 = NOP;

	// TStringFiler::LoadFromFile
	*(LPBYTE )0x004B65D0 = JMP_REL32;
	*(LPDWORD)0x004B65D1 = (DWORD)TStringFiler_LoadFromFile - (0x004B65D1 + sizeof(DWORD));
	*(LPDWORD)0x004B65D5 = NOP_X4;

	// TSSGCtrl::GetSSGScriptVec
	*(LPBYTE )0x004FAE66 = CALL_REL32;
	*(LPDWORD)0x004FAE67 = (DWORD)TSSGCtrl_GetSSGScriptVec_AdjustLogFooter - (0x004FAE67 + sizeof(DWORD));
	*(LPBYTE )0x004FAE6B = NOP;

	// TSSGCtrl::OpenSSG
	*(LPDWORD)(0x004FD1D7 + 1) = (DWORD)EndWord;
	*(LPBYTE )(0x004FD1E1 + 0) = 0x8D;
	*(LPBYTE )(0x004FD1E1 + 1) = 0x15;
	*(LPDWORD)(0x004FD1E1 + 2) = ULONG_MAX;
	*(LPBYTE )(0x004FD1EF + 1) = 0x10;// strF::smBYTE without strF::emSTRING (0x02) avoiding asserts
}
