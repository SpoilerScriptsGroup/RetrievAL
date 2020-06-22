#include <windows.h>
#include "TTitleSelectForm.h"
#include "TFindNameForm.h"

#define PUSH_ECX  (BYTE)0x51
#define NOP       (BYTE)0x90
#define RET_IMM16 (BYTE)0xC2
#define JMP_REL32 (BYTE)0xE9
#define JMP_REL8  (BYTE)0xEB

#define MAKEDWORD(a, b, c, d) (DWORD)MAKELONG(MAKEWORD(a, b), MAKEWORD(c, d))

EXTERN_C void __cdecl Attach_FixTitleSelectForm()
{
	// TTitleSelectForm::SortCmbBoxChange
	*(LPDWORD)0x00473D63 = (DWORD)TTitleSelectForm_SortFunc;

	// TTitleSelectForm::CnvString
	*(LPBYTE )0x00476940 = JMP_REL32;
	*(LPDWORD)0x00476941 = (DWORD)TTitleSelectForm_CnvString - (0x00476941 + sizeof(DWORD));
	*(LPBYTE )0x00476945 = NOP;

	// TTitleSelectForm::TitleDGridMouseMove
	/*
		jmp     004783F0H                               ; 004783E2 _ EB, 0C
	*/
	*(LPWORD )0x004783E2 = MAKEWORD(JMP_REL8, 0x004783F0 - (0x004783E2 + 2));
	*(LPDWORD)0x004783E4 = MAKEDWORD(NOP, NOP, NOP, NOP);

	// TTitleSelectForm::KeyDGridMouseMove
	/*
		ret     8                                       ; 0047937C _ C2, 0008
	*/
	*(LPDWORD)0x0047937C = RET_IMM16 | (0x0008 << 8) | (PUSH_ECX << 24);

	// TFindNameForm::CnvString
	*(LPBYTE )0x00486528 = JMP_REL32;
	*(LPDWORD)0x00486529 = (DWORD)TFindNameForm_CnvString - (0x00486529 + sizeof(DWORD));
	*(LPBYTE )0x0048652D = NOP;
}
