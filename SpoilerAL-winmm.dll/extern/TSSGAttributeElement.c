#define USING_NAMESPACE_BCB6
#define USING_NAMESPACE_BCB6_STD
#include "TSSGAttributeElement.h"
#include "bcb6_operator.h"

#pragma function(memcmp, strlen)

#pragma warning(disable:4733)

void(__cdecl * const TReplaceAttribute_Setting)(TReplaceAttribute *, TStringDivision *, const char *) = (LPVOID)0x004CEA24;
void(__cdecl * const TIO_FEPAttribute_Setting)(TIO_FEPAttribute *this, TStringDivision *StrD, const char *Code) = (LPVOID)0x004CEEE4;
void(__cdecl * const TAdjustCheckAttribute_Setting)(TAdjustCheckAttribute *this, TStringDivision *StrD, const char *Code) = (LPVOID)0x004CF6A0;

__inline void TSSGAttributeElement_ctor(TSSGAttributeElement *this)
{
	this->VTable = TSSGAttributeElement_VTable;
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
	this->VTable = TDirAttribute_VTable;
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
	this->VTable = TSizeAttribute_VTable;
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
	this->VTable = TReplaceAttribute_VTable;
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
	this->VTable = TFunnelAttribute_VTable;
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
	this->VTable = TIO_FEPAttribute_VTable;
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
	this->VTable = TEndWithAttribute_VTable;
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
	this->VTable = TEnabledAttribute_VTable;
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
	this->VTable = TChildRWAttribute_VTable;
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

void __cdecl TChildRWAttribute_Setting(TChildRWAttribute *this, LPVOID Reserved, const char *Code)
{
	char c;

	this->prohibit = 0;
	while ((c = *(Code++)) == ' ' || c == '\t');
	if (c         != 'f' ||
	    *(Code++) != 'a' ||
	    *(Code++) != 'l' ||
	    *(Code++) != 's' ||
	    *(Code++) != 'e')
		return;
	while ((c = *(Code++)) == ' ' || c == '\t');
	if (!c)
		this->prohibit = 1;
}

__inline void TCautionAttribute_ctor(TCautionAttribute *this)
{
	this->VTable = TCautionAttribute_VTable;
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
	this->VTable = TAdjustCheckAttribute_VTable;
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
