#define USING_NAMESPACE_BCB6_STD
#include "TProcessCtrl.h"

THeapListData *(__cdecl *TProcessCtrl_GetHeapList)(TProcessCtrl *this, unsigned long ListNo) = (LPVOID)0x004A5A94;
HANDLE(__cdecl *TProcessCtrl_Open)(TProcessCtrl *this, DWORD Mode) = (LPVOID)0x004A618C;
BOOLEAN(__cdecl *TProcessCtrl_OneRead)(HANDLE ProcessHandle, DWORD Address, void *Data, DWORD Size) = (LPVOID)0x004A6FF4;

extern const DWORD F004A61F8;

__declspec(naked) LPMODULEENTRY32A __fastcall TProcessCtrl_GetModuleFromName(TProcessCtrl *this, LPCSTR Name)
{
	__asm
	{
		#define sizeof_string 24

		sub     esp, sizeof_string
		mov     eax, ecx
		mov     ecx, esp
		push    eax
		call    string_ctor_assign_cstr
		call    dword ptr [F004A61F8]
		add     esp, 4 + sizeof_string
		ret

		#undef sizeof_string
	}
}
