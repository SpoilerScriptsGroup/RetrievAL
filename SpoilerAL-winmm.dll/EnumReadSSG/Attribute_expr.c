#include <windows.h>
#include "intrinsic.h"
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"
#include "bcb6_std_vector.h"
#include "TSSGCtrl.h"
#include "TSSGSubject.h"
#include "TSSGAttributeElement.h"

extern HANDLE hHeap;

void __stdcall Attribute_expr(TSSGCtrl *this, LPCSTR Code, LPCSTR EndOfCode)
{
	size_t nCodeLength;

	if (!(nCodeLength = EndOfCode - Code))
		return;
	for (TPrologueAttribute **it = vector_begin(this->attributeSelector.nowAttributeVec), **end = vector_end(this->attributeSelector.nowAttributeVec); it < end; it++)
	{
		string *lpPrevCode;
		size_t nPrevCodeLength;

		if ((*it)->type != atPROLOGUE)
			continue;
		lpPrevCode = &(*it)->code;
		nPrevCodeLength = string_length(lpPrevCode);
		// semicolon(;) is not the lead and trail byte of codepage 932.
		// it can scan from backward.
		if (nPrevCodeLength != 0 || *(EndOfCode - 1) != ';')
		{
			LPSTR  lpszCode;
			size_t length;

			lpszCode = HeapAlloc(hHeap, 0, nPrevCodeLength + nCodeLength + 2);
			if (lpszCode == NULL)
				break;
			if (nPrevCodeLength != 0)
				__movsb(lpszCode, string_c_str(lpPrevCode), nPrevCodeLength);
			__movsb(lpszCode + nPrevCodeLength, Code, nCodeLength + 1);
			length = nPrevCodeLength + nCodeLength;
			if (!length || *(lpszCode + length - 1) != ';')
				*(LPWORD)(lpszCode + length++) = BSWAP16(';\0');
			TEndWithAttribute_Setting(*it, lpszCode, length);
			HeapFree(hHeap, 0, lpszCode);
		}
		else
		{
			TEndWithAttribute_Setting(*it, Code, nCodeLength);
		}
		break;
	}
}
