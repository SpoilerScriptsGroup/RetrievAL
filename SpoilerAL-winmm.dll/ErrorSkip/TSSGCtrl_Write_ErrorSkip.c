#include "TSSGCtrl.h"
#include "TMainForm.h"

__declspec(naked) void __cdecl TSSGCtrl_Write_ErrorSkip()
{
	__asm
	{
		#define this                        ebx
		#define SSGS                        (ebp + 0CH)
		#define ssgActionListner            (ebx + 54H)
		#define OldAddress                  edi
		#define result                      esi
		#define AT_ERRORSKIP                2000H
		#define offsetof_TMainForm_userMode 448H

		mov     ecx, dword ptr [SSGS]
		push    AT_ERRORSKIP
		push    ecx
		push    this
		call    dword ptr [TSSGCtrl_GetAttribute]
		add     esp, 12
		test    eax, eax
		mov     eax, 0051C466H
		jz      L1
		mov     ecx, dword ptr ds:[_MainForm]
		xor     esi, esi
		cmp     dword ptr [ecx + offsetof_TMainForm_userMode], 3
		je      L2
	L1:
		mov     eax, 0051C45AH
		mov     edx, dword ptr [SSGS]
		push    OldAddress
		push    edx
	L2:
		jmp     eax

		#undef this
		#undef SSGS
		#undef ssgActionListner
		#undef OldAddress
		#undef result
		#undef AT_ERRORSKIP
		#undef offsetof_TMainForm_userMode
	}
}
