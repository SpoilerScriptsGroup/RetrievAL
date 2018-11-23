#include <windows.h>
#include "intrinsic.h"
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"
#include "bcb6_std_vector.h"
#include "TSSGCtrl.h"
#include "TSSGSubject.h"
#include "TSSGAttributeElement.h"

extern HANDLE hHeap;

#define atVARIABLE 0x0800

void __stdcall Attribute_expr(TSSGCtrl *SSGCtrl, TSSGSubject *parent, string *prefix, string *code)
{
	size_t nCodeLength;

	nCodeLength = code->_M_finish - code->_M_start;
	if (nCodeLength == 0)
		return;
	for (TEndWithAttribute **it = SSGCtrl->attributeSelector.nowAttributeVec->_M_start, **end = SSGCtrl->attributeSelector.nowAttributeVec->_M_finish; it < end; it++)
	{
		string *lpPrevCode;
		size_t nPrevCodeLength;

		if ((*it)->type != atVARIABLE)
			continue;
		lpPrevCode = &(*it)->code;
		nPrevCodeLength = lpPrevCode->_M_finish - lpPrevCode->_M_start;
		// semicolon(;) is not the lead and trail byte of codepage 932.
		// it can scan from backward.
		if (nPrevCodeLength != 0 || *(code->_M_finish - 1) != ';')
		{
			LPSTR  lpszCode;
			size_t length;

			lpszCode = HeapAlloc(hHeap, 0, nPrevCodeLength + nCodeLength + 2);
			if (lpszCode == NULL)
				break;
			if (nPrevCodeLength != 0)
				__movsb(lpszCode, lpPrevCode->_M_start, nPrevCodeLength);
			__movsb(lpszCode + nPrevCodeLength, code->_M_start, nCodeLength + 1);
			length = nPrevCodeLength + nCodeLength;
			if (!length || *(lpszCode + length - 1) != ';')
				*(LPWORD)(lpszCode + length++) = BSWAP16(';\0');
			TEndWithAttribute_Setting_cstr(*it, lpszCode, length);
			HeapFree(hHeap, 0, lpszCode);
		}
		else
		{
			TEndWithAttribute_Setting_cstr(*it, code->_M_start, code->_M_finish - code->_M_start);
		}
		break;
	}
}
