#include <windows.h>
#include "TMainForm.h"

#define _BSWAP16(value) (           \
    (((value) >> 8) & 0x000000FF) | \
    (((value) << 8) & 0x0000FF00))

#define _BSWAP32(value) (            \
    (((value) >> 24) & 0x000000FF) | \
    (((value) >>  8) & 0x0000FF00) | \
    (((value) <<  8) & 0x00FF0000) | \
    (((value) << 24) & 0xFF000000))

void __stdcall TMainForm_HotKeyEditKeyDown_PageUp(TMainForm *this);
void __stdcall TMainForm_HotKeyEditKeyDown_PageDown(TMainForm *this);
void __stdcall TMainForm_HotKeyEditKeyDown_End(TMainForm *this);
void __stdcall TMainForm_HotKeyEditKeyDown_Home(TMainForm *this);
void __stdcall TMainForm_HotKeyEditKeyDown_Delete(TMainForm *this);

__declspec(naked) void __cdecl TMainForm_HotKeyEditKeyDown_SwitchKey()
{
	#define ReturnAddress 0x00443484

	static const LPVOID JumpTable[] = {
		(LPVOID)TMainForm_HotKeyEditKeyDown_PageUp,     // VK_PRIOR  0x21
		(LPVOID)TMainForm_HotKeyEditKeyDown_PageDown,   // VK_NEXT   0x22
		(LPVOID)TMainForm_HotKeyEditKeyDown_End,        // VK_END    0x23
		(LPVOID)TMainForm_HotKeyEditKeyDown_Home,       // VK_HOME   0x24
		(LPVOID)ReturnAddress,                          // VK_LEFT   0x25
		(LPVOID)0x004431F7,                             // VK_UP     0x26
		(LPVOID)0x0044333B,                             // VK_RIGHT  0x27
		(LPVOID)0x00443267,                             // VK_DOWN   0x28
	};

	__asm
	{
		#define this          ebx
		#define return        (JumpTable + (4 * 4))
		#define case_VK_RIGHT (JumpTable + (4 * 6))

		cmp     cx, VK_DELETE - VK_SPACE
		ja      L5
		cmp     cx, VK_HOME - VK_SPACE
		ja      L1
		and     ecx, 7
		push    this
		push    ReturnAddress
		jmp     dword ptr [(JumpTable - 4) + ecx * 4]

		align   16
	L1:
		cmp     cx, VK_DOWN - VK_SPACE
		ja      L4
		cmp     cx, VK_RIGHT - VK_SPACE
		je      L2
		and     ecx, 15
		jmp     dword ptr [(JumpTable - 4) + ecx * 4]

		align   16
	L2:
		/*
		HWND hWnd;
		char lpClassName[5 + 1 + 1];

		if (hWnd = GetFocus())
			if (GetClassNameA(hWnd, lpClassName, _countof(lpClassName)) == 5)
				if (*(LPDWORD)lpClassName == BSWAP32('TEdi') && *(LPWORD)&lpClassName[4] == BSWAP16('t\0'))
					return;
		*/
		call    GetFocus
		test    eax, eax
		jz      L3
		sub     esp, 8
		mov     ecx, esp
		push    5 + 1 + 1
		push    ecx
		push    eax
		call    GetClassNameA
		pop     ecx
		pop     edx
		cmp     eax, 5
		jne     L3
		cmp     ecx, _BSWAP32('TEdi')
		jne     L3
		cmp     dx, _BSWAP16('t\0')
		je      L5
	L3:
		jmp     dword ptr [case_VK_RIGHT]

		align   16
	L4:
		cmp     cx, VK_DELETE - VK_SPACE
		jne     L5
		push    this
		push    ReturnAddress
		jmp     TMainForm_HotKeyEditKeyDown_Delete

		align   16
	L5:
		jmp     dword ptr [return]

		#undef this
		#undef return
		#undef case_VK_RIGHT
	}

	#undef ReturnAddress
}
