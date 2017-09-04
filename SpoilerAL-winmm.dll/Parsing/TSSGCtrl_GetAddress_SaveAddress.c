#include <windows.h>
#include <intrin.h>
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"
#include "TSSGCtrl.h"
#include "TSSGSubject.h"

#pragma intrinsic(_ReturnAddress)

unsigned long __cdecl TSSGCtrl_GetAddress_SaveAddress(TSSGCtrl *this, TSSGSubject *SSGS, const string *AddressStr, unsigned long Result);

__declspec(naked) unsigned long __cdecl Caller_TSSGCtrl_GetAddress_SaveAddress(TSSGCtrl *this, TSSGSubject *SSGS, const string *AddressStr, unsigned long Mode)
{
	__asm
	{
		mov     dword ptr [ebp + 14H], eax
		pop     ebp
		jmp     TSSGCtrl_GetAddress_SaveAddress
	}
}

unsigned long __cdecl TSSGCtrl_GetAddress_SaveAddress(TSSGCtrl *this, TSSGSubject *SSGS, const string *AddressStr, unsigned long Result)
{
	switch ((size_t)_ReturnAddress())
	{
	case 0x004B90F0:    // TSSBitList::Read
	case 0x004BACF7:    // TSSBitList::Write
	case 0x004BD73C:    // TSSBundleCalc::Read       -> (Offset)ReturnAddress
	case 0x004BE081:    // TSSBundleCalc::Write      -> (Offset)ReturnAddress
	case 0x004BF41B:    // TSSBundleList::Read       -> (Offset)ReturnAddress
	case 0x004BF734:    // TSSBundleList::Write      -> (Offset)ReturnAddress
	case 0x004C0204:    // TSSBundleToggle::Read     -> OffsetAddress
	case 0x004C0C00:    // TSSBundleToggle::Write    -> OffsetAddress
	case 0x004C1EAC:    // TSSCalc::Read
	case 0x004C206C:    // TSSCalc::Write
	case 0x004C2B51:    // TSSCopy::Write            -> ReturnAddress
	case 0x004C4217:    // TSSDoubleList::Read       -> OffsetAddress
	case 0x004C535F:    // TSSDoubleList::Write      -> OffsetAddress
	case 0x004C6547:    // TSSDoubleList::ToByteCode -> OffsetAddress
	case 0x004C866F:    // TSSDoubleToggle::Read     -> OffsetAddress
	case 0x004CAF0E:    // TSSDoubleToggle::Write    -> OffsetAddress
	case 0x004CE3C0:    // TSSFloatCalc::Read
	case 0x004CE598:    // TSSFloatCalc::Write
	case 0x0052A110:    // TSSList::Read
	case 0x0052A2B4:    // TSSList::Write
	case 0x0052AFA8:    // TSSString::Read
	case 0x0052B2AD:    // TSSString::Write
	case 0x0052BE87:    // TSSToggle::Setting
	case 0x0052C4BA:    // TSSToggle::Read
	case 0x0052C79A:    // TSSToggle::Write
	case 0x0052DD70:    // TSSBundleFloatCalc::Read  -> (Offset)ReturnAddress
	case 0x0052E724:    // TSSBundleFloatCalc::Write -> (Offset)ReturnAddress
		SSGS->address = (const BYTE *)Result;
#if 0//defined(_DEBUG)
		{
			char buf[0x100];

			OutputDebugStringA(_ultoa(Result, buf, 16));
			OutputDebugStringA(" ret ");
			OutputDebugStringA(_ultoa((size_t)_ReturnAddress(), buf, 16));
			OutputDebugStringA(" = ");
			OutputDebugStringA(AddressStr->_M_start);
			OutputDebugStringA(" ");
			OutputDebugStringA(_ultoa(Mode, buf, 16));
			OutputDebugStringA(" ");
			OutputDebugStringA(SSGS->name._M_start);
			OutputDebugStringA("\n");
		}
#endif
		break;
	}
	return Result;
}

