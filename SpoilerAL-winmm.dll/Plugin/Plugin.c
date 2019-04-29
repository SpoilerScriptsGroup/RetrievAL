#include "plugin.h"
#include "intrinsic.h"
#include "TMainForm.h"
#include "TSSGActionListner.h"

extern HANDLE hHeap;
extern BOOL ParsingContinue;

static HMODULE *Plugins = NULL;
static HMODULE *EndOfPlugins = NULL;

PLUGIN_FUNCTION        *PluginFunctions = NULL;
PLUGIN_FUNCTION        *EndOfPluginFunctions = NULL;
PLUGIN_FUNCTION_VECTOR PluginFunctionVector[256];

static void __stdcall Guide(LPCSTR Message)
{
	TMainForm_Guide(Message, FALSE);
}

static void __stdcall OnParsingProcess(LPCSTR Message, unsigned __int64 Value)
{
	TSSGActionListner_OnParsingProcess(Message, strlen(Message), Value);
}

static void __stdcall OnParsingDoubleProcess(LPCSTR Message, double Value)
{
	TSSGActionListner_OnParsingDoubleProcess(Message, strlen(Message), Value);
}

static void __stdcall OnProcessOpenError()
{
	TSSGActionListner_OnProcessOpenError(TSSGCtrl_GetSSGActionListner(&MainForm->ssgCtrl), NULL);
}

static void __stdcall OnSubjectReadError(LPCVOID Address)
{
	TSSGActionListner_OnSubjectReadError(TSSGCtrl_GetSSGActionListner(&MainForm->ssgCtrl), NULL, (DWORD)Address);
}

static void __stdcall OnSubjectWriteError(LPCVOID Address)
{
	TSSGActionListner_OnSubjectWriteError(TSSGCtrl_GetSSGActionListner(&MainForm->ssgCtrl), NULL, (DWORD)Address);
}

static void __stdcall OnParsingError(LPCSTR Message)
{
	TSSGActionListner_OnParsingError(TSSGCtrl_GetSSGActionListner(&MainForm->ssgCtrl), NULL, Message);
}

#pragma pack(push, 1)
typedef struct {
	struct {
		BYTE Major;
		BYTE Minor;
		BYTE Build;
		BYTE Revision;
	}              Version;
	DWORD          Reserved1;
	DWORD          Reserved2;
	DWORD          Reserved3;
	BOOL           *ParsingContinue;
	void(__stdcall *Guide)(LPCSTR Message);
	void(__stdcall *OnParsingProcess)(LPCSTR Message, unsigned __int64 Value);
	void(__stdcall *OnParsingDoubleProcess)(LPCSTR Message, double Value);
	void(__stdcall *OnProcessOpenError)();
	void(__stdcall *OnSubjectReadError)(LPCVOID Address);
	void(__stdcall *OnSubjectWriteError)(LPCVOID Address);
	void(__stdcall *OnParsingError)(LPCSTR Message);
} PLUGIN_INTERFACE;
#pragma pack(pop)

static const PLUGIN_INTERFACE PluginInterface = {
	{ 1, 0, 0, 0 },
	0,
	0,
	0,
	&ParsingContinue,
	Guide,
	OnParsingProcess,
	OnParsingDoubleProcess,
	OnProcessOpenError,
	OnSubjectReadError,
	OnSubjectWriteError,
	OnParsingError,
};

