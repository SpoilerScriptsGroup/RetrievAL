#include <windows.h>
#include "TStringDivision.h"

EXTERN_C void __cdecl TSSGCtrl_StrToProcessAccessElementVec_AppendNoEscapedChar();
EXTERN_C void __cdecl TStringDivision_IsLeadByteAtLowerUpper();

#define CALL_REL32 (BYTE )0xE8
#define JMP_REL32  (BYTE )0xE9
#define JZ_REL32   (WORD )0x840F
#define NOP        (BYTE )0x90
#define NOP_X4     (DWORD)0x90909090

EXTERN_C void __cdecl OptimizeStringDivision()
{
	// TSSGCtrl::StrToProcessAccessElementVec
	*(LPWORD )0x005072B4 = JZ_REL32;
	*(LPDWORD)0x005072B6 = 0x00507558 - (0x005072B6 + sizeof(DWORD));
	*(LPBYTE )0x005072BA = JMP_REL32;
	*(LPDWORD)0x005072BB = (DWORD)TSSGCtrl_StrToProcessAccessElementVec_AppendNoEscapedChar - (0x005072BB + sizeof(DWORD));
	*(LPBYTE )0x005072BF = NOP;
	*(LPDWORD)0x005072C0 = NOP_X4;

	// TStringDivision::Find
	*(LPBYTE )0x004AAB28 = JMP_REL32;
	*(LPDWORD)0x004AAB29 = (DWORD)TStringDivision_Find - (0x004AAB29 + sizeof(DWORD));
	*(LPBYTE )0x004AAB2D = NOP;

	// TStringDivision::FindBack
	*(LPBYTE )0x004AB0D4 = JMP_REL32;
	*(LPDWORD)0x004AB0D5 = (DWORD)TStringDivision_FindBack - (0x004AB0D5 + sizeof(DWORD));
	*(LPBYTE )0x004AB0D9 = NOP;

	// TStringDivision::Find
	*(LPBYTE )0x004AB594 = JMP_REL32;
	*(LPDWORD)0x004AB595 = (DWORD)TStringDivision_FindByVector - (0x004AB595 + sizeof(DWORD));
	*(LPBYTE )0x004AB599 = NOP;

	// TStringDivision::Half
	*(LPBYTE )0x004AC3EC = JMP_REL32;
	*(LPDWORD)0x004AC3ED = (DWORD)TStringDivision_Half - (0x004AC3ED + sizeof(DWORD));
	*(LPDWORD)0x004AC3F1 = NOP_X4;

	// TStringDivision::List
	*(LPBYTE )0x004ACECC = JMP_REL32;
	*(LPDWORD)0x004ACECD = (DWORD)TStringDivision_List - (0x004ACECD + sizeof(DWORD));
	*(LPDWORD)0x004ACED1 = NOP_X4;

	// TStringDivision::Remove
	*(LPBYTE )0x004AD9C4 = JMP_REL32;
	*(LPDWORD)0x004AD9C5 = (DWORD)TStringDivision_Remove - (0x004AD9C5 + sizeof(DWORD));
	*(LPDWORD)0x004AD9C9 = NOP_X4;

	// TStringDivision::Editing
	*(LPBYTE )0x004AD7FC = JMP_REL32;
	*(LPDWORD)0x004AD7FD = (DWORD)TStringDivision_Editing - (0x004AD7FD + sizeof(DWORD));
	*(LPBYTE )0x004AD801 = NOP;

	// TStringDivision::Remove
	*(LPBYTE )0x004ADE54 = JMP_REL32;
	*(LPDWORD)0x004ADE55 = (DWORD)TStringDivision_RemoveByMap - (0x004ADE55 + sizeof(DWORD));
	*(LPDWORD)0x004ADE59 = NOP_X4;

	// TStringDivision::Trim
	*(LPBYTE )0x004AE1DC = JMP_REL32;
	*(LPDWORD)0x004AE1DD = (DWORD)TStringDivision_Trim - (0x004AE1DD + sizeof(DWORD));
	*(LPDWORD)0x004AE1E1 = NOP_X4;

	// TStringDivision::WCList
	*(LPBYTE )0x004AE8C4 = JMP_REL32;
	*(LPDWORD)0x004AE8C5 = (DWORD)TStringDivision_WCList - (0x004AE8C5 + sizeof(DWORD));
	*(LPBYTE )0x004AE8C9 = NOP;

	// TStringDivision::Lower
	*(LPBYTE )0x004AE4EB = 0x76;

	// TStringDivision::Lower
	*(LPBYTE )0x004AE4F3 = CALL_REL32;
	*(LPDWORD)0x004AE4F4 = (DWORD)TStringDivision_IsLeadByteAtLowerUpper - (0x004AE4F4 + sizeof(DWORD));

	// TStringDivision::Upper
	*(LPBYTE )0x004AE5CF = 0x76;

	// TStringDivision::Upper
	*(LPBYTE )0x004AE5D7 = CALL_REL32;
	*(LPDWORD)0x004AE5D8 = (DWORD)TStringDivision_IsLeadByteAtLowerUpper - (0x004AE5D8 + sizeof(DWORD));
}
