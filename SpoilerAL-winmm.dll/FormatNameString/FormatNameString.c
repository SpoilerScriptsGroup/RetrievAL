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
#include "SSGSubjectProperty.h"
#include "IsBadPtr.h"

EXTERN_C HANDLE hHeap;
EXTERN_C const DWORD F00504284;

void __stdcall ReplaceDefineDynamic(TSSGSubject *SSGS, string *line);
unsigned long __cdecl Parsing(IN TSSGCtrl *this, IN TSSGSubject *SSGS, IN const string *Src, ...);
double __cdecl ParsingDouble(IN TSSGCtrl *this, IN TSSGSubject *SSGS, IN const string *Src, IN double Val);
char * __fastcall Unescape(char *first, char *last);
size_t __stdcall StringLengthA(HANDLE hProcess, LPCSTR lpString);
size_t __stdcall StringLengthW(HANDLE hProcess, LPCWSTR lpString);
extern char * __fastcall TrimLeft(const char *first);
extern char * __fastcall TrimRight(const char *first, const char *last);

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
		count = s->_M_finish - destEnd + 1;
		if (srcLength > destLength)
		{
			destBegin -= (size_t)s->_M_start;
			destEnd -= (size_t)s->_M_start;
			string_resize(s, string_length(s) + diff);
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

void __stdcall FormatNameString(TSSGCtrl *this, TSSGSubject *SSGS, string *s)
{
	#define NUMBER_IDENTIFIER '#'
	#define LIST_IDENTIFIER   '@'
	#define NUMBER_CLOSE      (WORD)(NUMBER_IDENTIFIER | (WORD)BRACKET_CLOSE << 8)
	#define LIST_CLOSE        (WORD)(LIST_IDENTIFIER   | (WORD)BRACKET_CLOSE << 8)

	char stackBuffer[256];
	char *bracketBegin;

	ReplaceDefineDynamic(SSGS, s);
	bracketBegin = FindBracketOpen(s->_M_start);
	while (bracketBegin)
	{
		char *bracketEnd;

		if (bracketBegin[1] == NUMBER_IDENTIFIER)
		{
			char    *valueBegin, *valueEnd, *formatBegin, *formatEnd, type;
			BOOLEAN isFEP;

			bracketEnd = FindDoubleChar(bracketBegin + 2, NUMBER_CLOSE);
			if (!bracketEnd)
				break;
			formatBegin = NULL;
			type = '\0';
			isFEP = FALSE;
			do	/* do { ... } while (0); */
			{
				char *term, *fepBegin, *fepEnd;

				valueBegin = TrimLeft(bracketBegin + 2);
				valueEnd = TrimRight(valueBegin, bracketEnd);
				bracketEnd += 2;
				if (valueEnd == valueBegin)
					break;
				term = valueEnd;
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
			switch (type)
			{
			case 'e': case 'E': case 'f': case 'g': case 'G': case 'a': case 'A':
				{
					double number;
					string src;
					UINT   length;
					char   *buffer;

					*valueEnd = '\0';
					valueEnd = Unescape(valueBegin, valueEnd);
					src._M_start = valueBegin;
					src._M_end_of_storage = src._M_finish = valueEnd;
					number = ParsingDouble(this, SSGS, &src, 0);
					if (isFEP)
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

					*valueEnd = '\0';
					Unescape(valueBegin, valueEnd);
					src._M_start = valueBegin;
					src._M_end_of_storage = src._M_finish = valueEnd;
					param = Parsing(this, SSGS, &src, 0);
					if (isFEP)
						param = TSSGCtrl_CheckIO_FEP(this, SSGS, param, FALSE);
					isAllocated = FALSE;
					if (type == 's' || type == 'S')
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
										size = StringLengthA(hProcess, (LPCSTR)readAddress);
										param = (UINT_PTR)HeapAlloc(hHeap, 0, size + sizeof(char));
										if (!param)
											break;
										*((char *)(param + size)) = '\0';
									}
									else
									{
										size = StringLengthW(hProcess, (LPCWSTR)readAddress) * sizeof(wchar_t);
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
		}
		else if (bracketBegin[1] == LIST_IDENTIFIER)
		{
			char *fileNameBegin, *fileNameEnd, *indexBegin, *indexEnd;

			bracketEnd = FindDoubleChar(bracketBegin + 2, LIST_CLOSE);
			if (!bracketEnd)
				break;
			fileNameBegin = NULL;
			indexBegin = NULL;
			do	/* do { ... } while (0); */
			{
				char *begin, *end;

				begin = TrimLeft(bracketBegin + 2);
				end = TrimRight(begin, bracketEnd);
				bracketEnd += 2;
				if (begin == end)
					break;
				fileNameBegin = begin;
				begin = FindDelimiter(begin, end);
				fileNameEnd = TrimRight(fileNameBegin, begin);
				if (begin == end)
					break;
				begin = TrimLeft(begin + 1);
				if (begin == end)
					break;
				indexBegin = begin;
				indexEnd = end;
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

					*indexEnd = '\0';
					indexEnd = Unescape(indexBegin, indexEnd);
					s._M_start = indexBegin;
					s._M_end_of_storage = s._M_finish = indexEnd;
					index = Parsing(this, SSGS, &s, 0);
				}
				else
				{
					TSSGSubjectProperty *prop;

					prop = GetSubjectProperty(SSGS);
					if (prop == NULL)
						break;
					index = prop->RepeatIndex;
				}
				string_ctor_assign(&src, &((string *)file->_M_start)[index % count]);
				ReplaceDefineDynamic(SSGS, &src);
				begin = src._M_start;
				end = src._M_finish;
				if (prefix == '+')
				{
					while (*begin && *(begin++) != '=');
					begin = TrimLeft(begin);
				}
				else if (prefix == '*')
				{
					end = begin;
					while (*end && *end != '=')
						end++;
					end = TrimRight(begin, end);
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