static __inline size_t EnumNamedFunctions(HMODULE hModule)
{
	const PLUGIN_TYPE       *PluginType;
	LPVOID                  ProcAddress;
	PIMAGE_DOS_HEADER       DosHeader;
	PIMAGE_NT_HEADERS       NtHeaders;
	PIMAGE_DATA_DIRECTORY   DataDirectory;
	PIMAGE_EXPORT_DIRECTORY ExportDirectory;
	size_t                  GrowSize, AppendSize;
	PLUGIN_FUNCTION         *Function;
	LPDWORD                 AddressOfFunctions, AddressOfNames;
	LPWORD                  AddressOfNameOrdinals;
	DWORD                   i;

	if (!(PluginType = (const PLUGIN_TYPE *)GetProcAddress(hModule, "PluginType")))
		return 0;
	if (*PluginType != PLUGIN_PARSING)
		return 0;
	if (ProcAddress = GetProcAddress(hModule, "PluginInterface"))
		*(const PLUGIN_INTERFACE **)ProcAddress = &PluginInterface;
	DosHeader = (PIMAGE_DOS_HEADER)hModule;
	if (DosHeader->e_magic != IMAGE_DOS_SIGNATURE)
		return 0;
	NtHeaders = (PIMAGE_NT_HEADERS)((LPCBYTE)hModule + DosHeader->e_lfanew);
	if (NtHeaders->Signature != IMAGE_NT_SIGNATURE)
		return 0;
	if (NtHeaders->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC)
		DataDirectory = &((PIMAGE_OPTIONAL_HEADER32)&NtHeaders->OptionalHeader)->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
#ifdef _WIN64
	else if (NtHeaders->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC)
		DataDirectory = &((IMAGE_OPTIONAL_HEADER64)&NtHeaders->OptionalHeader)->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
#endif
	else
		return 0;
	if (!DataDirectory->VirtualAddress || DataDirectory->Size < sizeof(IMAGE_EXPORT_DIRECTORY))
		return 0;
	ExportDirectory = (PIMAGE_EXPORT_DIRECTORY)((LPCBYTE)hModule + DataDirectory->VirtualAddress);
	if (ExportDirectory->NumberOfNames <= 1)
		return 0;
	GrowSize = (ExportDirectory->NumberOfNames - 1) * sizeof(PLUGIN_FUNCTION);
	if (PluginFunctions)
	{
		size_t Size;
		LPVOID MemBlock;

		Size = (LPBYTE)EndOfPluginFunctions - (LPBYTE)PluginFunctions;
		MemBlock = HeapReAlloc(hHeap, 0, PluginFunctions, Size + GrowSize);
		if (!MemBlock)
			return 0;
		PluginFunctions = (PLUGIN_FUNCTION *)MemBlock;
		EndOfPluginFunctions = (PLUGIN_FUNCTION *)((LPBYTE)MemBlock + Size);
	}
	else
	{
		PluginFunctions = (PLUGIN_FUNCTION *)HeapAlloc(hHeap, 0, GrowSize);
		if (!PluginFunctions)
			return 0;
		EndOfPluginFunctions = PluginFunctions;
	}
	Function = EndOfPluginFunctions;
	AddressOfFunctions = (LPDWORD)((LPBYTE)hModule + ExportDirectory->AddressOfFunctions);
	AddressOfNames = (LPDWORD)((LPBYTE)hModule + ExportDirectory->AddressOfNames);
	AddressOfNameOrdinals = (LPWORD)((LPBYTE)hModule + ExportDirectory->AddressOfNameOrdinals);
	i = 0;
	do	// do { ... } while (0);
	{
		#pragma pack(push, 1)
		typedef struct {
			LPCVOID     Address;
			RETURN_TYPE ReturnType;
			PARAM_TYPE  ParamTypes[1];
		} IMPORT_FUNCTION;
		#pragma pack(pop)

		LPCSTR          Name;
		size_t          NameLength;
		IMPORT_FUNCTION *Import;
		size_t          StackSize;
		PARAM_TYPE      *EndOfParamTypes;

		Name = (LPCSTR)hModule + AddressOfNames[i];
		if (Name[ 0] != 'P' ||
			Name[ 1] != 'l' ||
			Name[ 2] != 'u' ||
			Name[ 3] != 'g' ||
			Name[ 4] != 'i' ||
			Name[ 5] != 'n' ||
			Name[ 6] != 'F' ||
			Name[ 7] != 'u' ||
			Name[ 8] != 'n' ||
			Name[ 9] != 'c' ||
			Name[10] != 't' ||
			Name[11] != 'i' ||
			Name[12] != 'o' ||
			Name[13] != 'n' ||
			Name[14] != '_' ||
			Name[15] == '\0')
			continue;
		NameLength = strlen(Name += 15);
		Import = (IMPORT_FUNCTION *)((LPBYTE)hModule + AddressOfFunctions[AddressOfNameOrdinals[i]]);
		StackSize = 0;
		EndOfParamTypes = Import->ParamTypes;
		while (*EndOfParamTypes != END_OF_PARAMS)
		{
#ifndef _WIN64
			StackSize +=
				*EndOfParamTypes != PARAM_QWORD && *EndOfParamTypes != PARAM_DOUBLE ?
					4 :
					8;
#else
			StackSize += 8;
#endif
			EndOfParamTypes++;
		}
		Function->Name = Name;
		Function->NameLength = NameLength;
		Function->Address = Import->Address;
		Function->ReturnType = Import->ReturnType;
		Function->StackSize = StackSize;
		Function->ParamTypes = Import->ParamTypes;
		Function->EndOfParamTypes = EndOfParamTypes;
		Function++;
	} while (++i != ExportDirectory->NumberOfNames);
	AppendSize = (LPBYTE)Function - (LPBYTE)EndOfPluginFunctions;
	EndOfPluginFunctions = Function;
	if (AppendSize != GrowSize)
	{
		size_t Size;

		if (Size = (LPBYTE)EndOfPluginFunctions - (LPBYTE)PluginFunctions)
		{
			PluginFunctions = (PLUGIN_FUNCTION *)HeapReAlloc(hHeap, 0, PluginFunctions, Size);
			EndOfPluginFunctions = (PLUGIN_FUNCTION *)((LPBYTE)PluginFunctions + Size);
		}
		else
		{
			HeapFree(hHeap, 0, PluginFunctions);
			EndOfPluginFunctions = PluginFunctions = NULL;
		}
	}
	return AppendSize;
}

