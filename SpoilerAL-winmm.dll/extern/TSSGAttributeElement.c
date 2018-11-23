#define USING_NAMESPACE_BCB6
#define USING_NAMESPACE_BCB6_STD
#include "TSSGAttributeElement.h"
#include "bcb6_operator.h"

void(__cdecl * const TReplaceAttribute_Setting)(TReplaceAttribute *, TStringDivision *, const char *) = (LPVOID)0x004CEA24;

__inline void TSSGAttributeElement_ctor(TSSGAttributeElement *this)
{
	this->VTable = (void *)0x006151C0;
	this->type = 0;
}

TSSGAttributeElement * __cdecl new_TSSGAttributeElement()
{
	TSSGAttributeElement *this = operator_new(sizeof(TSSGAttributeElement));
	TSSGAttributeElement_ctor(this);
	return this;
}

void __fastcall delete_TSSGAttributeElement(TSSGAttributeElement *this)
{
	operator_delete(this);
}

__inline void TDirAttribute_ctor(TDirAttribute *this)
{
	this->VTable = (void *)0x006403A8;
	this->type = atDIR_LEVEL;
	this->level = 0;
}

TDirAttribute * __cdecl new_TDirAttribute()
{
	TDirAttribute *this = operator_new(sizeof(TDirAttribute));
	TDirAttribute_ctor(this);
	return this;
}

void __fastcall delete_TDirAttribute(TDirAttribute *this)
{
	operator_delete(this);
}

__inline void TSizeAttribute_ctor(TSizeAttribute *this)
{
	this->VTable = (void *)0x00640378;
	this->type = atSIZE;
	this->size = 0;
}

TSizeAttribute * __cdecl new_TSizeAttribute()
{
	TSizeAttribute *this = operator_new(sizeof(TSizeAttribute));
	TSizeAttribute_ctor(this);
	return this;
}

void __fastcall delete_TSizeAttribute(TSizeAttribute *this)
{
	operator_delete(this);
}

__inline void TReplaceAttribute_ctor(TReplaceAttribute *this)
{
	this->VTable = (void *)0x00640390;
	this->type = atREPLACE;
	string_ctor(&this->offsetCode);
	string_ctor(&this->fileName);
	this->offsetNum = 0;
}

TReplaceAttribute * __cdecl new_TReplaceAttribute()
{
	TReplaceAttribute *this = operator_new(sizeof(TReplaceAttribute));
	TReplaceAttribute_ctor(this);
	return this;
}

__inline void TReplaceAttribute_dtor(TReplaceAttribute *this)
{
	string_dtor(&this->fileName);
	string_dtor(&this->offsetCode);
}

void __fastcall delete_TReplaceAttribute(TReplaceAttribute *this)
{
	TReplaceAttribute_dtor(this);
	operator_delete(this);
}

__inline void TFunnelAttribute_ctor(TFunnelAttribute *this)
{
	this->VTable = (void *)0x0064033C;
	this->type = atFUNNEL;
	string_ctor(&this->fileName);
}

TFunnelAttribute * __cdecl new_TFunnelAttribute()
{
	TFunnelAttribute *this = operator_new(sizeof(TFunnelAttribute));
	TFunnelAttribute_ctor(this);
	return this;
}

__inline void TFunnelAttribute_dtor(TFunnelAttribute *this)
{
	string_dtor(&this->fileName);
}

void __fastcall delete_TFunnelAttribute(TFunnelAttribute *this)
{
	TFunnelAttribute_dtor(this);
	operator_delete(this);
}

__inline void TIO_FEPAttribute_ctor(TIO_FEPAttribute *this)
{
	this->VTable = (void *)0x00640324;
	this->type = atIO_FEP;
	string_ctor(&this->inputCode);
	string_ctor(&this->outputCode);
}

TIO_FEPAttribute * __cdecl new_TIO_FEPAttribute()
{
	TIO_FEPAttribute *this = operator_new(sizeof(TIO_FEPAttribute));
	TIO_FEPAttribute_ctor(this);
	return this;
}

__inline void TIO_FEPAttribute_dtor(TIO_FEPAttribute *this)
{
	string_dtor(&this->outputCode);
	string_dtor(&this->inputCode);
}

void __fastcall delete_TIO_FEPAttribute(TIO_FEPAttribute *this)
{
	TIO_FEPAttribute_dtor(this);
	operator_delete(this);
}

__inline void TEndWithAttribute_ctor(TEndWithAttribute *this)
{
	this->VTable = (void *)0x0064030C;
	this->type = atE_WITH;
	string_ctor(&this->code);
}

TEndWithAttribute * __cdecl new_TEndWithAttribute()
{
	TEndWithAttribute *this = operator_new(sizeof(TEndWithAttribute));
	TEndWithAttribute_ctor(this);
	return this;
}

__inline void TEndWithAttribute_dtor(TEndWithAttribute *this)
{
	string_dtor(&this->code);
}

void __fastcall delete_TEndWithAttribute(TEndWithAttribute *this)
{
	TEndWithAttribute_dtor(this);
	operator_delete(this);
}

__inline void TEnabledAttribute_ctor(TEnabledAttribute *this)
{
	this->VTable = (void *)0x006402F4;
	this->type = atENABLED;
	string_ctor(&this->code);
}

TEnabledAttribute * __cdecl new_TEnabledAttribute()
{
	TEnabledAttribute *this = operator_new(sizeof(TEnabledAttribute));
	TEnabledAttribute_ctor(this);
	return this;
}

__inline void TEnabledAttribute_dtor(TEnabledAttribute *this)
{
	string_dtor(&this->code);
}

void __fastcall delete_TEnabledAttribute(TEnabledAttribute *this)
{
	TEnabledAttribute_dtor(this);
	operator_delete(this);
}

__inline void TChildRWAttribute_ctor(TChildRWAttribute *this)
{
	this->VTable = (void *)0x006402C4;
	this->type = atCHILD_RW;
	this->prohibit = 0;
}

TChildRWAttribute * __cdecl new_TChildRWAttribute()
{
	TChildRWAttribute *this = operator_new(sizeof(TChildRWAttribute));
	TChildRWAttribute_ctor(this);
	return this;
}

void __fastcall delete_TChildRWAttribute(TChildRWAttribute *this)
{
	operator_delete(this);
}

__inline void TCautionAttribute_ctor(TCautionAttribute *this)
{
	this->VTable = (void *)0x006402AC;
	this->type = atCAUTION;
	string_ctor(&this->fileName);
}

TCautionAttribute * __cdecl new_TCautionAttribute()
{
	TCautionAttribute *this = operator_new(sizeof(TCautionAttribute));
	TCautionAttribute_ctor(this);
	return this;
}

__inline void TCautionAttribute_dtor(TCautionAttribute *this)
{
	string_dtor(&this->fileName);
}

void __fastcall delete_TCautionAttribute(TCautionAttribute *this)
{
	TCautionAttribute_dtor(this);
	operator_delete(this);
}

__inline void TAdjustCheckAttribute_ctor(TAdjustCheckAttribute *this)
{
	this->VTable = (void *)0x006402DC;
	this->type = atADJUST_CHECK;
	this->check = TRUE;
	this->mustCheck = FALSE;
}

TAdjustCheckAttribute * __cdecl new_TAdjustCheckAttribute()
{
	TAdjustCheckAttribute *this = operator_new(sizeof(TAdjustCheckAttribute));
	TAdjustCheckAttribute_ctor(this);
	return this;
}

void __fastcall delete_TAdjustCheckAttribute(TAdjustCheckAttribute *this)
{
	operator_delete(this);
}
