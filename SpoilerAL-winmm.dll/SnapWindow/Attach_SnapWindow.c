#include <windows.h>
#include "intrinsic.h"

EXTERN_C void __cdecl TMainForm_ctor_AttachSnapWindow();
EXTERN_C void __cdecl TFindNameForm_ctor_AttachSnapWindow();
EXTERN_C void __cdecl TGuideForm_ctor_AttachSnapWindow();
EXTERN_C void __cdecl TSearchForm_ctor_AttachSnapWindow();

#define CALL_REL32 (BYTE)0xE8
#define NOP        (BYTE)0x90
#define NOP_X2     (WORD)0x9090

EXTERN_C void __cdecl Attach_SnapWindow()
{
	// TMainForm::TMainForm
	*(LPBYTE )0x00401FAB = CALL_REL32;
	*(LPDWORD)0x00401FAC = (DWORD)TMainForm_ctor_AttachSnapWindow - (0x00401FAC + sizeof(DWORD));
	*(LPBYTE )0x00401FB0 = NOP;

	// TFindNameForm::TFindNameForm
	*(LPBYTE )0x0048397C = CALL_REL32;
	*(LPDWORD)0x0048397D = (DWORD)TFindNameForm_ctor_AttachSnapWindow - (0x0048397D + sizeof(DWORD));
	*(LPBYTE )0x00483981 = NOP;

	// TGuideForm::TGuideForm
	/*
		call    TGuideForm_ctor_AttachSnapWindow        ; 0048C247 _ E8, ????????
		jmp     0048C251H                               ; 0048C24C _ EB, 03
	*/
	*(LPBYTE )0x0048C247 = CALL_REL32;
	*(LPDWORD)0x0048C248 = (DWORD)TGuideForm_ctor_AttachSnapWindow - (0x0048C248 + sizeof(DWORD));
	*(LPDWORD)0x0048C24C = BSWAP32(0xEB039090);
	*(LPBYTE )0x0048C250 = NOP;

	// TSearchForm::TSearchForm
	/*
		call    TSearchForm_ctor_AttachSnapWindow       ; 004919AF _ E8, ????????
		jmp     004919B8H                               ; 004919B4 _ EB, 02
	*/
	*(LPBYTE )0x004919AF = CALL_REL32;
	*(LPDWORD)0x004919B0 = (DWORD)TSearchForm_ctor_AttachSnapWindow - (0x004919B0 + sizeof(DWORD));
	*(LPDWORD)0x004919B4 = BSWAP32(0xEB029090);
	*(LPWORD )0x004919B6 = NOP_X2;
}
