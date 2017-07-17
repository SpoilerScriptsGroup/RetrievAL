#define _CRT_NONSTDC_NO_WARNINGS

#ifdef __BORLANDC__
#define bcb6_std_string_empty(s)             (s)->empty()
#define bcb6_std_string_length(s)            (s)->length()
#define bcb6_std_string_begin(s)             (s)->begin()
#define bcb6_std_string_end(s)               (s)->end()
#define bcb6_std_string_assign_cstr(s, cstr) *(s) = (cstr);
#else
#include <windows.h>
#include "bcb6_std_string.h"
#endif

#include <mbstring.h>
#include "intrinsic.h"
#include "tlhelp32fix.h"

#include "ProcessMonitor.h"
#include "FindWindowContainsModule.h"
#include "GetFileTitlePointer.h"

extern HANDLE hHeap;

static char * __fastcall FindInvalidChar(const char *string)
{
	while (*string)
	{
		if (!__intrinsic_isleadbyte(*string))
		{
			switch (*string)
			{
			case '\\':
			case '/':
			case ':':
			case '*':
			case '?':
			case '"':
			case '<':
			case '>':
			case '|':
				return (char *)string;
			}
		}
		else
		{
			if (!*(++string))
				break;
		}
		string++;
	}
	return NULL;
}

static char ** __stdcall ParseArgument(const char *begin, const char *end, size_t *argc)
{
	*argc = 0;
	while (*begin == ' ')
		begin++;
	if (end > begin)
	{
		size_t length, maxArgCount, capacity;
		char   **argv;

		length = end - begin;
		maxArgCount = (length + 1) / 2;
		capacity = maxArgCount * sizeof(char *) + (length + 1) * sizeof(char);
		argv = (char **)HeapAlloc(hHeap, 0, capacity);
		if (argv)
		{
			char    *p;
			BOOLEAN inDoubleQuote;
			size_t  index;

			p = (char *)(argv + maxArgCount);
			*(char *)(end = p + length) = '\0';
			memcpy(p, begin, length * sizeof(char));
			inDoubleQuote = FALSE;
			argv[index = 0] = p;
			while (*p)
			{
				if (!__intrinsic_isleadbyte(*p))
				{
					switch (*p)
					{
					case ' ':
						if (inDoubleQuote)
							break;
						*p = '\0';
						while (*(++p) == ' ');
						if (*p)
							argv[++index] = p;
						continue;
					case '"':
						inDoubleQuote = !inDoubleQuote;
						memcpy(p, p + 1, ((end--) - p) * sizeof(char));
						continue;
					case '\\':
						length = 0;
						while (p[++length] == '\\');
						if (p[length] == '"')
						{
							const char *src;

							length = (length + 1) / 2;
							src = p + length;
							memcpy(p, src, (end - src + 1) * sizeof(char));
							end -= length;
						}
						p += length;
						continue;
					}
				}
				else if (!*(++p))
					break;
				p++;
			}
			*argc = index + 1;
			return argv;
		}
	}
	return NULL;
}

#ifdef __BORLANDC__
unsigned long TProcessCtrl::FindProcess(string *ProcessName, PROCESSENTRY32 *Entry)
#else
unsigned long __cdecl TProcessCtrl_FindProcess(LPVOID _this, bcb6_std_string *ProcessName, PROCESSENTRY32A *Entry)
#endif
{
	#define CLASSNAME_BRACKET_OPEN  '<'
	#define CLASSNAME_BRACKET_CLOSE '>'
	#define MODULENAME_DELIMITER    ':'
	#define WINDOWNAME_DELIMITER    '*'
	#define OPTION_DELIMITER        '/'

	do	/* do { ... } while (0); */
	{
		DWORD           dwProcessId;
		HANDLE          hSnapshot;
		PROCESSENTRY32A pe;

		if (bcb6_std_string_empty(ProcessName))
			break;
		if (!FindInvalidChar(bcb6_std_string_begin(ProcessName)))
		{
			dwProcessId = FindProcessId(FALSE, bcb6_std_string_begin(ProcessName), bcb6_std_string_length(ProcessName), NULL);
		}
		else
		{
			char   **argv;
			size_t argc;

			argv = ParseArgument(bcb6_std_string_begin(ProcessName), bcb6_std_string_end(ProcessName), &argc);
			if (!argv)
				break;
			dwProcessId = 0;
			do	/* do { ... } while (0); */
			{
				BOOL   bIsRegex;
				LPSTR  lpProcessName;
				LPSTR  lpClassName;
				LPSTR  lpWindowName;
				LPSTR  lpModuleName;
				size_t length;

				bIsRegex = FALSE;
				lpProcessName = NULL;
				lpClassName = NULL;
				lpWindowName = NULL;
				lpModuleName = NULL;
				for (size_t i = 0; i < argc; i++)
				{
					switch (*argv[i])
					{
					case CLASSNAME_BRACKET_OPEN:
						if (lpClassName)
							break;
						length = strlen(argv[i] + 1);
						if (!length || argv[i][length] != CLASSNAME_BRACKET_CLOSE)
							break;
						argv[i][length] = '\0';
						lpClassName = argv[i] + 1;
						break;
					case WINDOWNAME_DELIMITER:
						if (!lpWindowName)
							lpWindowName = argv[i] + 1;
						break;
					case MODULENAME_DELIMITER:
						if (!lpModuleName)
							lpModuleName = argv[i] + 1;
						break;
					case OPTION_DELIMITER:
						if (stricmp(argv[i] + 1, "regex") == 0)
							bIsRegex = TRUE;
						break;
					default:
						if (!lpProcessName)
							lpProcessName = argv[i];
						break;
					}
				}
				if (lpProcessName || (lpModuleName && !lpClassName && !lpWindowName))
				{
					dwProcessId = FindProcessId(bIsRegex, lpProcessName, lpProcessName ? strlen(lpProcessName) : 0, lpModuleName);
				}
				else if (lpClassName || lpWindowName)
				{
					if (FindWindowContainsModule(bIsRegex, lpClassName, lpWindowName, lpModuleName, &dwProcessId))
						StopProcessMonitor();
				}
			} while (0);
			HeapFree(hHeap, 0, argv);
		}
		if (!dwProcessId)
			break;
		hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (hSnapshot == INVALID_HANDLE_VALUE)
			break;
		pe.dwSize = sizeof(PROCESSENTRY32A);
		if (Process32FirstA(hSnapshot, &pe))
		{
			do
			{
				if (pe.th32ProcessID != dwProcessId)
					continue;
				CloseHandle(hSnapshot);
				bcb6_std_string_assign_cstr(ProcessName, GetFileTitlePointerA(pe.szExeFile));
				*Entry = pe;
				return 0;
			} while (Process32NextA(hSnapshot, &pe));
		}
		CloseHandle(hSnapshot);
	} while (0);
	return 1;

	#undef CLASSNAME_BRACKET_OPEN
	#undef CLASSNAME_BRACKET_CLOSE
	#undef MODULENAME_DELIMITER
	#undef WINDOWNAME_DELIMITER
	#undef OPTION_DELIMITER
}
