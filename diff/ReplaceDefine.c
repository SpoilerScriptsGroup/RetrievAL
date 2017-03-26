#include <windows.h>
#include "intrinsic.h"

#if defined(__BORLANDC__)
#pragma warn -8060
#endif

extern HANDLE hHeap;

#if defined(__BORLANDC__)
#ifndef SIZE_MAX
#ifdef _WIN64
#define SIZE_MAX _UI64_MAX
#else
#define SIZE_MAX UINT_MAX
#endif
#endif
#define bcb6_std_vector_string                                       vector<string>
#define bcb6_std_vector_TSSGAttributeElement                         vector<TSSGAttributeElement *>
#define bcb6_std_string                                              string
#define bcb6_std_string_begin(s)                                     (s)->begin()
#define bcb6_std_string_end(s)                                       (s)->end()
#define bcb6_std_string_length(s)                                    (s)->length()
#define TSSGAttributeElement_GetType(SSGAttributeElement)            (SSGAttributeElement)->GetType()
#define TIO_FEPAttribute_GetInputCode(IO_FEPAttribute)               (string *)((LPBYTE)(IO_FEPAttribute) + 8)
#define TIO_FEPAttribute_GetOutputCode(IO_FEPAttribute)              ((string *)((LPBYTE)(IO_FEPAttribute) + 8) + 1)
#define TSSGAttributeSelector_GetNowAtteributeVec(attributeSelector) attributeSelector->GetNowAtteributeVec()
#define TSSGSubject_GetAttribute(SSGS)                               (SSGS)->GetAttribute()
#else
#define LOCAL_MEMORY_SUPPORT 1
#include "bcb6_std_vector.h"
#include "bcb6_std_string.h"
#include "TIO_FEPAttribute.h"
#include "TSSGAttributeSelector.h"
#include "TSSGSubject.h"
#define bcb6_std_vector_string               bcb6_std_vector
#define bcb6_std_vector_TSSGAttributeElement bcb6_std_vector
#endif

#ifndef AT_DEFINE
#define AT_DEFINE 0x1000
#endif

