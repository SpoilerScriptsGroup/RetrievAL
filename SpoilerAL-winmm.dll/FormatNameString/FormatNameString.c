#include <windows.h>
#include "intrinsic.h"
#include "bcb6_stdio.h"
#include "bcb6_float.h"
#include "bcb6_std_string.h"
#include "TSSGCtrl.h"
#include "TSSGSubject.h"

unsigned long __cdecl Parsing(IN TSSGCtrl *_this, IN TSSGSubject *SSGS, IN const bcb6_std_string *Src, ...);
double __cdecl ParsingDouble(IN TSSGCtrl *_this, IN TSSGSubject *SSGS, IN const bcb6_std_string *Src, IN double Val);
void __stdcall FormatNameString(TSSGCtrl *_this, TSSGSubject *SSGS, bcb6_std_string *s);

__declspec(naked) bcb6_std_string * __cdecl TSSGCtrl_GetNameString(bcb6_std_string *Result, TSSGCtrl *_this, TSSGSubject *SSGS, const bcb6_std_string *NameStr)
{
	__asm
	{
		#define Result  (esp +  4)
		#define _this   (esp +  8)
		#define SSGS    (esp + 12)
		#define NameStr (esp + 16)

		mov     edx, dword ptr [NameStr]
		sub     esp, 24
		mov     ecx, esp
		push    edx
		push    ecx
		call    dword ptr [bcb6_std_string_ctor_assign]
		mov     edx, dword ptr [SSGS   + 32]
		add     esp, 8
		mov     ecx, dword ptr [_this  + 24]
		push    esp
		push    edx
		push    ecx
		call    FormatNameString
		mov     eax, dword ptr [SSGS   + 24]
		push    esp
		push    eax
		mov     eax, 00504284H
		mov     edx, dword ptr [_this  + 32]
		mov     ecx, dword ptr [Result + 32]
		push    edx
		push    ecx
		call    eax
		add     esp, 16
		mov     ecx, esp
		call    bcb6_std_string_dtor
		mov     eax, dword ptr [Result + 24]
		add     esp, 24
		ret

		#undef Result
		#undef _this
		#undef SSGS
		#undef NameStr
	}
}

static char * __fastcall FindDoubleChar(const char *p, unsigned short w)
{
	char c;

	for (; c = *p; p++)
	{
		if (!__intrinsic_isleadbyte(c))
		{
			if (c != '\\')
			{
				if (c == (char)w && p[1] == (char)(w >> 8))
					break;
			}
			else
			{
				if (!(c = *(++p)))
					break;
				if (__intrinsic_isleadbyte(c))
					if (!*(++p))
						break;
			}
		}
		else if (!*(++p))
		{
			break;
		}
	}
	return (char *)p;
}

static char * __fastcall FindDelimiter(const char *p, const char *end)
{
	size_t nest;

	if (p >= end)
		goto NOT_FOUND;
	nest = 0;
	do
	{
		switch (*p)
		{
		case '(':
			nest++;
			break;
		case ')':
			if (nest)
				nest--;
			break;
		case ',':
			if (nest)
				break;
			return (char *)p;
		case '\\':
			if (++p >= end)
				goto NOT_FOUND;
		default:
			if (!__intrinsic_isleadbyte(*p) || ++p < end)
				break;
			goto NOT_FOUND;
		}
	} while (++p < end);
NOT_FOUND:
	return (char *)end;
}

static char * __fastcall TrimLeft(const char *left)
{
	char c = *left;
	while (__intrinsic_isspace(c))
		c = *(++left);
	return (char *)left;
}

static char * __fastcall TrimRight(const char *left, const char *right)
{
	while (--right > left)
	{
		char c = *right;
		if (!__intrinsic_isspace(c))
			return (char *)++right;
	}
	return (char *)left;
}

static char * __fastcall UnescapeString(char *p, char *end)
{
	if (p >= end)
		return p;
	do
	{
		char c = *p;
		if (!__intrinsic_isleadbyte(c))
		{
			if (c != '\\')
				continue;
			memcpy(p, p + 1, (end--) - p);
			if (p >= end)
				break;
			c = *p;
			if (!__intrinsic_isleadbyte(c))
				continue;
		}
		p++;
	} while (++p < end);
	return end;
}

static char * __stdcall ReplaceString(bcb6_std_string *s, char *destBegin, char *destEnd, char *srcBegin, char *srcEnd)
{
	size_t srcLength, destLength, diff, count;

	srcLength = srcEnd - srcBegin;
	destLength = destEnd - destBegin;
	if (diff = srcLength - destLength)
	{
		count = s->_M_finish - destEnd + 1;
		if (srcLength > destLength)
		{
			destBegin -= (size_t)s->_M_start;
			destEnd -= (size_t)s->_M_start;
			bcb6_std_string_reserve(s, bcb6_std_string_length(s) + diff);
			s->_M_finish += diff;
			destBegin += (size_t)s->_M_start;
			destEnd += (size_t)s->_M_start;
			memmove(destBegin + srcLength, destEnd, count);
		}
		else
		{
			memcpy(destBegin + srcLength, destEnd, count);
			s->_M_finish += diff;
		}
		destEnd += diff;
	}
	memcpy(destBegin, srcBegin, srcLength);
	return destEnd;
}

