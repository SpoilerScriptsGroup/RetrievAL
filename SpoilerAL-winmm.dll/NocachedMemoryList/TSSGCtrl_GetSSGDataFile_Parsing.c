#include <windows.h>
#include "intrinsic.h"
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"
#include "bcb6_std_vector.h"
#include "TSSGCtrl.h"
#include "TSSGSubject.h"

unsigned long __cdecl Parsing(IN TSSGCtrl *this, IN TSSGSubject *SSGS, IN const string *Src, ...);

static unsigned long __stdcall TSSGCtrl_GetSSGDataFile_Parsing(
	IN  TSSGCtrl      *this,
	IN  TSSGSubject   *SSGS,
	IN  vector        *tmpV,
	IN  unsigned long Address,
	OUT unsigned long *RowSize,
	OUT unsigned long *StepSize);

__declspec(naked) void __cdecl Caller_TSSGCtrl_GetSSGDataFile_Parsing()
{
	__asm
	{
		#define IsNocache esp
		#define this      (ebp + 8H)
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
		lea     eax, [tmpV]
		push    eax
		mov     eax, dword ptr [this]
		push    SSGS
		push    eax
		call    TSSGCtrl_GetSSGDataFile_Parsing
		mov     ecx, 004EE34EH
		mov     StrSize, eax
		jmp     ecx

		#undef IsNocache
		#undef this
		#undef SSGS
		#undef tmpV
		#undef Address
		#undef StrSize
		#undef RowSize
		#undef StepSize
	}
}

static unsigned long __stdcall TSSGCtrl_GetSSGDataFile_Parsing(
	IN  TSSGCtrl      *this,
	IN  TSSGSubject   *SSGS,
	IN  vector        *tmpV,
	IN  unsigned long Address,
	OUT unsigned long *RowSize,
	OUT unsigned long *StepSize)
{
	LPCSTR        VariableName = "List";
	unsigned long StrSize;

	StrSize = Parsing(this, SSGS, (string *)tmpV->_M_start + 2, 4, VariableName, (QWORD)Address, 0);
	*RowSize = Parsing(this, SSGS, (string *)tmpV->_M_start + 3, 4, VariableName, (QWORD)Address, 0);
	*StepSize = Parsing(this, SSGS, (string *)tmpV->_M_start + 4, 4, VariableName, (QWORD)Address, 0);
	return StrSize;
}
