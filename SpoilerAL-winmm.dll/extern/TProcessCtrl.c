#define USING_NAMESPACE_BCB6_STD
#include "TProcessCtrl.h"

void(__cdecl * const TProcessCtrl_Clear)(TProcessCtrl *this) = (LPVOID)0x004A3350;
void(__cdecl * const TProcessCtrl_LoadHeapList)(TProcessCtrl *this) = (LPVOID)0x004A3980;
void(__cdecl * const TProcessCtrl_LoadModuleList)(TProcessCtrl *this) = (LPVOID)0x004A4E4C;
THeapListData *(__cdecl * const TProcessCtrl_GetHeapList)(TProcessCtrl *this, unsigned long ListNo) = (LPVOID)0x004A5A94;
HANDLE(__cdecl * const TProcessCtrl_Open)(TProcessCtrl *this, DWORD Mode) = (LPVOID)0x004A618C;
BOOLEAN(__cdecl * const TProcessCtrl_OneRead)(HANDLE ProcessHandle, DWORD Address, void *Data, DWORD Size) = (LPVOID)0x004A6FF4;

extern const DWORD F004A61F8;

__declspec(naked) LPMODULEENTRY32A __fastcall TProcessCtrl_GetModuleFromName(TProcessCtrl *this, LPCSTR Name)
{
	__asm
	{
		sub     esp, size string
		mov     eax, ecx
		mov     ecx, esp
		push    eax
		call    string_ctor_assign_cstr
		call    dword ptr [F004A61F8]
		add     esp, 4 + size string
		ret
	}
}