void __stdcall FormatNameString(TSSGCtrl *_this, TSSGSubject *SSGS, bcb6_std_string *s)
{
	#define BRACKET_OPEN  BSWAP16('<#')
	#define BRACKET_CLOSE BSWAP16('#>')

	char *bracketBegin;

	bracketBegin = FindDoubleChar(s->_M_start, BRACKET_OPEN);
	while (*bracketBegin)
	{
		char    *bracketEnd, *valueBegin, *valueEnd, *formatBegin, *formatEnd, type;
		BOOLEAN isFEP;

		bracketEnd = FindDoubleChar(bracketBegin + 2, BRACKET_CLOSE);
		if (!*bracketEnd)
			break;
		formatBegin = NULL;
		type = '\0';
		isFEP = FALSE;
		do	/* do { ... } while (0); */
		{
			char *term, *fepBegin, *fepEnd;

			valueBegin = TrimLeft(bracketBegin + 2);
			term = TrimRight(valueBegin, bracketEnd);
			bracketEnd += 2;
			if (term == valueBegin)
				break;
			formatBegin = FindDelimiter(valueBegin, term);
			valueEnd = TrimRight(valueBegin, formatBegin);
			if (formatBegin == term)
				break;
			formatBegin = TrimLeft(formatBegin + 1);
			fepBegin = FindDelimiter(formatBegin, term);
			formatEnd = TrimRight(formatBegin, fepBegin);
			if (formatEnd != formatBegin)
				type = *(formatEnd - 1);
			if (fepBegin == term)
				break;
			fepBegin = TrimLeft(fepBegin + 1);
			fepEnd = FindDelimiter(fepBegin, term);
			if (fepEnd == fepBegin)
				break;
			fepEnd = TrimRight(fepBegin, fepEnd);
			if (fepEnd - fepBegin != 3)
				break;
			if (fepBegin[0] != 'f' || fepBegin[1] != 'e' || fepBegin[2] != 'p')
				break;
			isFEP = TRUE;
		} while (0);
		if (formatBegin)
		{
			char buffer[256];

			switch (type)
			{
			case 'e': case 'E': case 'f': case 'g': case 'G': case 'a': case 'A':
				{
					double          number;
					bcb6_std_string src;
					UINT            length;

					*valueEnd = '\0';
					valueEnd = UnescapeString(valueBegin, valueEnd);
					bcb6_std_string_ctor_assign_range(&src, valueBegin, valueEnd);
					number = ParsingDouble(_this, SSGS, &src, 0);
					bcb6_std_string_dtor(&src);
					if (isFEP)
						number = TSSGCtrl_CheckIO_FEPDouble(_this, SSGS, number, FALSE);
					if (!bcb6__isnan(number))
						*formatEnd = '\0';
					else
						formatBegin = "%f";
					length = bcb6__snprintf(buffer, _countof(buffer), formatBegin, number);
					if (length >= _countof(buffer))
						length = (int)length >= 0 ? _countof(buffer) - 1 : 0;
					bracketEnd = ReplaceString(s, bracketBegin, bracketEnd, buffer, buffer + length);
				}
				break;
			case 'n':
				bracketEnd = ReplaceString(s, bracketBegin, bracketEnd, formatBegin, formatEnd);
				break;
			default:
				{
					DWORD           number;
					bcb6_std_string src;
					UINT            length;

					*valueEnd = '\0';
					valueEnd = UnescapeString(valueBegin, valueEnd);
					bcb6_std_string_ctor_assign_range(&src, valueBegin, valueEnd);
					number = Parsing(_this, SSGS, &src, 0);
					bcb6_std_string_dtor(&src);
					if (isFEP)
						number = TSSGCtrl_CheckIO_FEP(_this, SSGS, number, FALSE);
					if (type)
						*formatEnd = '\0';
					else
						formatBegin = "%d";
					length = bcb6__snprintf(buffer, _countof(buffer), formatBegin, number);
					if (length >= _countof(buffer))
						length = (int)length >= 0 ? _countof(buffer) - 1 : 0;
					bracketEnd = ReplaceString(s, bracketBegin, bracketEnd, buffer, buffer + length);
				}
				break;
			}
		}
		bracketBegin = FindDoubleChar(bracketEnd, BRACKET_OPEN);
	}

	#undef BRACKET_OPEN
	#undef BRACKET_CLOSE
}
