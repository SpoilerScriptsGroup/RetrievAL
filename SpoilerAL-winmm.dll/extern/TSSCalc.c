#define USING_NAMESPACE_BCB6
#define USING_NAMESPACE_BCB6_STD
#include "TSSCalc.h"
#include "bcb6_operator.h"
#include "SSGSubjectProperty.h"

__inline void TSSCalc_ctor(TSSCalc *this)
{
	memset(this, 0, sizeof(TSSCalc));
	this->super.VTable = (void *)0x006402DC;
	this->super.type = stCALC;
	AppendSubjectProperty(&this->super);
}

TSSCalc * __cdecl new_TSSCalc()
{
	TSSCalc *this = operator_new(sizeof(TSSCalc));
	TSSCalc_ctor(this);
	return this;
}

void __fastcall delete_TSSCalc(TSSCalc *this)
{
	operator_delete(this);
}

