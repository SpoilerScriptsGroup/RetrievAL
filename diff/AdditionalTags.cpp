#include <windows.h>
#include "SSGAttributeElement.h"
#include "intrinsic.h"

#define AT_VARIABLE  0x0800
#define AT_DEFINE    0x1000
#define AT_ERRORSKIP 0x2000

#ifndef TYPEDEF_PROCESSMEMORYBLOCK
#define TYPEDEF_PROCESSMEMORYBLOCK
typedef struct {
	DWORD  Id;
	LPVOID Address;
	size_t Size;
	DWORD  Protect;
} PROCESSMEMORYBLOCK, *PPROCESSMEMORYBLOCK;
#endif

EXTERN_C unsigned __int64 __cdecl _strtoui64(const char *nptr, char **endptr, int base);

extern HANDLE             hHeap;
extern size_t             nNumberOfProcessMemory;
extern PROCESSMEMORYBLOCK *lpProcessMemory;

void __stdcall Attribute_variable_open(TSSGCtrl *SSGCtrl, TSSGSubject *parent, string &prefix, string &code)
{
	TEndWithAttribute        *variable, *parentVariable;
	LPDWORD                  type;
	string                   newCode;
	string::reverse_iterator rit;

	variable = new TEndWithAttribute();
	if (variable == NULL)
		return;
	type = (LPDWORD)variable + 1;
	*type = AT_VARIABLE;
	parentVariable = (TEndWithAttribute *)SSGCtrl->GetAttribute(parent, AT_VARIABLE);
	if (parentVariable != NULL)
		newCode = parentVariable->GetCode();
	newCode += code;
	rit = newCode.rbegin();
	if (rit != newCode.rend())
		if (*rit != ';')
			newCode += ';';
	variable->Setting(newCode.c_str());
	SSGCtrl->GetAttributeSelector()->PushElement(variable);
}

void __stdcall Attribute_variable_close(TSSGCtrl *SSGCtrl, TSSGSubject *parent, string &prefix, string &code)
{
	TEndWithAttribute variable;
	LPDWORD           type;

	type = (LPDWORD)&variable + 1;
	*type = AT_VARIABLE;
	SSGCtrl->GetAttributeSelector()->PopElement(&variable);
}

void __stdcall Attribute_expr(TSSGCtrl *SSGCtrl, TSSGSubject *parent, string &prefix, string &code)
{
	vector<TSSGAttributeElement *>           *attributeVec;
	vector<TSSGAttributeElement *>::iterator it, end;

	if (code.empty())
		return;
	attributeVec = SSGCtrl->GetAttributeSelector()->GetNowAtteributeVec();
	for (it = attributeVec->begin(), end = attributeVec->end(); it < end; it++)
	{
		TEndWithAttribute        *variable;
		string                   *variableCode;
		string                   newCode;
		string::reverse_iterator rit;

		variable = (TEndWithAttribute *)*it;
		if (variable->GetType() != AT_VARIABLE)
			continue;
		variableCode = (string *)((LPBYTE)variable + 8);
		newCode = *variableCode + code;
		rit = newCode.rbegin();
		if (rit != newCode.rend())
			if (*rit != ';')
				newCode += ';';
		variable->Setting(newCode.c_str());
		break;
	}
}

