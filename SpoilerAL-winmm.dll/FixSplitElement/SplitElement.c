#include <windows.h>
#include "intrinsic.h"

#ifdef __BORLANDC__
#define string_begin(s)                                              (s)->begin()
#define string_end(s)                                                (s)->end()
#define vector_TSSGAttributeElement                                  vector<TSSGAttributeElement *>
#define offsetof_TSSGCtrl_attributeSelector                          (size_t)((TSSGCtrl *)NULL)->GetAttributeSelector()
#define TSSGAttributeSelector_GetNowAtteributeVec(attributeSelector) attributeSelector->GetNowAtteributeVec()
#else
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"
#include "TSSGCtrl.h"
#define vector_TSSGAttributeElement                                  vector
#define offsetof_TSSGCtrl_attributeSelector                          offsetof(TSSGCtrl, attributeSelector)
#endif

extern HANDLE hHeap;
size_t __stdcall ReplaceDefineByHeap(vector_TSSGAttributeElement *attributes, LPSTR *line, size_t length, size_t capacity);

#if 0
#ifdef __BORLANDC__
string __cdecl SplitElement(LPVOID SSGC_strD, string *Src, string Token, DWORD Index, DWORD Option)
#else
string * __cdecl SplitElement(string *Result, LPVOID SSGC_strD, string *Src, string Token, DWORD Index, DWORD Option)
#endif
{
	LPSTR  buffer;
	LPBYTE token, p, begin, end;

#ifndef __BORLANDC__
	string_dtor(&Token);
#endif
	buffer = NULL;
	p = (LPBYTE)string_begin(Src);
	while (__intrinsic_isspace(*p))
		p++;
	if (*p != '_')
	{
		begin = p;
		end = string_end(Src);
		while (p < end)
		{
			if (!__intrinsic_isleadbyte(*p))
			{
				if (*p != '=')
				{
					p++;
				}
				else
				{
					token = p;
					goto FOUND_TOKEN;
				}
			}
			else
			{
				p += 2;
			}
		}
	}
	else
	{
		size_t                      length, capacity;
		unsigned long               bits;
		TSSGAttributeSelector       *attributeSelector;
		vector_TSSGAttributeElement *attributes;

		length = string_end(Src) - p;
		if (!length || (INT_PTR)(length + (sizeof(DWORD) - 1)) < 0)
			goto FAILED1;
#ifndef _WIN64
		_BitScanReverse(&bits, length + (sizeof(DWORD) - 1));
#else
		_BitScanReverse64(&bits, length + (sizeof(DWORD) - 1));
#endif
		capacity = (size_t)1 << (bits + 1);
		buffer = (LPSTR)HeapAlloc(hHeap, 0, capacity);
		if (!buffer)
			goto FAILED1;
		memcpy(buffer, p, length + 1);
		attributeSelector = (TSSGAttributeSelector *)((LPBYTE)SSGC_strD - (offsetof(TSSGCtrl, strD) - offsetof_TSSGCtrl_attributeSelector));
		attributes = TSSGAttributeSelector_GetNowAtteributeVec(attributeSelector);
		if (attributes)
		{
			length = ReplaceDefineByHeap(attributes, &buffer, length, capacity);
			if (!length || length == -1)
				goto FAILED2;
#ifndef _WIN64
			_BitScanReverse(&bits, length);
#else
			_BitScanReverse64(&bits, length);
#endif
			capacity = (size_t)1 << (bits + 1);
			if (capacity < length + sizeof(DWORD))
			{
				p = (LPBYTE)HeapReAlloc(hHeap, 0, buffer, length + sizeof(DWORD));
				if (!p)
					goto FAILED2;
				buffer = p;
			}
		}
		begin = buffer;
		end = buffer + length;
		*(LPDWORD)end = 0;
		p = begin + 1;
		while (__intrinsic_isspace(*p))
			p++;
		if (*p == 'L' && __intrinsic_isspace(*(p + 1)))
		{
			p += 2;
			while (__intrinsic_isspace(*p))
				p++;
		}
		token = NULL;
		do
		{
			BYTE c;

			c = *p;
			if (!__intrinsic_isleadbyte(c))
			{
				if (c != '$')
				{
					if (c == '=')
						token = p;
					p++;
				}
				else
				{
					if (++p >= end)
						break;
					c = *p;
					if ((char)c < '0' || c > (BYTE)'8')
						continue;
					p++;
					while (p < end)
					{
						c = *(p++);
						if (!__intrinsic_isleadbyte(c))
						{
							if (c != '$' || *p != '$')
								continue;
							p++;
							break;
						}
						else
						{
							p++;
						}
					}
				}
			}
			else
			{
				p += 2;
			}
		} while (p < end);
		if (token)
			goto FOUND_TOKEN;
	FAILED2:
		HeapFree(hHeap, 0, buffer);
	}
FAILED1:
#ifdef __BORLANDC__
	return "";
#else
	string_ctor(Result);
	return Result;
#endif

FOUND_TOKEN:
#ifdef __BORLANDC__
	*(token++) = '\0';
	string Result((LPCSTR)begin);
	*Src = (LPCSTR)token;
	if (buffer)
		HeapFree(hHeap, 0, buffer);
#else
	string_ctor_assign_range(Result, begin, token);
	string_assign_range(Src, token + 1, end);
	if (buffer)
		HeapFree(hHeap, 0, buffer);
#endif
	return Result;
}
#endif
