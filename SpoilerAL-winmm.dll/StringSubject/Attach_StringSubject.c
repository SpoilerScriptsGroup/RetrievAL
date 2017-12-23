#include <windows.h>
#include "intrinsic.h"

#if 0
EXTERN_C void __cdecl Caller_TSSString_Setting_CheckCodePage();
#endif
EXTERN_C void __cdecl Caller_TSSString_Read_TranslateString();
EXTERN_C void __cdecl Caller_TSSString_Read_terminate_Data();
EXTERN_C void __cdecl Caller_TSSString_Write_WriteString_reserve();
EXTERN_C void __cdecl Caller_TSSString_ToByteCode_tmpS_reserve();

EXTERN_C void __cdecl Attach_StringSubject()
{
	// replaced at "SubjectStringTable\SubjectStringOperator.c" - TSSString_Setting_SetEndWord
#if 0
	// TSSString::Setting
	*(LPDWORD)(0x0052AABA + 1) = (DWORD)Caller_TSSString_Setting_CheckCodePage - (0x0052AABA + 1 + sizeof(DWORD));
#endif

	// TSSString::Read
	//   char *tmpC = new char[size+1]; -> new char[size+2]
	/*
		mov     edi, esp                                ; 0052AF98 _ 8B. FC
		lea     ecx, [ebx + 7FH]                        ; 0052AF9A _ 8D. 4B, 7F
		push    15                                      ; 0052AF9D _ 6A, 0F
		inc     ecx                                     ; 0052AF9F _ 41
	*/
	*(LPDWORD)0x0052AF98 = BSWAP32(0x8BFC8D4B);
	*(LPDWORD)0x0052AF9C = BSWAP32(0x7F6A0F41);
	/*
		mov     dword ptr [ebp - 4CH], eax              ; 0052AFA8 _ 89. 45, B4
		mov     eax, dword ptr [ebx + 78H]              ; 0052AFAB _ 8B. 43, 78
		mov     esp, edi                                ; 0052AFAE _ 8B. E7
		inc     eax                                     ; 0052AFB0 _ 40
	*/
	*(LPDWORD)0x0052AFA8 = BSWAP32(0x8945B48B);
	*(LPDWORD)0x0052AFAC = BSWAP32(0x43788BE7);
	*(LPBYTE )0x0052AFB0 =         0x40       ;

	//   tmpC[size] = (char)NULL; -> *(LPWSTR)&tmpC[size] = NULL
	/*
		mov     edx, dword ptr [ebx + 78H]              ; 0052AFBA _ 8B. 53, 78
		mov     word ptr [edi + edx], 0                 ; 0052AFBE _ 66: C7. 04 3A, 0000
		nop                                             ; 0052AFC4 _ 90
	*/
	*(LPBYTE )0x0052AFBB =               0x53 ;
	*(LPWORD )0x0052AFBE = BSWAP16(    0x66C7);
	*(LPDWORD)0x0052AFC0 = BSWAP32(0x043A0000);
	*(LPBYTE )0x0052AFC4 =         0x90       ;

	//   delete[] tmpC;
	*(LPDWORD)(0x0052B034 + 1) = (DWORD)Caller_TSSString_Read_TranslateString - (0x0052B034 + 1 + sizeof(DWORD));

	//   Data[Pos]=(byte)NULL;
	/*
		push    eax                                     ; 0052B122 _ 50
		call    Caller_TSSString_Read_terminate_Data    ; 0052B123 _ E8, <offset Caller_TSSString_Read_terminate_Data - 0052B128H>
		nop                                             ; 0052B128 _ 90
	*/
	*(LPWORD )0x0052B122 = BSWAP16(    0x50E8);
	*(LPDWORD)0x0052B124 = (DWORD)Caller_TSSString_Read_terminate_Data - (0x0052B124 + sizeof(DWORD));
	*(LPBYTE )0x0052B128 =         0x90       ;

	// TSSString::Write
	*(LPDWORD)(0x0052B358 + 1) = (DWORD)Caller_TSSString_Write_WriteString_reserve - (0x0052B358 + 1 + sizeof(DWORD));

	// TSSString::ToByteCode
	*(LPDWORD)(0x0052B800 + 1) = (DWORD)Caller_TSSString_ToByteCode_tmpS_reserve - (0x0052B800 + 1 + sizeof(DWORD));
}