void __stdcall Attribute_define(TSSGCtrl *SSGCtrl, TSSGSubject *parent, string &prefix, string &code)
{
	LPCSTR                         key;
	size_t                         keyLength;
	vector<TSSGAttributeElement *> *attribute;
	TSSGAttributeSelector          *attributeSelector;
	TIO_FEPAttribute               *define;

	if (code.empty())
		return;

	key = prefix.c_str() + 8;
	while (__intrinsic_isspace(*key))
		key++;
	keyLength = prefix.length() - (key - prefix.c_str());
	if (keyLength == 0)
		return;

	attributeSelector = SSGCtrl->GetAttributeSelector();
	attribute = attributeSelector->GetNowAtteributeVec();
	for (vector<TSSGAttributeElement *>::iterator it = attribute->begin(), end = attribute->end(); it < end; it++)
	{
		TIO_FEPAttribute *define;
		string *inputCode;
		string *outputCode;

		define = (TIO_FEPAttribute *)*it;
		if (define->GetType() != AT_DEFINE)
			continue;
		inputCode = (string *)((LPBYTE)define + 8);
		if (inputCode->length() != keyLength + 2)
			continue;
		if (memcmp(inputCode->c_str() + 1, key, keyLength) != 0)
			continue;
		outputCode = (string *)((LPBYTE)define + 8) + 1;
		*outputCode = code;
		return;
	}

	attribute = parent->GetAttribute();
	if (attribute != NULL)
	{
		for (vector<TSSGAttributeElement *>::iterator it = attribute->begin(), end = attribute->end(); it < end; it++)
		{
			TIO_FEPAttribute *define;
			string           *inputCode;
			string           *outputCode;

			define = (TIO_FEPAttribute *)*it;
			if (define->GetType() != AT_DEFINE)
				continue;
			inputCode = (string *)((LPBYTE)define + 8);
			if (inputCode->length() != keyLength + 2)
				continue;
			if (memcmp(inputCode->c_str() + 1, key, keyLength) != 0)
				continue;
			outputCode = (string *)((LPBYTE)define + 8) + 1;
			*outputCode = code;
			return;
		}
	}

	define = new TIO_FEPAttribute();
	if (define != NULL)
	{
		LPDWORD type;
		string  *inputCode;
		string  *outputCode;
		LPSTR   p;

		type = (LPDWORD)define + 1;
		*type = AT_DEFINE;
		inputCode = (string *)((LPBYTE)define + 8);
		inputCode->resize(keyLength + 2);
		p = inputCode->begin();
		*p = '{';
		memcpy(p + 1, key, keyLength);
		*(p + 1 + keyLength) = '}';
		outputCode = (string *)((LPBYTE)define + 8) + 1;
		*outputCode = code;
		attributeSelector->AddElement(define);
	}
}

void __stdcall Attribute_undef(TSSGCtrl *SSGCtrl, TSSGSubject *parent, string &prefix, string &code)
{
	LPCSTR                         key;
	size_t                         keyLength;
	vector<TSSGAttributeElement *> *attribute;
	TSSGAttributeSelector          *attributeSelector;

	key = prefix.c_str() + 7;
	while (__intrinsic_isspace(*key))
		key++;
	keyLength = prefix.length() - (key - prefix.c_str());
	if (keyLength == 0)
		return;

	attributeSelector = SSGCtrl->GetAttributeSelector();
	attribute = attributeSelector->GetNowAtteributeVec();
	for (vector<TSSGAttributeElement *>::iterator it = attribute->end() - 1, end = attribute->begin() - 1; it > end; it--)
	{
		TIO_FEPAttribute *define;
		string           *inputCode;

		define = (TIO_FEPAttribute *)*it;
		if (define->GetType() != AT_DEFINE)
			continue;
		inputCode = (string *)((LPBYTE)define + 8);
		if (inputCode->length() != keyLength + 2)
			continue;
		if (memcmp(inputCode->c_str() + 1, key, keyLength) != 0)
			continue;
		attributeSelector->EraseElement(define);
		break;
	}
}

