#include <windows.h>
#include <intrin.h>
#define USING_NAMESPACE_BCB6_STD
#include "TSSGCtrl.h"

#pragma intrinsic(_ReturnAddress)

static unsigned long __fastcall TSSGCtrl_GetAddress_SaveAddress(TSSGSubject *const SSGS, LPCVOID const Address)
{
	switch ((uintptr_t)_ReturnAddress())
	{
	case 0x004B90F0u:// TSSBitList::Read
	case 0x004BACF7u:// TSSBitList::Write
	case 0x004BD73Cu:// TSSBundleCalc::Read       => (Offset)Address
	case 0x004BE081u:// TSSBundleCalc::Write      => (Offset)Address
	case 0x004BF41Bu:// TSSBundleList::Read       => (Offset)Address
	case 0x004BF734u:// TSSBundleList::Write      => (Offset)Address
	case 0x004C0204u:// TSSBundleToggle::Read     => OffsetAddress
	case 0x004C0C00u:// TSSBundleToggle::Write    => OffsetAddress
	case 0x004C1EACu:// TSSCalc::Read
	case 0x004C206Cu:// TSSCalc::Write
	case 0x004C2B51u:// TSSCopy::Write            => (!Src)Address
	case 0x004C4217u:// TSSDoubleList::Read       => OffsetAddress
	case 0x004C535Fu:// TSSDoubleList::Write      => OffsetAddress
	case 0x004C6547u:// TSSDoubleList::ToByteCode => OffsetAddress
	case 0x004C866Fu:// TSSDoubleToggle::Read     => OffsetAddress
	case 0x004CAF0Eu:// TSSDoubleToggle::Write    => OffsetAddress
	case 0x004CE3C0u:// TSSFloatCalc::Read
	case 0x004CE598u:// TSSFloatCalc::Write
	case 0x0052A110u:// TSSList::Read
	case 0x0052A2B4u:// TSSList::Write
	case 0x0052AFA8u:// TSSString::Read
	case 0x0052B2ADu:// TSSString::Write
	case 0x0052BE87u:// TSSToggle::Setting        => *StartAddress
	case 0x0052C4BAu:// TSSToggle::Read
	case 0x0052C79Au:// TSSToggle::Write
	case 0x0052DD70u:// TSSBundleFloatCalc::Read  => (Offset)Address
	case 0x0052E724u:// TSSBundleFloatCalc::Write => (Offset)Address
		SSGS->lastAddr = Address;
		break;
	}
	return (uintptr_t)Address;
}

__declspec(naked) unsigned long __cdecl Caller_TSSGCtrl_GetAddress_SaveAddress(TSSGCtrl *this, TSSGSubject *SSGS, const string *AddressStr, unsigned long Mode)
{
	__asm
	{
		mov     edx, eax
		mov     ecx, dword ptr [esp + 8]
		jmp     TSSGCtrl_GetAddress_SaveAddress
	}
}
