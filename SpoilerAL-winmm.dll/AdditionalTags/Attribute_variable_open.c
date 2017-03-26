#include <windows.h>
#include "intrinsic.h"
#include "bcb6_std_string.h"
#include "TSSGCtrl.h"
#include "TSSGSubject.h"
#include "TEndWithAttribute.h"

extern HANDLE hHeap;

#define AT_VARIABLE 0x0800

void __stdcall Attribute_variable_open(TSSGCtrl *SSGCtrl, TSSGSubject *parent, bcb6_std_string *prefix, bcb6_std_string *code)
{
	TEndWithAttribute *lpNewVariable;
	TEndWithAttribute *lpParentVariable;
	bcb6_std_string   *lpParentCode;
	size_t            nParentCodeLength;
	size_t            nCodeLength;

	lpNewVariable = new_TEndWithAttribute();
	if (lpNewVariable == NULL)
		return;
	lpNewVariable->type = AT_VARIABLE;
	lpParentVariable = (TEndWithAttribute *)TSSGCtrl_GetAttribute(SSGCtrl, parent, AT_VARIABLE);
	if (lpParentVariable != NULL)
	{
		lpParentCode = &lpParentVariable->code;
		nParentCodeLength = lpParentCode->_M_finish - lpParentCode->_M_start;
	}
	else
	{
		nParentCodeLength = 0;
	}
	nCodeLength = code->_M_finish - code->_M_start;
	// semicolon(;) is not the lead and tail byte of codepage 932.
	// it can scan from backward.
	if (nParentCodeLength != 0 || nCodeLength == 0 || *(code->_M_finish - 1) != ';')
	{
		LPSTR lpszCode;

		lpszCode = HeapAlloc(hHeap, 0, nParentCodeLength + nCodeLength + 2);
		if (lpszCode != NULL)
		{
			if (nParentCodeLength != 0)
				__movsb(lpszCode, lpParentCode->_M_start, nParentCodeLength);
			__movsb(lpszCode + nParentCodeLength, code->_M_start, nCodeLength + 1);
			if (*(code->_M_finish - 1) != ';')
				*(LPWORD)(lpszCode + nParentCodeLength + nCodeLength) = BSWAP16(';\0');
			TEndWithAttribute_Setting(lpNewVariable, lpszCode);
			HeapFree(hHeap, 0, lpszCode);
		}
	}
	else
	{
		TEndWithAttribute_Setting(lpNewVariable, code->_M_start);
	}
	TSSGAttributeSelector_PushElement(&SSGCtrl->attributeSelector, lpNewVariable);
}