void __stdcall Attribute_allocate(TSSGCtrl *SSGCtrl, TSSGSubject *parent, string &prefix, string &code)
{
	ULARGE_INTEGER     qwValue;
	DWORD              nId;
	size_t             nSize;
	DWORD              flProtect;
	LPSTR              p, endptr;
	size_t             nLength;
	PROCESSMEMORYBLOCK *lpElement;

	qwValue.QuadPart = _strtoui64(code.begin(), &endptr, 0);
	if (qwValue.HighPart != 0)
		return;
	nId = qwValue.LowPart;
	if (endptr == code.begin() || *endptr == '\0')
		return;
	for (size_t i = 0; i < nNumberOfProcessMemory; i++)
		if ((DWORD)nId == lpProcessMemory[i].Id)
			return;
	p = endptr;
	while (__intrinsic_isspace(*p))
		p++;
	if (*p != ',')
		return;
	p++;
	while (__intrinsic_isspace(*p))
		p++;
#ifndef _WIN64
	qwValue.QuadPart = _strtoui64(p, &endptr, 0);
	if (qwValue.HighPart != 0)
		return;
	nSize = qwValue.LowPart;
#else
	nSize = _strtoui64(p, &endptr, 0);
#endif
	if (endptr == p || *endptr == '\0')
		return;
	p = endptr;
	while (__intrinsic_isspace(*p))
		p++;
	if (*p != ',')
		return;
	p++;
	while (__intrinsic_isspace(*p))
		p++;
	nLength = code.end() - p;
	switch (nLength)
	{
	case 1:
		if (*(LPBYTE)p == 'R')
		{
			flProtect = PAGE_READONLY;
		}
		else if (*(LPBYTE)p == 'E')
		{
			flProtect = PAGE_EXECUTE;
		}
		else if (*(LPBYTE)p == 'L')
		{
			flProtect = 0;
		}
		else
		{
			goto UNMATCHED;
		}
		break;
	case 2:
		if (*(LPWORD)p == BSWAP16('NA'))
		{
			flProtect = PAGE_NOACCESS;
		}
		else if (*(LPWORD)p == BSWAP16('RW'))
		{
			flProtect = PAGE_READWRITE;
		}
		else if (*(LPWORD)p == BSWAP16('ER'))
		{
			flProtect = PAGE_EXECUTE_READ;
		}
		else
		{
			goto UNMATCHED;
		}
		break;
	case 3:
		if (*(LPDWORD)p == BSWAP32('ERW\0'))
		{
			flProtect = PAGE_EXECUTE_READWRITE;
		}
		else if (*(LPDWORD)p == BSWAP32('RNC\0'))
		{
			flProtect = PAGE_READONLY | PAGE_NOCACHE;
		}
		else if (*(LPDWORD)p == BSWAP32('ENC\0'))
		{
			flProtect = PAGE_EXECUTE | PAGE_NOCACHE;
		}
		else
		{
			goto UNMATCHED;
		}
		break;
	case 4:
		if (*(LPDWORD)p == BSWAP32('RWNC'))
		{
			flProtect = PAGE_READONLY | PAGE_NOCACHE;
		}
		else if (*(LPDWORD)p == BSWAP32('ERNC'))
		{
			flProtect = PAGE_EXECUTE_READ | PAGE_NOCACHE;
		}
		else
		{
			goto UNMATCHED;
		}
		break;
	case 5:
		if (*(LPDWORD)p == BSWAP32('ERWN'))
		{
			if (*(LPBYTE )(p + 4) == (BYTE)'C')
				flProtect = PAGE_EXECUTE_READWRITE | PAGE_NOCACHE;
			else
				goto UNMATCHED;
		}
		else if (*(LPDWORD)p == BSWAP32('Loca'))
		{
			if (*(LPBYTE )(p + 4) == (BYTE)'l')
				flProtect = 0;
			else
				goto UNMATCHED;
		}
		else
		{
			goto UNMATCHED;
		}
		break;
	case 7:
		if (*(LPDWORD) p      == BSWAP32('Exec') &&
			*(LPDWORD)(p + 4) == BSWAP32('ute\0'))
		{
			flProtect = PAGE_EXECUTE;
		}
		else
		{
			goto UNMATCHED;
		}
		break;
	case 8:
		if (*(LPDWORD)p == BSWAP32('Read'))
		{
			if (*(LPDWORD)(p + 4) == BSWAP32('Only'))
				flProtect = PAGE_READONLY;
			else
				goto UNMATCHED;
		}
		else if (*(LPDWORD)p == BSWAP32('NoAc'))
		{
			if (*(LPDWORD)(p + 4) == BSWAP32('cess'))
				flProtect = PAGE_NOACCESS;
			else
				goto UNMATCHED;
		}
		else
		{
			goto UNMATCHED;
		}
		break;
	case 9:
		if (*(LPDWORD) p      == BSWAP32('Read') &&
		    *(LPDWORD)(p + 4) == BSWAP32('Writ') &&
		    *(LPBYTE )(p + 8) == (BYTE)  'e')
		{
			flProtect = PAGE_READWRITE;
		}
		else
		{
			goto UNMATCHED;
		}
		break;
	case 11:
		if (*(LPDWORD) p      == BSWAP32('Exec') &&
		    *(LPDWORD)(p + 4) == BSWAP32('uteR') &&
		    *(LPDWORD)(p + 8) == BSWAP32('ead\0'))
		{
			flProtect = PAGE_EXECUTE_READ;
		}
		else
		{
			goto UNMATCHED;
		}
		break;
	case 14:
		if (*(LPDWORD) p       == BSWAP32('Exec') &&
		    *(LPDWORD)(p +  4) == BSWAP32('uteN') &&
		    *(LPDWORD)(p +  8) == BSWAP32('oCac') &&
		    *(LPWORD )(p + 12) == BSWAP16('he'))
		{
			flProtect = PAGE_EXECUTE | PAGE_NOCACHE;
		}
		else
		{
			goto UNMATCHED;
		}
		break;
	case 15:
		if (*(LPDWORD) p       == BSWAP32('Read') &&
		    *(LPDWORD)(p +  4) == BSWAP32('Only') &&
		    *(LPDWORD)(p +  8) == BSWAP32('NoCa') &&
		    *(LPDWORD)(p + 12) == BSWAP32('che\0'))
		{
			flProtect = PAGE_READONLY | PAGE_NOCACHE;
		}
		else
		{
			goto UNMATCHED;
		}
		break;
	case 16:
		if (*(LPDWORD)p == BSWAP32('Exec'))
		{
			if (*(LPDWORD)(p +  4) == BSWAP32('uteR') &&
			    *(LPDWORD)(p +  8) == BSWAP32('eadW') &&
			    *(LPDWORD)(p + 12) == BSWAP32('rite'))
			{
				flProtect = PAGE_EXECUTE_READWRITE;
			}
			else
			{
				goto UNMATCHED;
			}
		}
		else if (*(LPDWORD)p == BSWAP32('Read'))
		{
			if (*(LPDWORD)(p +  4) == BSWAP32('Writ') &&
			    *(LPDWORD)(p +  8) == BSWAP32('eNoC') &&
			    *(LPDWORD)(p + 12) == BSWAP32('ache'))
			{
				flProtect = PAGE_READONLY | PAGE_NOCACHE;
			}
			else
			{
				goto UNMATCHED;
			}
		}
		else
		{
			goto UNMATCHED;
		}
		break;
	case 18:
		if (*(LPDWORD) p       == BSWAP32('Exec') &&
		    *(LPDWORD)(p +  4) == BSWAP32('uteR') &&
		    *(LPDWORD)(p +  8) == BSWAP32('eadN') &&
		    *(LPDWORD)(p + 12) == BSWAP32('oCac') &&
		    *(LPWORD )(p + 16) == BSWAP16('he'))
		{
			flProtect = PAGE_EXECUTE_READ | PAGE_NOCACHE;
		}
		else
		{
			goto UNMATCHED;
		}
		break;
	case 23:
		if (*(LPDWORD) p       == BSWAP32('Exec') &&
		    *(LPDWORD)(p +  4) == BSWAP32('uteR') &&
		    *(LPDWORD)(p +  8) == BSWAP32('eadW') &&
		    *(LPDWORD)(p + 12) == BSWAP32('rite') &&
		    *(LPDWORD)(p + 16) == BSWAP32('NoCa') &&
		    *(LPDWORD)(p + 20) == BSWAP32('che\0'))
		{
			flProtect = PAGE_EXECUTE_READWRITE | PAGE_NOCACHE;
		}
		else
		{
			goto UNMATCHED;
		}
		break;
	default:
	UNMATCHED:
		qwValue.QuadPart = _strtoui64(p, &endptr, 0);
		if (qwValue.HighPart != 0)
			return;
		flProtect = qwValue.LowPart;
		if (endptr == p || *endptr != '\0')
			return;
		break;
	}
	if ((nNumberOfProcessMemory & 0x0F) == 0)
	{
		if (nNumberOfProcessMemory != 0)
		{
			LPVOID lpMem;
			size_t nBytes;

			nBytes = (nNumberOfProcessMemory + 0x10) * sizeof(PROCESSMEMORYBLOCK);
			lpMem = HeapReAlloc(hHeap, 0, lpProcessMemory, nBytes);
			if (lpMem == NULL)
				return;
			lpProcessMemory = (PROCESSMEMORYBLOCK *)lpMem;
		}
		else
		{
			lpProcessMemory = (PROCESSMEMORYBLOCK *)HeapAlloc(hHeap, 0, 0x10 * sizeof(PROCESSMEMORYBLOCK));
			if (lpProcessMemory == NULL)
				return;
		}
	}
	lpElement = lpProcessMemory + nNumberOfProcessMemory;
	lpElement->Id = nId;
	lpElement->Address = NULL;
	lpElement->Size = nSize;
	lpElement->Protect = flProtect;
	nNumberOfProcessMemory++;
}

