#include <windows.h>

EXTERN_C void __cdecl TStringFiler_LoadFromFile();
EXTERN_C void __cdecl TTitleSelectForm_FindSSG_AdjustLogFooter();
EXTERN_C void __cdecl TSSGCtrl_GetSSGScriptVec_AdjustLogFooter();

#define NOP        (BYTE )0x90
#define NOP_X2     (WORD )0x9066
#define NOP_X4     (DWORD)0x00401F0F
#define CALL_REL32 (BYTE )0xE8
#define JMP_REL32  (BYTE )0xE9
#define JMP_SHORT  (BYTE )0xEB

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

	// TSSGCtrl::SeekScriptIndex
	//   NowByte points to line head
	*(LPWORD )0x004F5818 = 0xD233;// xor edx, edx
	*(LPBYTE )0x004F581A = JMP_REL32;
	*(LPDWORD)0x004F581B = 0x004F5829 - (0x004F581B + sizeof(DWORD));
	*(LPBYTE )0x004F581F = 0x8D;
	*(LPDWORD)0x004F5820 = 0xC88BE842;
	//   prevent duplicate registration
	*(LPBYTE )0x004F5B43 = 0x004F5B87 - (0x004F5B43 + sizeof(BYTE));

	*(LPBYTE )0x004F5B79 = 0x75;
	*(LPBYTE )0x004F5B7A = 0x004F5B87 - (0x004F5B7A + sizeof(BYTE));
	//   ignores [/script] before [script]
	*(LPBYTE )0x004F5B7D = 0x39;
	*(LPWORD )0x004F5B7E = 0xF085;
	*(LPDWORD)0x004F5B80 = 0x74FFFFF8;
	*(LPBYTE )0x004F5B84 = (BYTE)(0x004F5B1A - (0x004F5B84 + sizeof(BYTE)));
	*(LPBYTE )0x004F5B85 = JMP_SHORT;
	*(LPBYTE )0x004F5B86 = 0x004F5B8C - (0x004F5B86 + sizeof(BYTE));
	*(LPBYTE )0x004F5B87 = JMP_REL32;

	*(LPBYTE )0x004F5BA3 = 0x03;// sub => add

	*(LPBYTE )0x004F6033 = 0x83;
	*(LPDWORD)0x004F6034 = 0x33041C6B;
	*(LPDWORD)0x004F6038 = 0xF09589D2;
	*(LPWORD )0x004F603C = 0xFFF8;
	*(LPBYTE )0x004F603E = 0xFF;

	*(LPDWORD)0x004F78AB = 0x004F581F - (0x004F78AB + sizeof(DWORD));

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
