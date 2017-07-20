#include <windows.h>
#include "intrinsic.h"

EXTERN_C void __cdecl TSSGCtrl_GetNameString();

EXTERN_C void __cdecl Attach_FormatNameString()
{
	// TSSGCtrl::AddressNaming
	*(LPDWORD)(0x0050490D + 1) = (DWORD)TSSGCtrl_GetNameString - (0x0050490D + 1 + sizeof(DWORD));

	#define CASE_INTEGER 0x00506061
	#define CASE_FLOAT   0x005061E2

	//   switch(*tmpV[5].rbegin()){//[tmpV[5].size()-1]){
	/*
		cmp     ecx, 'a' - 'G'  ; 00505FEF _ 83. F9, 1A
		je      FLOAT           ; 00505FF2 _ 74, 05
		cmp     ecx, 'A' - 'G'  ; 00505FF4 _ 83. F9, FA
		jne     CASE_INTEGER    ; 00505FF7 _ 75, 68
	FLOAT:
		jmp     CASE_FLOAT      ; 00505FF9 _ E9, 000001E4
	*/
	*(LPDWORD)0x00505FF0 = BSWAP32(0xF91A7405);
	*(LPDWORD)0x00505FF4 = BSWAP32(0x83F9FA75);
	*(LPDWORD)0x00505FF8 = BSWAP32(0x68E9E401);

	*(LPDWORD)(0x00506004 + 2) = CASE_INTEGER - (0x00506004 + 2 + sizeof(DWORD));
	*(LPDWORD)0x0050601D = CASE_INTEGER;
	*(LPDWORD)0x00506025 = CASE_INTEGER;
	*(LPDWORD)0x00506029 = CASE_INTEGER;
	*(LPDWORD)0x0050602D = CASE_INTEGER;
	*(LPDWORD)0x00506031 = CASE_INTEGER;
	// 0x00506035 ... case 'n':
	*(LPDWORD)0x0050603D = CASE_INTEGER;
	*(LPDWORD)0x00506041 = CASE_INTEGER;
	*(LPDWORD)0x00506045 = CASE_INTEGER;
	*(LPDWORD)0x00506049 = CASE_INTEGER;
	*(LPDWORD)0x0050604D = CASE_INTEGER;
	*(LPDWORD)0x00506055 = CASE_INTEGER;
	*(LPDWORD)0x00506059 = CASE_INTEGER;

	#undef CASE_INTEGER
	#undef CASE_FLOAT

	// TSSGSubject::GetSubjectName
	*(LPDWORD)(0x0052CF93 + 1) = (DWORD)TSSGCtrl_GetNameString - (0x0052CF93 + 1 + sizeof(DWORD));
}