static __inline void LoadPlugin(const wchar_t PluginName[MAX_PATH])
{
	HMODULE hModule;

	if (!(hModule = LoadLibraryW(PluginName)))
		return;
	do	// do { ... } while (0);
	{
		size_t Size;

		if (Plugins)
		{
			LPVOID MemBlock;

			Size = (LPBYTE)EndOfPlugins - (LPBYTE)Plugins;
			MemBlock = HeapReAlloc(hHeap, 0, Plugins, Size + sizeof(HMODULE));
			if (!MemBlock)
				break;
			Plugins = (HMODULE *)MemBlock;
			EndOfPlugins = (HMODULE *)((LPBYTE)MemBlock + Size);
		}
		else
		{
			Plugins = (HMODULE *)HeapAlloc(hHeap, 0, sizeof(HMODULE));
			if (!Plugins)
				break;
			EndOfPlugins = Plugins;
		}
		if (EnumNamedFunctions(hModule))
		{
			*(EndOfPlugins++) = hModule;
			return;
		}
		if (Size = (LPBYTE)EndOfPlugins - (LPBYTE)Plugins)
		{
			Plugins = (HMODULE *)HeapReAlloc(hHeap, 0, Plugins, Size);
			EndOfPlugins = (HMODULE *)((LPBYTE)Plugins + Size);
		}
		else
		{
			HeapFree(hHeap, 0, Plugins);
			EndOfPlugins = Plugins = NULL;
		}
	} while (0);
	FreeLibrary(hModule);
}

static int __cdecl CompareFunctionName(const void *a, const void *b)
{
	#define a ((PLUGIN_FUNCTION *)a)
	#define b ((PLUGIN_FUNCTION *)b)

	return memcmp(a->Name, b->Name, min(a->NameLength, b->NameLength) + 1);

	#undef a
	#undef b
}

