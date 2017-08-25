#include "TSSGCtrl.h"

__declspec(naked) void __cdecl TSSGCtrl_Write_ErrorSkip()
{
	__asm
	{
		#define _this                       ebx
		#define SSGS                        (ebp + 0CH)
		#define ssgActionListner            (ebx + 54H)
		#define OldAddress                  edi
		#define result                      esi
		#define AT_ERRORSKIP                2000H
		#define MainForm                    0064CE2CH
		#define offsetof_TMainForm_userMode 448H

		mov     ecx, dword ptr [SSGS]
		push    AT_ERRORSKIP
		push    ecx
		push    _this
		call    dword ptr [TSSGCtrl_GetAttribute]
		add     esp, 12
		test    eax, eax
		mov     eax, MainForm
		jz      L1
		mov     eax, dword ptr [eax]
		xor     esi, esi
		cmp     dword ptr [eax + offsetof_TMainForm_userMode], 3
		je      L2
	L1:
		mov     eax, 0051C45AH
		mov     edx, dword ptr [SSGS]
		push    OldAddress
		push    edx
		jmp     eax
	L2:
		mov     eax, 0051C466H
		jmp     eax

		#undef _this
		#undef SSGS
		#undef ssgActionListner
		#undef OldAddress
		#undef result
		#undef AT_ERRORSKIP
		#undef MainForm
		#undef offsetof_TMainForm_userMode
	}
}
