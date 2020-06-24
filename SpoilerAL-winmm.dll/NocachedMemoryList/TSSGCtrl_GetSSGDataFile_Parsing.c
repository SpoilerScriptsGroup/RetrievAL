#include <windows.h>
#include "intrinsic.h"
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_vector_string.h"
#include "TSSGCtrl.h"
#include "TSSGSubject.h"

unsigned long __cdecl Parsing(IN TSSGCtrl *this, IN TSSGSubject *SSGS, IN const string *Src, ...);

static unsigned long __fastcall TSSGCtrl_GetSSGDataFile_Parsing(
	IN  TSSGCtrl      *this,
	IN  TSSGSubject   *SSGS,
	IN  string        *tmpV,
	IN  unsigned long Address,
	OUT unsigned long *RowSize,
	OUT unsigned long *StepSize)
{
	static LPCSTR const VariableName = "List";
	unsigned long StrSize;

	StrSize   = Parsing(this, SSGS, &tmpV[2], 4, VariableName, (uint64_t)Address, 0);
	*RowSize  = Parsing(this, SSGS, &tmpV[3], 4, VariableName, (uint64_t)Address, 0);
	*StepSize = Parsing(this, SSGS, &tmpV[4], 4, VariableName, (uint64_t)Address, 0);
	return StrSize;
}

__declspec(naked) void __cdecl Caller_TSSGCtrl_GetSSGDataFile_Parsing()
{
	extern BOOL FixTheProcedure;

	static const DWORD X004EE312 = 0x004EE312;
	static const DWORD X004EE34E = 0x004EE34E;

	__asm
	{
		#define IsNocache (ebp - 304H)
		#define this      (ebp + 8H)
		#define SSGS      edi
		#define tmpV      (ebp - 30H)
		#define Address   (ebp - 2FCH)
		#define StrSize   esi
		#define RowSize   (ebp - 300H)
		#define StepSize  (ebp - 308H)

		mov     eax, dword ptr [IsNocache]
		or      eax, FixTheProcedure
		jz      L1
		lea     eax, [StepSize]
		push    eax
		lea     eax, [RowSize]
		push    eax
		push    dword ptr [Address]
		push    dword ptr [tmpV]
		mov     edx, SSGS
		mov     ecx, dword ptr [this]
		call    TSSGCtrl_GetSSGDataFile_Parsing
		mov     StrSize, eax
		jmp     dword ptr [X004EE34E]

		align   16
	L1:
		push    1
		mov     ecx, dword ptr [tmpV]
		jmp     dword ptr [X004EE312]

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
