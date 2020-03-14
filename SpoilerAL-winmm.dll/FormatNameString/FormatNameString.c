#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#define _NO_CRT_STDIO_INLINE
#include <stdio.h>
#include <float.h>
#include "intrinsic.h"
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_vector_string.h"
#include "TSSGCtrl.h"
#include "TSSGSubject.h"
#include "TStringDivision.h"
#include "SSGSubjectProperty.h"
#include "IsBadPtr.h"
#include "FindProcessMemory.h"
#include "Unescape.h"

EXTERN_C HANDLE hHeap;
EXTERN_C const DWORD F00504284;

void __stdcall ReplaceDefineDynamic(TSSGSubject *SSGS, string *line);
unsigned long __cdecl Parsing(IN TSSGCtrl *this, IN TSSGSubject *SSGS, IN const string *Src, ...);
double __cdecl ParsingDouble(IN TSSGCtrl *this, IN TSSGSubject *SSGS, IN const string *Src, IN double Val);

void __stdcall FormatNameString(TSSGCtrl *this, TSSGSubject *SSGS, string *s);

__declspec(naked) string * __cdecl TSSGCtrl_GetNameString(string *Result, TSSGCtrl *this, TSSGSubject *SSGS, const string *NameStr)
{
	__asm
	{
		#define Result  (esp +  4)
		#define this    (esp +  8)
		#define SSGS    (esp + 12)
		#define NameStr (esp + 16)

		mov     edx, dword ptr [NameStr]
		sub     esp, 24
		mov     ecx, esp
		call    string_ctor_assign
		mov     edx, dword ptr [SSGS   + 24]
		mov     ecx, dword ptr [this   + 24]
		push    esp
		push    edx
		push    ecx
		call    FormatNameString
		mov     eax, dword ptr [SSGS   + 24]
		push    esp
		push    eax
		mov     edx, dword ptr [this   + 32]
		mov     ecx, dword ptr [Result + 32]
		push    edx
		push    ecx
		call    dword ptr [F00504284]
		add     esp, 16
		mov     ecx, esp
		call    string_dtor
		mov     eax, dword ptr [Result + 24]
		add     esp, 24
		ret

		#undef Result
		#undef this
		#undef SSGS
		#undef NameStr
	}
}

#define BRACKET_OPEN  '<'
#define BRACKET_CLOSE '>'

