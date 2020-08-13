#include <stdint.h>
#include <windows.h>
#include <dhcpsapi.h>
#include "intrinsic.h"
#define USING_NAMESPACE_BCB6_STD
#include "TProcessAccessElementBase.h"

EXTERN_C void __cdecl Caller_StrToProcessAccessElementVecWithoutRel();
EXTERN_C void __cdecl Caller_ParsingWithRel();
EXTERN_C void __cdecl Caller_FixLoopByteArray();
EXTERN_C void __cdecl TSSGCtrl_StrToProcessAccessElementVec_MakeLoopSet();
EXTERN_C void __cdecl TSSGCtrl_MakeDataCode_MakeLoopSet();

static DWORD_DWORD __fastcall TProcessAccessElementLoop_dtorFix(
	const TProcessAccessElementLoop *const Loop,
	const size_t surplusVec_capInBytes)
{
	for (TProcessAccessElementBase **it = (void *)vector_begin(&Loop->surplusVec);
		 it < (TProcessAccessElementBase **)vector_end(&Loop->surplusVec);
		 it++)
		delete_TProcessAccessElement(*it);
	for (TProcessAccessElementBase **it = (void *)vector_begin(&Loop->loopVec);
		 it < (TProcessAccessElementBase **)vector_end(&Loop->loopVec);
		 it++)
		delete_TProcessAccessElement(*it);
	return (DWORD_DWORD) { (DWORD)vector_begin(&Loop->surplusVec),
		(ptrdiff_t)surplusVec_capInBytes < 0 ? surplusVec_capInBytes + 3 : surplusVec_capInBytes };
}

#define PUSH_IMM8 (BYTE )0x6A
#define CALL_REL  (BYTE )0xE8
#define JMP_REL32 (BYTE )0xE9
#define NOP       (BYTE )0x90
#define NOP_X4    (DWORD)0x00401F0F

EXTERN_C void __cdecl Attach_FixLoopByteArray()
{
	// TSSGCtrl::StrToProcessAccessElementVec
	*(LPBYTE )0x00506F78 = JMP_REL32;
	*(LPDWORD)0x00506F79 = (DWORD)Caller_StrToProcessAccessElementVecWithoutRel - (0x00506F79 + sizeof(DWORD));
	*(LPDWORD)0x00506F7D = NOP_X4;

	// TSSGCtrl::StrToProcessAccessElementVec
	//   new TProcessAccessElementJump() => new TProcessAccessElementBase()
	*(LPBYTE )(0x00508433 + 1) = sizeof(TProcessAccessElementBase);
	*(LPBYTE )(0x00508466 + 0) = JMP_REL32;
	*(LPDWORD)(0x00508466 + 1) = 0x0050848B - (0x00508466 + 1 + sizeof(DWORD));

	// TSSGCtrl::StrToProcessAccessElementVec
	*(LPBYTE )(0x0050AEF8 + 1) = 0x00;
#if 0
	*(LPBYTE )0x0050AF9C = JMP_REL32;
	*(LPDWORD)0x0050AF9D = (DWORD)Caller_FixLoopByteArray - (0x0050AF9D + sizeof(DWORD));
	*(LPBYTE )0x0050AFA1 = NOP;
#else
	*(LPBYTE )(0x0050AFE1 + 3) = 0x05;
	*(LPDWORD) 0x0050AFE8 = BSWAP32(0xFF752C << 8 | JMP_REL32);
	*(LPDWORD) 0x0050AFEC = 0x0050B207 - (0x0050AFEC + sizeof(DWORD));
	*(LPBYTE ) 0x0050AFF0 = NOP;

	*(LPDWORD)(0x0050B224 + 1) = (DWORD)TSSGCtrl_StrToProcessAccessElementVec_MakeLoopSet - (0x0050B224 + 1 + sizeof(DWORD));
	*(LPBYTE )(0x0050B229 + 2) = 0x00;
#endif

	// TSSGCtrl::StrToProcessAccessElementVec
	*(LPBYTE )0x0050B4D4 = JMP_REL32;
	*(LPDWORD)0x0050B4D5 = (DWORD)Caller_ParsingWithRel - (0x0050B4D5 + sizeof(DWORD));

	// TSSGCtrl::MakeDataCode
	*(LPBYTE )(0x0050F886 + 3) = 0x05;
	*(LPBYTE ) 0x0050F88D = PUSH_IMM8;
	*(LPWORD ) 0x0050F88E = JMP_REL32 << 8;
	*(LPDWORD) 0x0050F890 = 0x0050FA2D - (0x0050F890 + sizeof(DWORD));

	*(LPDWORD)(0x0050FB7E + 1) = (DWORD)TSSGCtrl_MakeDataCode_MakeLoopSet - (0x0050FB7E + 1 + sizeof(DWORD));
	*(LPBYTE )(0x0050FB83 + 2) = 0x00;

	// TProcessAccessElementLoop::dtor
	*(LPBYTE )0x00522C19 = CALL_REL;
	*(LPDWORD)0x00522C1A = (DWORD)TProcessAccessElementLoop_dtorFix - (0x00522C1A + sizeof(DWORD));

}
