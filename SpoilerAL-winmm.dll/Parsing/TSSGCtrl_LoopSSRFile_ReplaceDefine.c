#include "bcb6_std_string.h"
#include "bcb6_std_vector.h"
#include "TSSGCtrl.h"
#include "TSSGAttributeSelector.h"

void __stdcall ReplaceDefine(TSSGAttributeSelector *attributeSelector, bcb6_std_string *Line);

void __stdcall  TSSGCtrl_LoopSSRFile_ReplaceDefine(TSSGCtrl *_this, bcb6_std_string *tmpS, bcb6_std_string *Str, bcb6_std_vector *File, unsigned long Index)
{
	bcb6_std_string_ctor_assign(Str, (bcb6_std_string *)File->_M_start + Index % bcb6_std_vector_size(File, bcb6_std_string));
	ReplaceDefine(&_this->attributeSelector, Str);
	bcb6_std_string_add_string(tmpS, Str->_M_start, bcb6_std_string_length(Str));
	bcb6_std_string_dtor(Str);
}
