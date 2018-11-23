#define USING_NAMESPACE_BCB6
#define USING_NAMESPACE_BCB6_STD
#include "TSSString.h"
#include "bcb6_operator.h"
#include "SSGSubjectProperty.h"

__inline void TSSString_ctor(TSSString *this)
{
	memset(this, 0, sizeof(TSSString));
	this->super.VTable = (void *)0x00640204;
	this->super.type = stSTRING;
	AppendSubjectProperty(&this->super);
}

TSSString * __cdecl new_TSSString()
{
	TSSString *this = operator_new(sizeof(TSSString));
	TSSString_ctor(this);
	return this;
}

void __fastcall delete_TSSString(TSSString *this)
{
	operator_delete(this);
}

