#include <windows.h>
#include "intrinsic.h"
#include "bcb6_std_string.h"
#include "bcb6_std_vector.h"
#include "TSSGCtrl.h"
#include "TSSGSubject.h"

unsigned long __cdecl Parsing(IN TSSGCtrl *_this, IN TSSGSubject *SSGS, IN const bcb6_std_string *Src, ...);

static unsigned long __stdcall TSSGCtrl_GetSSGDataFile_Parsing(
	IN  TSSGCtrl        *_this,
	IN  TSSGSubject     *SSGS,
	IN  bcb6_std_vector *tmpV,
	IN  unsigned long   Address,
	OUT unsigned long   *RowSize,
	OUT unsigned long   *StepSize);

__declspec(naked) void __cdecl Caller_TSSGCtrl_GetSSGDataFile_Parsing()
{
	__asm
	{
		#define IsNocache esp
		#define _this     (ebp + 8H)
		#define SSGS      edi
		#define tmpV      (ebp - 30H)
		#define Address   (ebp - 2FCH)
		#define StrSize   esi
		#define RowSize   (ebp - 300H)
		#define StepSize  (ebp - 308H)

		mov     eax, dword ptr [IsNocache]
		lea     ecx, [StepSize]
		test    eax, eax
		jnz     L1
		mov     eax, 004EE312H
		push    1
		mov     ecx, dword ptr [tmpV]
		jmp     eax
		align   16
	L1:
		push    ecx
		lea     eax, [RowSize]
		push    eax
		mov     eax, dword ptr [Address]
		push    eax
		mov     eax, dword ptr [tmpV]
		push    eax
		mov     eax, dword ptr [_this]
		push    SSGS
		push    eax
		call    TSSGCtrl_GetSSGDataFile_Parsing
		mov     ecx, 004EE34EH
		mov     StrSize, eax
		jmp     ecx

		#undef IsNocache
		#undef _this
		#undef SSGS
		#undef tmpV
		#undef Address
		#undef StrSize
		#undef RowSize
		#undef StepSize
	}
}

static unsigned long __stdcall TSSGCtrl_GetSSGDataFile_Parsing(
	IN  TSSGCtrl        *_this,
	IN  TSSGSubject     *SSGS,
	IN  bcb6_std_vector *tmpV,
	IN  unsigned long   Address,
	OUT unsigned long   *RowSize,
	OUT unsigned long   *StepSize)
{
	LPCSTR        VariableName = "List";
	unsigned long StrSize;

	StrSize = Parsing(_this, SSGS, (bcb6_std_string *)tmpV->_M_start + 2, 4, VariableName, Address, 0);
	*RowSize = Parsing(_this, SSGS, (bcb6_std_string *)tmpV->_M_start + 3, 4, VariableName, Address, 0);
	*StepSize = Parsing(_this, SSGS, (bcb6_std_string *)tmpV->_M_start + 4, 4, VariableName, Address, 0);
	return StrSize;
}
