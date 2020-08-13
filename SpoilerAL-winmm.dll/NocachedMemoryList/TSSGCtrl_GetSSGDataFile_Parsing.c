#include <windows.h>
#include <dhcpsapi.h>
#include "intrinsic.h"
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_vector_string.h"
#include "TSSGCtrl.h"
#include "TSSGSubject.h"

extern unsigned long __cdecl Parsing(IN TSSGCtrl *this, IN TSSGSubject *SSGS, IN const string *Src, ...);

static DWORD_DWORD __fastcall TSSGCtrl_GetSSGDataFile_Parsing(
	OUT unsigned long *RowSize,
	OUT unsigned long *StepSize,
	IN  unsigned long  Address,
	IN  const string  *tmpV,
	IN  TSSGCtrl      *SSGC,
	IN  TSSGSubject   *SSGS)
{
	static const char VarName[] = "List";
	unsigned     long StrSize;

	 StrSize  = Parsing(SSGC, SSGS, &tmpV[2], sizeof(VarName) - sizeof(*VarName), VarName, (uint64_t)Address, 0);
	*RowSize  = Parsing(SSGC, SSGS, &tmpV[3], sizeof(VarName) - sizeof(*VarName), VarName, (uint64_t)Address, 0);
	*StepSize = Parsing(SSGC, SSGS, &tmpV[4], sizeof(VarName) - sizeof(*VarName), VarName, (uint64_t)Address, 0);
	return (DWORD_DWORD) { Address, StrSize };// can be changed by return different value
}

__declspec(naked) uint64_t __cdecl Caller_TSSGCtrl_GetSSGDataFile_Parsing()
{
	extern BOOL FixTheProcedure;

	__asm
	{// Borland's fastcall like
		#define SSGS       edi
		#define this      (ebp + 0x08)
		#define tmpV      (ebp - 0x30)
		#define Address   (ebp - 0x02FC)
		#define StrSize    esi
		#define RowSize   (ebp - 0x0300)
		#define IsNocache (ebp - 0x0304)
		#define StepSize  (ebp - 0x0308)

		mov     edx, dword ptr [IsNocache]
		or      edx, FixTheProcedure
		neg     edx
	//	sbb     edx, edx
		and     edx, eax
		jz      L1

		push    SSGS
		push    dword ptr [this]
		push    dword ptr [tmpV]
		push    eax
		lea     edx, [StepSize]
		lea     ecx, [RowSize]
		call    TSSGCtrl_GetSSGDataFile_Parsing

	L1:
		mov     dword ptr [Address], eax
		ret

		#undef StepSize
		#undef IsNocache
		#undef RowSize
		#undef StrSize
		#undef Address
		#undef tmpV
		#undef this
		#undef SSGS
	}
}