void __stdcall Attribute_error_skip_open(TSSGCtrl *SSGCtrl, TSSGSubject *parent, string &prefix, string &code)
{
	TSSGAttributeElement *error_skip;

	error_skip = new TSSGAttributeElement();
	if (error_skip == NULL)
		return;
	*((LPDWORD)error_skip + 1) = AT_ERRORSKIP;
	SSGCtrl->GetAttributeSelector()->PushElement(error_skip);
}

void __stdcall Attribute_error_skip_close(TSSGCtrl *SSGCtrl, TSSGSubject *parent, string &prefix, string &code)
{
	TSSGAttributeElement error_skip;

	*((LPDWORD)&error_skip + 1) = AT_ERRORSKIP;
	SSGCtrl->GetAttributeSelector()->PopElement(&error_skip);
}

#if SCOPE_SUPPORT
EXTERN_C void __stdcall ReplaceDefine(TSSGAttributeSelector *attributeSelector, string *line);

void __stdcall Attribute_scope_open(TSSGCtrl *SSGCtrl, TSSGSubject *parent, string &prefix, string &code)
{
	THeapAdjustmentAttribute *heap = (THeapAdjustmentAttribute *)SSGCtrl->MakeAdjustmentClass("heap");
	heap->Setting(*SSGCtrl, "");
	heap->type = atSCOPE;
	SSGCtrl->GetAttributeSelector()->PushElement(heap);
}

