#include <windows.h>

EXTERN_C void __cdecl Caller_StrToProcessAccessElementVecWithoutRel();
EXTERN_C void __cdecl Caller_ParsingWithRel();
EXTERN_C void __cdecl Caller_FixLoopByteArray();

#define JMP_REL32 (BYTE )0xE9
#define NOP       (BYTE )0x90
#define NOP_X4    (DWORD)0x90909090

EXTERN_C void __cdecl Attach_FixLoopByteArray()
{
	// TSSGCtrl::StrToProcessAccessElementVec
	*(LPBYTE )0x00506F78 = JMP_REL32;
	*(LPDWORD)0x00506F79 = (DWORD)Caller_StrToProcessAccessElementVecWithoutRel - (0x00506F79 + sizeof(DWORD));
	*(LPDWORD)0x00506F7D = NOP_X4;

	// TSSGCtrl::StrToProcessAccessElementVec
	*(LPBYTE )(0x0050AEF8 + 1) = 0x00;
	*(LPBYTE )0x0050AF9C = JMP_REL32;
	*(LPDWORD)0x0050AF9D = (DWORD)Caller_FixLoopByteArray - (0x0050AF9D + sizeof(DWORD));
	*(LPBYTE )0x0050AFA1 = NOP;

	// TSSGCtrl::StrToProcessAccessElementVec
	*(LPBYTE )0x0050B4D4 = JMP_REL32;
	*(LPDWORD)0x0050B4D5 = (DWORD)Caller_ParsingWithRel - (0x0050B4D5 + sizeof(DWORD));
}
