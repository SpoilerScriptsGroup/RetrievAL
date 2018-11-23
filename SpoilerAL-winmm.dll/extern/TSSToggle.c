#define USING_NAMESPACE_BCB6
#define USING_NAMESPACE_BCB6_STD
#include "TSSToggle.h"
#include "bcb6_operator.h"
#include "SSGSubjectProperty.h"

__inline void TSSToggle_ctor(TSSToggle *this)
{
	memset(this, 0, sizeof(TSSToggle));
	this->super.VTable = (void *)0x0064023C;
	this->super.type = stTOGGLE;
	AppendSubjectProperty(&this->super);
}

TSSToggle * __cdecl new_TSSToggle()
{
	TSSToggle *this = operator_new(sizeof(TSSToggle));
	TSSToggle_ctor(this);
	return this;
}

void __fastcall delete_TSSToggle(TSSToggle *this)
{
	operator_delete(this);
}