void __stdcall Attribute_scope_close(TSSGCtrl *SSGCtrl, TSSGSubject *parent, string &prefix, string &code)
{
	TSSGAttributeElement scope;
	scope.type = atSCOPE;
	SSGCtrl->GetAttributeSelector()->PopElement(&scope);
}

void __stdcall Attribute_offset_open(TSSGCtrl *SSGCtrl, TSSGSubject *parent, string &prefix, string &code)
{
	TReplaceAttribute *replace = new TReplaceAttribute();
	ReplaceDefine(SSGCtrl->GetAttributeSelector(), &code);
	replace->Setting(SSGCtrl->strD, code.c_str());
	SSGCtrl->GetAttributeSelector()->AddElement(replace);
}

void __stdcall Attribute_offset_close(TSSGCtrl *SSGCtrl, TSSGSubject *parent, string &prefix, string &code)
{
	TSSGAttributeElement offset;
	offset.type = ssgCtrl::atREPLACE;
	SSGCtrl->GetAttributeSelector()->EraseElement(&offset);
}
#endif

void __stdcall AdditionalTags(TSSGCtrl *SSGCtrl, TSSGSubject *parent, string &prefix, string &code)
{
	size_t length;
	LPCSTR lpsz;
	DWORD  dw;

	length = prefix.length();
	if (length < 5)
		return;
	lpsz = prefix.c_str();
	dw = *(LPDWORD)lpsz;
	if (length == 5)
	{
		if (dw == BSWAP32('[exp') &&
			*(LPWORD)&lpsz[4] == BSWAP16('r\0'))
		{
			Attribute_expr(SSGCtrl, parent, prefix, code);
		}
		return;
	}
	if (dw == BSWAP32('[und'))
	{
		if (length < 8)
			return;
		if (*(LPWORD)&lpsz[4] == BSWAP16('ef') &&
			__intrinsic_isspace(lpsz[6]))
		{
			Attribute_undef(SSGCtrl, parent, prefix, code);
		}
		return;
	}
	if (dw == BSWAP32('[def'))
	{
		if (length < 9)
			return;
		if ((*(LPDWORD)&lpsz[4] & 0x00FFFFFF) == BSWAP32('ine\0') &&
			__intrinsic_isspace(lpsz[7]))
		{
			Attribute_define(SSGCtrl, parent, prefix, code);
		}
		return;
	}
	switch (length)
	{
#if SCOPE_SUPPORT
	case 6:
		if (dw == BSWAP32('[sco') &&
			*(LPWORD)&lpsz[4] == BSWAP16('pe'))
		{
			Attribute_scope_open(SSGCtrl, parent, prefix, code);
		}
		break;
	case 7:
		if (dw == BSWAP32('[off'))
		{
			if (*(LPDWORD)&lpsz[4] == BSWAP32('set\0'))
			{
				Attribute_offset_open(SSGCtrl, parent, prefix, code);
			}
		}
		else if (dw == BSWAP32('[/sc'))
		{
			if (*(LPDWORD)&lpsz[4] == BSWAP32('ope\0'))
			{
				Attribute_scope_close(SSGCtrl, parent, prefix, code);
			}
		}
		break;
	case 8:
		if (dw == BSWAP32('[/of') &&
			*(LPDWORD)&lpsz[4] == BSWAP32('fset'))
		{
			Attribute_offset_close(SSGCtrl, parent, prefix, code);
		}
		break;
#endif
	case 9:
		if (dw == BSWAP32('[all'))
		{
			if (*(LPDWORD)&lpsz[4] == BSWAP32('ocat') &&
				(BYTE)lpsz[8] == (BYTE)'e')
			{
				Attribute_allocate(SSGCtrl, parent, prefix, code);
			}
		}
		else if (dw == BSWAP32('[var'))
		{
			if (*(LPDWORD)&lpsz[4] == BSWAP32('iabl') &&
				(BYTE)lpsz[8] == (BYTE)'e')
			{
				Attribute_variable_open(SSGCtrl, parent, prefix, code);
			}
		}
		break;
	case 10:
		if (dw == BSWAP32('[/va') &&
			*(LPDWORD)&lpsz[4] == BSWAP32('riab') &&
			*(LPWORD)&lpsz[8] == BSWAP16('le'))
		{
			Attribute_variable_close(SSGCtrl, parent, prefix, code);
		}
		break;
	case 11:
		if (dw == BSWAP32('[err') &&
			*(LPDWORD)&lpsz[4] == BSWAP32('or_s') &&
			*(LPDWORD)&lpsz[8] == BSWAP32('kip\0'))
		{
			Attribute_error_skip_open(SSGCtrl, parent, prefix, code);
		}
		break;
	case 12:
		if (dw == BSWAP32('[/er') &&
			*(LPDWORD)&lpsz[4] == BSWAP32('ror_') &&
			*(LPDWORD)&lpsz[8] == BSWAP32('skip'))
		{
			Attribute_error_skip_close(SSGCtrl, parent, prefix, code);
		}
		break;
	}
}