static char * __fastcall FindBracketOpen(const char *p)
{
	char c;

	for (; c = *p; p++)
	{
		if (!__intrinsic_isleadbyte(c))
		{
			if (c != '\\')
			{
				if (c == BRACKET_OPEN)
					return (char *)p;
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
	return NULL;
}

static char * __fastcall FindDoubleChar(const char *p, const unsigned short w)
{
	char c;

	for (; c = *p; p++)
	{
		if (!__intrinsic_isleadbyte(c))
		{
			if (c != '\\')
			{
				if (c == (char)w && p[1] == (char)(w >> 8))
					return (char *)p;
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
	return NULL;
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

static char * __stdcall ReplaceString(string *s, char *destBegin, char *destEnd, const char *srcBegin, const char *srcEnd)
{
	size_t srcLength, destLength, diff, count;

	srcLength = srcEnd - srcBegin;
	destLength = destEnd - destBegin;
	if (diff = srcLength - destLength)
	{
		count = string_end(s) - destEnd + 1;
		if (srcLength > destLength)
		{
			destBegin -= (size_t)string_begin(s);
			destEnd -= (size_t)string_begin(s);
			string_resize(s, string_length(s) + diff);
			destBegin += (size_t)string_begin(s);
			destEnd += (size_t)string_begin(s);
			memmove(destBegin + srcLength, destEnd, count);
		}
		else
		{
			memcpy(destBegin + srcLength, destEnd, count);
			string_end(s) += diff;
		}
		destEnd += diff;
	}
	memcpy(destBegin, srcBegin, srcLength);
	return destEnd;
}

static char * __fastcall UnescapeParam(char *p, char *end)
{
	char quate;

	while ((p = UnescapeA(p, &end, TRUE)) < end && ((quate = *p) == '"' || quate == '\'') && ++p < end)
	{
		for (; ; )
		{
			char c;

			c = *(p++);
			if (p >= end)
				goto DONE;
			if (c == quate)
				break;
			if (c == '\\')
			{
				c = *(p++);
				if (p >= end)
					goto DONE;
			}
			if (__intrinsic_isleadbyte(c) && ++p >= end)
				goto DONE;
		}
	}
DONE:
	*p = '\0';
	return p;
}

void __stdcall FormatNameString(TSSGCtrl *this, TSSGSubject *SSGS, string *s)
{
	#define NUMBER_IDENTIFIER '#'
	#define LIST_IDENTIFIER   '@'
	#define NUMBER_CLOSE      (WORD)(NUMBER_IDENTIFIER | (WORD)BRACKET_CLOSE << 8)
	#define LIST_CLOSE        (WORD)(LIST_IDENTIFIER   | (WORD)BRACKET_CLOSE << 8)

	char stackBuffer[256];
	char *bracketBegin;

	ReplaceDefineDynamic(SSGS, s);
	bracketBegin = FindBracketOpen(string_c_str(s));
	while (bracketBegin)
	{
		char *bracketEnd;

		if (bracketBegin[1] == NUMBER_IDENTIFIER)
		{
			#define FEP  0x01
			#define UTF8 0x02

			char *valueBegin, *valueEnd, *formatBegin, *formatEnd, type;
			int  option;

			bracketEnd = FindDoubleChar(bracketBegin + 2, NUMBER_CLOSE);
			if (!bracketEnd)
				break;
			formatBegin = NULL;
			type = '\0';
			option = 0;
			do	/* do { ... } while (0); */
			{
				char *term, *optionBegin, *optionEnd;

				valueBegin = TrimLeftBlank(bracketBegin + 2);
				valueEnd = TrimRightBlank(valueBegin, bracketEnd);
				bracketEnd += 2;
				if (valueEnd == valueBegin)
					break;
				term = valueEnd;
				formatBegin = FindDelimiter(valueBegin, term);
				valueEnd = TrimRightBlank(valueBegin, formatBegin);
				if (formatBegin == term)
					break;
				formatBegin = TrimLeftBlank(formatBegin + 1);
				optionBegin = FindDelimiter(formatBegin, term);
				formatEnd = TrimRightBlank(formatBegin, optionBegin);
				if (formatEnd != formatBegin)
					type = *(formatEnd - 1);
				if (optionBegin == term)
					break;
				optionBegin = TrimLeftBlank(optionBegin + 1);
				optionEnd = FindDelimiter(optionBegin, term);
				if (optionEnd == optionBegin)
					break;
				optionEnd = TrimRightBlank(optionBegin, optionEnd);
				switch (optionEnd - optionBegin)
				{
				case 3:
					if (optionBegin[0] != 'f' || optionBegin[1] != 'e' || optionBegin[2] != 'p')
						break;
					option = FEP;
					break;
				case 4:
					if (*(LPDWORD)optionBegin != BSWAP32('utf8'))
						break;
					option = UTF8;
					break;
				case 8:
					if (*(LPDWORD)optionBegin != BSWAP32('fep_'))
						break;
					if (*(LPDWORD)(optionBegin + 4) != BSWAP32('utf8'))
						break;
					option = FEP | UTF8;
					break;
				}
			} while (0);
			switch (type)
			{
			case 'e': case 'E': case 'f': case 'g': case 'G': case 'a': case 'A':
				{
					double number;
					string src;
					UINT   length;
					char   *buffer;

					valueEnd = UnescapeParam(valueBegin, valueEnd);
					string_begin(&src) = valueBegin;
					string_end_of_storage(&src) = string_end(&src) = valueEnd;
					number = ParsingDouble(this, SSGS, &src, 0);
					if (option & FEP)
						number = TSSGCtrl_CheckIO_FEPDouble(this, SSGS, number, FALSE);
					if (formatBegin && !_isnan(number))
						*formatEnd = '\0';
					else
						formatBegin = "%f";
					length = _snprintf(stackBuffer, _countof(stackBuffer), formatBegin, number);
					buffer = stackBuffer;
					if (length >= _countof(stackBuffer))
					{
						if ((int)length >= 0)
						{
							UINT capacity;

							if (buffer = (char *)HeapAlloc(hHeap, 0, capacity = length + 1))
							{
								if ((length = _snprintf(buffer, capacity, formatBegin, number)) >= capacity)
									length = (int)length >= 0 ? capacity - 1 : 0;
							}
							else
							{
								buffer = stackBuffer;
								length = _countof(stackBuffer) - 1;
							}
						}
						else
						{
							length = 0;
						}
					}
					bracketEnd = ReplaceString(s, bracketBegin, bracketEnd, buffer, buffer + length);
					if (buffer != stackBuffer)
						HeapFree(hHeap, 0, buffer);
				}
				break;
			case 'n':
				bracketEnd = ReplaceString(s, bracketBegin, bracketEnd, formatBegin, formatEnd);
				break;
			default:
				{
					UINT_PTR param;
					string   src;
					UINT     length;
					char     *buffer;
					BOOL     isAllocated;

					valueEnd = UnescapeParam(valueBegin, valueEnd);
					string_begin(&src) = valueBegin;
					string_end_of_storage(&src) = string_end(&src) = valueEnd;
					param = Parsing(this, SSGS, &src, 0);
					if (option & FEP)
						param = TSSGCtrl_CheckIO_FEP(this, SSGS, param, FALSE);
					isAllocated = FALSE;
					if (type == 's' || type == 'S')
					{
						if (!(option & UTF8))
						{
							if (TSSGCtrl_IsRemoteProcess(valueBegin))
							{
								LPVOID readAddress;
								HANDLE hProcess;

								readAddress = (LPVOID)param;
								param = (UINT_PTR)NULL;
								hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_VM_READ);
								if (hProcess)
								{
									do	// do { ... } while (0);
									{
										size_t size;

										if (type == 's')
										{
											size = StringLengthA(hProcess, (LPCSTR)readAddress, -1);
											param = (UINT_PTR)HeapAlloc(hHeap, 0, size + sizeof(char));
											if (!param)
												break;
											*((char *)(param + size)) = '\0';
										}
										else
										{
											size = StringLengthW(hProcess, (LPCWSTR)readAddress, -1) * sizeof(wchar_t);
											param = (UINT_PTR)HeapAlloc(hHeap, 0, size + sizeof(wchar_t));
											if (!param)
												break;
											*((wchar_t *)(param + size)) = L'\0';
										}
										if (isAllocated = ReadProcessMemory(hProcess, readAddress, (LPVOID)param, size, NULL))
											break;
										HeapFree(hHeap, 0, (LPVOID)param);
										param = (UINT_PTR)NULL;
									} while (0);
									CloseHandle(hProcess);
								}
							}
							else
							{
								if ((type == 's' ? (BOOL (WINAPI *)(UINT_PTR, UINT_PTR))IsBadStringPtrA : (BOOL (WINAPI *)(UINT_PTR, UINT_PTR))IsBadStringPtrW)(param, MAXUINT_PTR))
									param = (UINT_PTR)NULL;
							}
						}
						else
						{
							LPSTR readBuffer;

							readBuffer = NULL;
							do	// do { ... } while (0);
							{
								LPCSTR lpMultiByteStr;
								int    cchWideChar;

								lpMultiByteStr = (LPCSTR)param;
								param = (UINT_PTR)NULL;
								if (TSSGCtrl_IsRemoteProcess(valueBegin))
								{
									HANDLE hProcess;
									size_t length;

									if (!(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_VM_READ)))
										break;
									length = StringLengthA(hProcess, lpMultiByteStr, -1);
									readBuffer = (LPSTR)HeapAlloc(hHeap, 0, length + 1);
									if (!readBuffer)
										break;
									if (!ReadProcessMemory(hProcess, lpMultiByteStr, readBuffer, length, NULL))
										break;
									readBuffer[length] = '\0';
									lpMultiByteStr = readBuffer;
								}
								else if (IsBadStringPtrA(lpMultiByteStr, MAXUINT_PTR))
									break;
								if (!(cchWideChar = MultiByteToWideChar(CP_UTF8, 0, lpMultiByteStr, -1, NULL, 0)))
									break;
								if (type == 's')
								{
									LPWSTR lpWideCharStr;
									int    cchMultiByte;

									if (!(lpWideCharStr = (LPWSTR)HeapAlloc(hHeap, 0, (size_t)cchWideChar * sizeof(wchar_t))))
										break;
									MultiByteToWideChar(CP_UTF8, 0, lpMultiByteStr, -1, lpWideCharStr, cchWideChar);
									if (cchMultiByte = WideCharToMultiByte(CP_THREAD_ACP, 0, lpWideCharStr, cchWideChar, NULL, 0, NULL, NULL))
										if (isAllocated = !!(param = (UINT_PTR)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, cchMultiByte)))
											WideCharToMultiByte(CP_THREAD_ACP, 0, lpWideCharStr, cchWideChar, (LPSTR)param, cchMultiByte, NULL, NULL);
									HeapFree(hHeap, 0, lpWideCharStr);
								}
								else if (isAllocated = !!(param = (UINT_PTR)HeapAlloc(hHeap, 0, (size_t)cchWideChar * sizeof(wchar_t))))
									MultiByteToWideChar(CP_UTF8, 0, lpMultiByteStr, -1, (LPWSTR)param, cchWideChar);
							} while (0);
							if (readBuffer)
								HeapFree(hHeap, 0, readBuffer);
						}
					}
					if (formatBegin && type)
						*formatEnd = '\0';
					else
						formatBegin = "%d";
					length = _snprintf(stackBuffer, _countof(stackBuffer), formatBegin, param);
					buffer = stackBuffer;
					if (length >= _countof(stackBuffer))
					{
						if ((int)length >= 0)
						{
							UINT capacity;

							if (buffer = (char *)HeapAlloc(hHeap, 0, capacity = length + 1))
							{
								if ((length = _snprintf(buffer, capacity, formatBegin, param)) >= capacity)
									length = (int)length >= 0 ? capacity - 1 : 0;
							}
							else
							{
								buffer = stackBuffer;
								length = _countof(stackBuffer) - 1;
							}
						}
						else
						{
							length = 0;
						}
					}
					if (isAllocated)
						HeapFree(hHeap, 0, (LPVOID)param);
					bracketEnd = ReplaceString(s, bracketBegin, bracketEnd, buffer, buffer + length);
					if (buffer != stackBuffer)
						HeapFree(hHeap, 0, buffer);
				}
				break;
			}

			#undef FEP
			#undef UTF8
		}
		else if (bracketBegin[1] == LIST_IDENTIFIER)
		{
			char    *fileNameBegin, *fileNameEnd, *indexBegin, *indexEnd;
			BOOLEAN isFEP;

			bracketEnd = FindDoubleChar(bracketBegin + 2, LIST_CLOSE);
			if (!bracketEnd)
				break;
			fileNameBegin = NULL;
			indexBegin = NULL;
			isFEP = FALSE;
			do	/* do { ... } while (0); */
			{
				char *begin, *end;

				begin = TrimLeftBlank(bracketBegin + 2);
				end = TrimRightBlank(begin, bracketEnd);
				bracketEnd += 2;
				if (begin == end)
					break;
				fileNameBegin = begin;
				begin = FindDelimiter(begin, end);
				fileNameEnd = TrimRightBlank(fileNameBegin, begin);
				if (begin == end)
					break;
				begin = TrimLeftBlank(begin + 1);
				if (begin == end)
					break;
				indexBegin = begin;
				begin = FindDelimiter(begin, end);
				indexEnd = TrimRightBlank(indexBegin, begin);
				if (begin == end)
					break;
				begin = TrimLeftBlank(begin + 1);
				if (end - begin != 3)
					break;
				if (begin[0] != 'f' || begin[1] != 'e' || begin[2] != 'p')
					break;
				isFEP = TRUE;
			} while (0);
			if (fileNameBegin)
			{
				LPCSTR        lpcszDotLst = (LPCSTR)0x00631C0D;
				char          prefix;
				string        FName;
				string        DefaultExt;
				vector_string *file;
				size_t        count;
				unsigned long index;
				string        src;
				char          *begin, *end;

				prefix = *fileNameBegin;
				if (prefix == '+' || prefix == '*')
					fileNameBegin++;
				string_ctor_assign_range(&FName, fileNameBegin, fileNameEnd);
				string_ctor_assign_cstr_with_length(&DefaultExt, lpcszDotLst, 4);
				file = TSSGCtrl_GetSSGDataFile(this, SSGS, FName, DefaultExt, NULL);
				if (file == NULL)
					break;
				count = vector_size(file);
				if (count == 0)
					break;
				if (indexBegin)
				{
					string s;

					indexEnd = UnescapeParam(indexBegin, indexEnd);
					string_begin(&s) = indexBegin;
					string_end_of_storage(&s) = string_end(&s) = indexEnd;
					index = Parsing(this, SSGS, &s, 0);
					if (isFEP)
						index = TSSGCtrl_CheckIO_FEP(this, SSGS, index, FALSE);
				}
				else
				{
					TSSGSubjectProperty *prop;

					prop = GetSubjectProperty(SSGS);
					if (prop == NULL)
						break;
					index = prop->RepeatIndex;
				}
				string_ctor_assign(&src, &vector_at(file, index % count));
				ReplaceDefineDynamic(SSGS, &src);
				begin = string_begin(&src);
				end = string_end(&src);
				if (prefix == '+')
				{
					while (*begin && *(begin++) != '=');
					begin = TrimLeftBlank(begin);
				}
				else if (prefix == '*')
				{
					end = begin;
					while (*end && *end != '=')
						end++;
					end = TrimRightBlank(begin, end);
				}
				bracketEnd = ReplaceString(s, bracketBegin, bracketEnd, begin, end);
				string_dtor(&src);
			}
		}
		else
		{
			bracketEnd = bracketBegin + 1;
		}
		bracketBegin = FindBracketOpen(bracketEnd);
	}

	#undef NUMBER_IDENTIFIER
	#undef LIST_IDENTIFIER
	#undef NUMBER_CLOSE
	#undef LIST_CLOSE
}

#undef BRACKET_OPEN
#undef BRACKET_CLOSE