size_t __stdcall ReplaceDefineByHeap(bcb6_std_vector_TSSGAttributeElement *attributes, LPSTR *line, size_t length, size_t capacity)
{
	LPSTR p;
	char  c;

	p = *line;
#if defined(LOCAL_MEMORY_SUPPORT) && LOCAL_MEMORY_SUPPORT
	if (*p == '_')
	{
		do
		{
			c = *(++p);
		} while (__intrinsic_isspace(c));
	}
	if (*p == 'L' && *(++p) == '{')
		goto INSERT_SPACE;
#endif
	while (c = *p)
	{
		if (!__intrinsic_isleadbyte(c))
		{
#if !defined(LOCAL_MEMORY_SUPPORT) || !LOCAL_MEMORY_SUPPORT
			if (c == '{')
			{
#else
			switch (c)
			{
			case ',':
				do
				{
					c = *(++p);
				} while (__intrinsic_isspace(c));
				if (c != '_')
					continue;
				do
				{
					c = *(++p);
				} while (__intrinsic_isspace(c));
				if (c != 'L')
					continue;
				if (*(++p) != '{')
					continue;
			INSERT_SPACE:
				if (++length == capacity)
				{
					LPSTR mem;

					if (!(capacity <<= 1))
						goto FAILED;
					mem = (LPSTR)HeapReAlloc(hHeap, 0, *line, capacity);
					if (!mem)
						goto FAILED;
					p += mem - *line;
					*line = mem;
				}
				memmove(p + 1, p, *line + length - p);
				*(p++) = ' ';
			case '{':
#endif
				LPCSTR key;
				size_t keyLength;
				LPCSTR end;

				end = key = p;
				while ((c = *(++end)) && c != '}');
				if (!c)
					goto NESTED_BREAK;
				keyLength = end - p + 1;
#if defined(__BORLANDC__)
				for (vector<TSSGAttributeElement *>::iterator it = attributes->begin(); it < attributes->end(); it++)
#else
				for (TIO_FEPAttribute **it = attributes->_M_start; it < (TIO_FEPAttribute **)attributes->_M_finish; it++)
#endif
				{
					bcb6_std_string *inputCode;
					bcb6_std_string *outputCode;
					size_t          valueLength;
					LPCSTR          value;
					ptrdiff_t       diff;

					if (TSSGAttributeElement_GetType(*it) != AT_DEFINE)
						continue;
					inputCode = TIO_FEPAttribute_GetInputCode((TIO_FEPAttribute *)*it);
					if (bcb6_std_string_length(inputCode) != keyLength)
						continue;
					if (memcmp(bcb6_std_string_begin(inputCode), key, keyLength) != 0)
						continue;
					outputCode = TIO_FEPAttribute_GetOutputCode((TIO_FEPAttribute *)*it);
					valueLength = bcb6_std_string_length(outputCode);
					diff = valueLength - keyLength;
					if (diff)
					{
						size_t n;
						LPCSTR src, end;

						n = length + diff;
						if (n >= capacity)
						{
							LPSTR mem;

							do
							{
								if (!(capacity <<= 1))
									goto FAILED;
							} while (n >= capacity);
							mem = (LPSTR)HeapReAlloc(hHeap, 0, *line, capacity);
							if (!mem)
								goto FAILED;
							p += mem - *line;
							*line = mem;
						}
						src = p + keyLength;
						end = *line + length;
						length = n;
						memmove(p + valueLength, src, end - src + 1);
					}
					value = bcb6_std_string_begin(outputCode);
					memcpy(p, value, valueLength);
					goto NESTED_CONTINUE;
				}
			}
		}
		else
		{
			if (!*(++p))
				break;
		}
		p++;
	NESTED_CONTINUE:;
	}
NESTED_BREAK:
	return p - *line;
FAILED:
	return SIZE_MAX;
}

void __stdcall ReplaceDefine(TSSGAttributeSelector *attributeSelector, bcb6_std_string *line)
{
	bcb6_std_vector_TSSGAttributeElement *attributes;
	LPCSTR                               begin;
	size_t                               length, capacity;
	unsigned long                        bits;
	LPSTR                                buffer;

	attributes = TSSGAttributeSelector_GetNowAtteributeVec(attributeSelector);
	if (attributes == NULL)
		return;
	begin = bcb6_std_string_begin(line);
	while (__intrinsic_isspace(*begin))
		begin++;
	length = bcb6_std_string_end(line) - begin;
	if ((INT_PTR)length < 0)
		return;
#ifndef _WIN64
	if (_BitScanReverse(&bits, length))
#else
	if (_BitScanReverse64(&bits, length))
#endif
		capacity = (size_t)1 << (bits + 1);
	else
		capacity = 1;
	buffer = (LPSTR)HeapAlloc(hHeap, 0, capacity);
	if (buffer != NULL)
	{
		memcpy(buffer, begin, length + 1);
		length = ReplaceDefineByHeap(attributes, &buffer, length, capacity);
		if (length != SIZE_MAX)
		{
#if defined(__BORLANDC__)
			*line = buffer;
#else
			bcb6_std_string_assign_cstr_with_length(line, buffer, length);
#endif
		}
		HeapFree(hHeap, 0, buffer);
	}
}

size_t __stdcall ByteArrayReplaceDefineByHeap(bcb6_std_vector_TSSGAttributeElement *attributes, LPSTR *line, size_t length, size_t capacity)
{
	LPSTR p;
	char  c, prev;

	p = *line;
	prev = '\0';
	while (c = *p)
	{
		if (!__intrinsic_isleadbyte(c))
		{
			switch (c)
			{
			case '$':
				if (!*(++p))
					goto NESTED_BREAK;
				for (; ; )
				{
					if (!(c = *(++p)))
						goto NESTED_BREAK;
					if (!__intrinsic_isleadbyte(c))
					{
						if (c != '\\')
						{
							if (c != '$' || *(p + 1) != '$')
								continue;
							p++;
							break;
						}
						if (!(c = *(++p)))
							goto NESTED_BREAK;
						if (!__intrinsic_isleadbyte(c))
							continue;
					}
					if (!*(++p))
						goto NESTED_BREAK;
				}
				prev = '$';
				break;
			case '\\':
				if (!(c = *(++p)))
					goto NESTED_BREAK;
				if (__intrinsic_isleadbyte(c))
				{
					if (!*(++p))
						goto NESTED_BREAK;
				}
				prev = '\0';
				break;
			case '{':
				if (prev != '*')
				{
					LPCSTR key;
					size_t keyLength;
					LPCSTR end;

					end = key = p;
					while ((c = *(++end)) && c != '}');
					if (!c)
						goto NESTED_BREAK;
					keyLength = end - p + 1;
#if defined(__BORLANDC__)
					for (vector<TSSGAttributeElement *>::iterator it = attributes->begin(); it < attributes->end(); it++)
#else
					for (TIO_FEPAttribute **it = attributes->_M_start; it < (TIO_FEPAttribute **)attributes->_M_finish; it++)
#endif
					{
						bcb6_std_string *inputCode;
						bcb6_std_string *outputCode;
						size_t          valueLength;
						LPCSTR          value;
						ptrdiff_t       diff;

						if (TSSGAttributeElement_GetType(*it) != AT_DEFINE)
							continue;
						inputCode = TIO_FEPAttribute_GetInputCode((TIO_FEPAttribute *)*it);
						if (bcb6_std_string_length(inputCode) != keyLength)
							continue;
						if (memcmp(bcb6_std_string_begin(inputCode), key, keyLength) != 0)
							continue;
						outputCode = TIO_FEPAttribute_GetOutputCode((TIO_FEPAttribute *)*it);
						valueLength = bcb6_std_string_length(outputCode);
						diff = valueLength - keyLength;
						if (diff)
						{
							size_t n;
							LPCSTR src, end;

							n = length + diff;
							if (n >= capacity)
							{
								LPSTR mem;

								do
								{
									if (!(capacity <<= 1))
										goto FAILED;
								} while (n >= capacity);
								mem = (LPSTR)HeapReAlloc(hHeap, 0, *line, capacity);
								if (!mem)
									goto FAILED;
								p += mem - *line;
								*line = mem;
							}
							src = p + keyLength;
							end = *line + length;
							length = n;
							memmove(p + valueLength, src, end - src + 1);
						}
						value = bcb6_std_string_begin(outputCode);
						memcpy(p, value, valueLength);
						goto NESTED_CONTINUE;
					}
					prev = '{';
					break;
				}
			default:
				prev = c;
				break;
			}
		}
		else
		{
			prev = '\0';
			if (!*(++p))
				break;
		}
		p++;
	NESTED_CONTINUE:;
	}
NESTED_BREAK:
	return p - *line;
FAILED:
	return SIZE_MAX;
}

void __stdcall ByteArrayReplaceDefine(TSSGSubject *SSGS, bcb6_std_string *line)
{
	bcb6_std_vector_TSSGAttributeElement *attributes;
	LPCSTR                               begin;
	size_t                               length, capacity;
	unsigned long                        bits;
	LPSTR                                buffer;

	attributes = TSSGSubject_GetAttribute(SSGS);
	if (attributes == NULL)
		return;
	begin = bcb6_std_string_begin(line);
	length = bcb6_std_string_end(line) - begin;
	if ((INT_PTR)length <= 0)
		return;
#ifndef _WIN64
	_BitScanReverse(&bits, length);
#else
	_BitScanReverse64(&bits, length);
#endif
	capacity = (size_t)1 << (bits + 1);
	buffer = (LPSTR)HeapAlloc(hHeap, 0, capacity);
	if (buffer != NULL)
	{
		memcpy(buffer, begin, length + 1);
		length = ByteArrayReplaceDefineByHeap(attributes, &buffer, length, capacity);
		if (length != SIZE_MAX)
		{
#if defined(__BORLANDC__)
			*line = buffer;
#else
			bcb6_std_string_assign_cstr_with_length(line, buffer, length);
#endif
		}
		HeapFree(hHeap, 0, buffer);
	}
}

#if defined(__BORLANDC__)
#undef bcb6_std_vector_string
#undef bcb6_std_vector_TSSGAttributeElement
#undef bcb6_std_string
#undef bcb6_std_string_begin
#undef bcb6_std_string_end
#undef bcb6_std_string_length
#undef TSSGAttributeElement_GetType
#undef TIO_FEPAttribute_GetInputCode
#undef TIO_FEPAttribute_GetOutputCode
#undef TSSGAttributeSelector_GetNowAtteributeVec
#undef TSSGSubject_GetAttribute
#else
#undef bcb6_std_vector_string
#undef bcb6_std_vector_TSSGAttributeElement
#endif
