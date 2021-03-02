#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "TMainForm.h"
#include "TTitleSelectForm.h"
#include "TFindNameForm.h"

static void __fastcall TTitleSelectForm_FindSSG_DistractionVec_ctor(vector_string *const DistractionVec)
{
	*DistractionVec = *MainForm->DistractionVec;
}

static void __cdecl TTitleSelectForm_FindSSG_DistractionVec_destroy_aux(
	const string        *const begin,
	const string        *const end,
	const vector_string *const DistractionVec)
{
	*MainForm->DistractionVec = *DistractionVec;
}

#define PUSH_ECX  (BYTE)0x51
#define NOP       (BYTE)0x90
#define NOP_X4   (DWORD)0x00401F0F
#define RET_IMM16 (BYTE)0xC2
#define CALL_REL  (BYTE)0xE8
#define JMP_REL32 (BYTE)0xE9
#define JMP_REL8  (BYTE)0xEB

#define MAKEDWORD(a, b, c, d) (DWORD)MAKELONG(MAKEWORD(a, b), MAKEWORD(c, d))

EXTERN_C void __cdecl Attach_FixTitleSelectForm()
{
	// TTitleSelectForm::FindSSG
	*(LPBYTE )0x004716E9 = 0x83;
	*(LPDWORD)0x004716EC = BSWAP32(0x028D4DE8);
	*(LPBYTE )0x004716F0 = CALL_REL;
	*(LPDWORD)0x004716F1 = (DWORD)TTitleSelectForm_FindSSG_DistractionVec_ctor - (0x004716F1 + sizeof(DWORD));
	//   skip TStringFiler
	*(LPBYTE )0x004725E9 = JMP_REL32;
	*(LPDWORD)0x004725EA = 0;

	*(LPBYTE )0x00472785 = JMP_REL8;
	*(LPBYTE )0x00472786 = (BYTE)(0x00472780 - (0x00472786 + sizeof(BYTE)));
	*(LPDWORD)0x00472787 = NOP_X4;
	//   omit DistractionVec dtor
	*(LPBYTE )0x00472CF7 = 0x8D;
	*(LPBYTE )0x00472CF8 = 0x95;
	*(LPDWORD)0x00472CF9 = -(signed)sizeof(vector_string);
	*(LPDWORD)0x00472D1F = (DWORD)TTitleSelectForm_FindSSG_DistractionVec_destroy_aux - (0x00472D1F + sizeof(DWORD));

	*(LPBYTE )0x00472D4D = JMP_REL8;

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