BOOL __cdecl PluginInitialize(const wchar_t *DirectoryPath, const wchar_t *ProfileName)
{
	wchar_t          FullPath[MAX_PATH];
	wchar_t          *PluginPath;
	size_t           length;
	HANDLE           hFind;
	WIN32_FIND_DATAW wfd;

	if (!*ProfileName)
		return FALSE;
	length = wcslen(DirectoryPath);
	memcpy(FullPath, DirectoryPath, length * sizeof(wchar_t));
	PluginPath = FullPath + length;
	if (GetPrivateProfileStringW(L"Plugin" , L"Path", L"", PluginPath, _countof(FullPath) - length, ProfileName))
	{
		wchar_t *PluginName, *Separator, c;

		Separator = NULL;
		c = *(PluginName = PluginPath);
		do
			if (c == L'\\' || c == '/' || c == ':')
				Separator = PluginName;
		while (c = *(++PluginName));
		if (Separator == PluginName - 1)
			if (--PluginName == PluginPath)
				return FALSE;
		if (&FullPath[_countof(FullPath)] - PluginName <= 6)
			return FALSE;
		PluginPath = PluginName;
		*(PluginPath++) = L'\\';
	}
	else
	{
		if (_countof(FullPath) - length <= 12)
			return FALSE;
		*(LPDWORD) PluginPath      = MAKELONG(L'P', L'l');
		*(LPDWORD)(PluginPath + 2) = MAKELONG(L'u', L'g');
		*(LPDWORD)(PluginPath + 4) = MAKELONG(L'i', L'n');
		*         (PluginPath + 6) =          L'\\';
		PluginPath += 7;
	}
	*(LPDWORD) PluginPath      = MAKELONG(L'*', L'.');
	*(LPDWORD)(PluginPath + 2) = MAKELONG(L'd', L'l');
	*(LPDWORD)(PluginPath + 4) = MAKELONG(L'l', L'\0');
	hFind = FindFirstFileW(FullPath, &wfd);
	if (hFind == INVALID_HANDLE_VALUE)
		return FALSE;
	do
		if (!(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			if ((length = wcslen(wfd.cFileName)) < (size_t)(&FullPath[_countof(FullPath)] - PluginPath))
			{
				memcpy(PluginPath, wfd.cFileName, (length + 1) * sizeof(wchar_t));
				LoadPlugin(FullPath);
			}
	while (FindNextFileW(hFind, &wfd));
	FindClose(hFind);
	if (PluginFunctions)
	{
		size_t          i;
		PLUGIN_FUNCTION *Function;

		qsort(
			PluginFunctions,
			EndOfPluginFunctions - PluginFunctions,
			sizeof(PLUGIN_FUNCTION),
			CompareFunctionName);
		i = 0;
		do
		{
			PluginFunctionVector[i].First = NULL;
			PluginFunctionVector[i].Last = NULL;
		} while (++i != _countof(PluginFunctionVector));
		Function = PluginFunctions;
		do
		{
			i = *Function->Name;
			PluginFunctionVector[i].First = Function;
			while (++Function != EndOfPluginFunctions)
				if (*Function->Name != (wchar_t)i)
					break;
			PluginFunctionVector[i].Last = Function;
		} while (Function != EndOfPluginFunctions);
	}
	return TRUE;
}

void __cdecl PluginFinalize()
{
	if (PluginFunctions)
	{
		HeapFree(hHeap, 0, PluginFunctions);
		EndOfPluginFunctions = PluginFunctions = NULL;
	}
	if (EndOfPlugins)
	{
		HMODULE *Plugin = EndOfPlugins - 1;
		do
			FreeLibrary(*Plugin);
		while (Plugin-- != Plugins);
		HeapFree(hHeap, 0, Plugins);
		EndOfPlugins = Plugins = NULL;
	}
}

__declspec(naked) unsigned __int64 __cdecl CallPluginFunction(const void *Address, const void *Stack, size_t StackSize)
{
	__asm
	{
		#define Address   (ebp + 16)
		#define Stack     (ebp + 20)
		#define StackSize (esp + 12)

		mov     ecx, dword ptr [StackSize]
		push    ebp
		push    esi
		push    edi
		mov     ebp, esp
		sub     esp, ecx
		mov     esi, dword ptr [Stack]
		mov     edi, esp
		rep movsb
		call    dword ptr [Address]
		mov     edi, dword ptr [ebp]
		lea     esp, [ebp + 4]
		pop     esi
		pop     ebp
		ret

		#undef Address
		#undef Stack
		#undef StackSize
	}
}
