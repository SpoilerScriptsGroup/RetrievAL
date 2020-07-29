#include <windows.h>
#include <intrin.h>
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"
#include "TSSGCtrl.h"
#include "TSSGSubject.h"

#pragma intrinsic(_ReturnAddress)

static unsigned long __cdecl TSSGCtrl_GetAddress_SaveAddress(LPCVOID Result, TSSGSubject *SSGS, const string *AddressStr, unsigned long Mode)
{
	switch ((intptr_t)_ReturnAddress())
	{
	case 0x004B90F0:// TSSBitList::Read
	case 0x004BACF7:// TSSBitList::Write
	case 0x004BD73C:// TSSBundleCalc::Read       => (Offset)Address
	case 0x004BE081:// TSSBundleCalc::Write      => (Offset)Address
	case 0x004BF41B:// TSSBundleList::Read       => (Offset)Address
	case 0x004BF734:// TSSBundleList::Write      => (Offset)Address
	case 0x004C0204:// TSSBundleToggle::Read     => OffsetAddress
	case 0x004C0C00:// TSSBundleToggle::Write    => OffsetAddress
	case 0x004C1EAC:// TSSCalc::Read
	case 0x004C206C:// TSSCalc::Write
	case 0x004C2B51:// TSSCopy::Write            => (!Src)Address
	case 0x004C4217:// TSSDoubleList::Read       => OffsetAddress
	case 0x004C535F:// TSSDoubleList::Write      => OffsetAddress
	case 0x004C6547:// TSSDoubleList::ToByteCode => OffsetAddress
	case 0x004C866F:// TSSDoubleToggle::Read     => OffsetAddress
	case 0x004CAF0E:// TSSDoubleToggle::Write    => OffsetAddress
	case 0x004CE3C0:// TSSFloatCalc::Read
	case 0x004CE598:// TSSFloatCalc::Write
	case 0x0052A110:// TSSList::Read
	case 0x0052A2B4:// TSSList::Write
	case 0x0052AFA8:// TSSString::Read
	case 0x0052B2AD:// TSSString::Write
	case 0x0052BE87:// TSSToggle::Setting        => *StartAddress
	case 0x0052C4BA:// TSSToggle::Read
	case 0x0052C79A:// TSSToggle::Write
	case 0x0052DD70:// TSSBundleFloatCalc::Read  => (Offset)Address
	case 0x0052E724:// TSSBundleFloatCalc::Write => (Offset)Address
		SSGS->lastAddr = Result;
		break;
	}
	return (uintptr_t)Result;
}

__declspec(naked) unsigned long __cdecl Caller_TSSGCtrl_GetAddress_SaveAddress(TSSGCtrl *this, TSSGSubject *SSGS, const string *AddressStr, unsigned long Mode)
{
	__asm
	{
		pop     ebp
		mov     dword ptr [esp + 4], eax
		mov     edx, eax
		mov     ecx, dword ptr [esp + 8]
		jmp     TSSGCtrl_GetAddress_SaveAddress
	}
}
