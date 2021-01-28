#include <windows.h>
#include "TStringDivision.h"

EXTERN_C void * __fastcall AddressNamingFromFloat(void *, void *, void *, void *, unsigned);

#define JMP_REL32 (BYTE)0xE9
#define CALL_REL  (BYTE)0xE8
#define NOP       (BYTE)0x90
#define JZ_NEAR   (WORD)0x840F
#define PUSH_DWORD_PTR_EBP (WORD)0xB5FF

EXTERN_C void __cdecl Attach_AddressNamingFromFloat()
{
	// TSSGCtrl::AddressNaming
	*(LPWORD )0x0050625C = PUSH_DWORD_PTR_EBP;
	*(LPWORD )0x00506262 = PUSH_DWORD_PTR_EBP;
	*(LPDWORD)0x00506264 = -0x01E8;
	*(LPWORD )0x00506268 = BSWAP16(0x8BCE);// mov  ecx, esi
	*(LPDWORD)0x00506272 = (DWORD)AddressNamingFromFloat - (0x00506272 + sizeof(DWORD));

	*(LPWORD )0x0050627C = BSWAP16(0x85C0);// test eax, eax
	*(LPWORD )0x0050627E = JZ_NEAR;
	*(LPDWORD)0x00506280 = 0x005061EB - (0x00506280 + sizeof(DWORD));
	*(LPBYTE )0x0050628B =         0x83       ;// sub esp,
	*(LPDWORD)0x0050628C = BSWAP32(0xEC08DD1C);// 8; fstp
	*(LPBYTE )0x00506290 =         0x24       ;// qword ptr [esp]
	*(LPDWORD)0x00506299 = (DWORD)TStringDivision_ToStringDouble - (0x00506299 + sizeof(DWORD));
	*(LPBYTE )0x0050629F = 0x10;
}
