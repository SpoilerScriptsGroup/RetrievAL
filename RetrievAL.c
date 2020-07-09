// RetrievAL.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "framework.h"
#include "SpoilerAL-winmm.dll/crc32/crc32.h"

#pragma comment(linker, "/NODEFAULTLIB:vcruntimed")
#pragma comment(linker, "/NODEFAULTLIB:vcruntime")
#pragma comment(linker, "/NODEFAULTLIB:OLDNAMES")
#pragma comment(linker, "/NODEFAULTLIB:MSVCRTD")
#pragma comment(linker, "/NODEFAULTLIB:MSVCRT")
#pragma comment(linker, "/NODEFAULTLIB:libcmt")
#pragma comment(linker, "/NODEFAULTLIB:libcmtd")
#pragma comment(linker, "/NODEFAULTLIB:libucrt")
#pragma comment(linker, "/NODEFAULTLIB:libucrtd")
#pragma comment(linker, "/NODEFAULTLIB:libvcruntime")
#pragma comment(linker, "/NODEFAULTLIB:libvcruntimed")

#pragma comment(lib, "Shlwapi")

#ifdef NDEBUG
#pragma comment(lib, "ucrt")
#else
#pragma comment(lib, "ucrtd")
#endif

#pragma comment(lib, "SpoilerAL")
#pragma comment(linker, "/INCLUDE:___GetExceptDLLinfo")

#if 0
#pragma comment(linker,"\"/MANIFESTDEPENDENCY:type='win32'\
 name='Microsoft.Windows.Common-Controls' version='6.0.0.0'\
 processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(lib, "../winmm")
#endif
#pragma comment(linker, "/SECTION:.text,D!P")

static void __fastcall _guard_check_icall_nop(uintptr_t Target) {
	UNREFERENCED_PARAMETER(Target);
	return;
}

static BOOL WINAPI __dyn_tls_init(HANDLE, DWORD, LPVOID);

#ifdef __cplusplus
extern "C" {
#endif

	ULONG _tls_index = 0;

#ifndef NDEBUG

#pragma data_seg(push, ".data$!")
	static PIMAGE_TLS_CALLBACK __xl[0x30] = {
		(PIMAGE_TLS_CALLBACK)__dyn_tls_init,
		NULL,
		NULL,// sentinel
	};
#pragma data_seg(pop)

#pragma data_seg(push, ".rdata$T")
	extern const IMAGE_TLS_DIRECTORY _tls_used = {
		/* start of tls data          */(ULONG)&__xl[3],
		/* end of tls data            */(ULONG)&__xl[_countof(__xl)],
		/* address of tls_index       */(ULONG)&_tls_index,
		/* pointer to call back array */(ULONG)&__xl,
		/* size of tls zero fill      */0,
		/* characteristics            */0// overridden to ".*tls" section's alignment by linker
	};
#pragma comment(linker, "/INCLUDE:__tls_used")
#pragma data_seg(pop)

#endif

#pragma const_seg(push, ".00cfg")
	DECLSPEC_SELECTANY extern const volatile PVOID __guard_check_icall_fptr = (PVOID)_guard_check_icall_nop;
#pragma const_seg(pop)

#pragma data_seg(push, ".stls$")
	DECLSPEC_SELECTANY extern IMAGE_LOAD_CONFIG_DIRECTORY _load_config_used = {
		sizeof(IMAGE_LOAD_CONFIG_DIRECTORY),
	};
#pragma data_seg(pop)


#pragma pack(push, 1)

#pragma bss_seg(push, ".textbss$0")
	__declspec(align(1)) __int8 _text0[0x04A0];
#pragma comment(linker, "/INCLUDE:__text0")
#pragma bss_seg(pop)

#pragma bss_seg(push, ".textbss$1")
	__declspec(align(1)) __int8 _entry[0x00202000 - 0x1000 - sizeof(_text0)];
#pragma comment(linker, "/INCLUDE:__entry")
#pragma bss_seg(pop)

#pragma bss_seg(push, ".udata$A")
	__declspec(align(1)) __int8 _dataA[0x0020208B - 0x1000 - sizeof(_text0) - sizeof(_entry)];
#pragma comment(linker, "/INCLUDE:__dataA")

#pragma bss_seg(push, ".udata$B")
	__declspec(align(1)) union {
		ULONG  $;
		__int8 _dataB[0x0025436C - 0x1000 - sizeof(_text0) - sizeof(_entry) - sizeof(_dataA)];
	} tlsidx;
#pragma comment(linker, "/INCLUDE:_tlsidx")
#pragma bss_seg(pop)

#pragma bss_seg(push, ".udata$C")
	__declspec(align(1)) union {
		FARPROC $;
		__int8  _dataC[0x00270000 - 0x1000 - sizeof(_text0) - sizeof(_entry) - sizeof(_dataA) - sizeof(tlsidx)];
	} fnexit;
#pragma comment(linker, "/INCLUDE:_fnexit")
#pragma bss_seg(pop)

#pragma pack(pop)


#ifdef __cplusplus
}
#endif

#pragma comment(linker, "/SECTION:.textbss,ERW!P")

#pragma code_seg(push, ".text$!")

__declspec(noinline, naked)
int WinMainCRTStartup(void) {
	__asm {
#ifdef NDEBUG
		push  0
		call  GetModuleHandleW
		push  0
		push  DLL_PROCESS_ATTACH
		push  eax
		call  __dyn_tls_init
		test  eax, eax
		jz    FAIL
		mov   dword ptr [fnexit], offset EXIT
#endif// fail-safe trick
		mov   eax, offset _entry + 0x0E
		jmp        offset _entry
#ifdef NDEBUG
	FAIL:
		jmp   GetLastError

		align 16
	EXIT:// via _terminate(errcode)
		push  0
		call  GetModuleHandleW
		push  0
		push  DLL_PROCESS_DETACH
		push  eax
		call  _DllMainCRTStartup
		jmp   ExitProcess
#endif
		ud2
	}
}

#pragma intrinsic(memset)
BOOL WINAPI __dyn_tls_init(
	HANDLE hInstance,
	DWORD  fdwReason,
	LPVOID lpvReserved
) {// terminate on any C++ exception that leaves a namespace-scope thread-local initializer
	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		WIN32_FILE_ATTRIBUTE_DATA attr;
		PBYTE it, ib;
		PIMAGE_NT_HEADERS32 nt;
		PIMAGE_SECTION_HEADER sec;
		DWORD flOldProtect, lpflOldProtect[7];
		WCHAR pszPath[MAX_PATH], lpString[0x80] = { 0, 0 };
#ifndef NDEBUG
		struct excDLLinfoRec info;
		__GetExceptDLLinfo(&info);
#elif TRUE// delay import by MSVC
		if (GetModuleFileNameW((HMODULE)hInstance, pszPath, _countof(pszPath))
			&& PathRemoveFileSpecW(pszPath)
			&& SetCurrentDirectoryW(pszPath)
			&& PathAppendW(pszPath, L"winmm.dll")
			&& PathFileExistsW(pszPath))
		{
			STARTUPINFOW start = { sizeof(STARTUPINFOW) };
			PROCESS_INFORMATION info;
			if (CreateProcessW(L"SpoilerAL.exe", NULL, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &start, &info))
			{
				LPVOID lpLib;
				if (lpLib = VirtualAllocEx(info.hProcess, NULL, sizeof(pszPath), MEM_COMMIT, PAGE_READWRITE))
				{
					if (WriteProcessMemory(info.hProcess, lpLib, pszPath, sizeof(pszPath), NULL))
					{
						HANDLE loader;
						if (loader = CreateRemoteThread(info.hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)LoadLibraryW, lpLib, 0, NULL))
						{
							WaitForSingleObject(loader, INFINITE);
							CloseHandle(loader);
						}
					}
					VirtualFreeEx(info.hProcess, lpLib, 0, MEM_RELEASE);
				}
				CloseHandle(info.hProcess);
				ResumeThread(info.hThread);
				CloseHandle(info.hThread);
			}
			return FALSE;
		}
		else
#endif
		if ((it = (PBYTE)GetModuleHandleW(L"SpoilerAL.exe"))
			&& (nt = (PIMAGE_NT_HEADERS32)&it[((PIMAGE_DOS_HEADER)it)->e_lfanew],
				sec = IMAGE_FIRST_SECTION(nt),
				ib = (PBYTE)nt->OptionalHeader.ImageBase,
				ib == (PBYTE)hInstance)
			&& nt->FileHeader.NumberOfSections == _countof(lpflOldProtect) + 1
			&& nt->FileHeader.TimeDateStamp == 0x3EFF23AA
			&& 0x00201000 == nt->OptionalHeader.SizeOfCode
			&& 0x00050000 == nt->OptionalHeader.SizeOfInitializedData
			&& 0x000014A0 == nt->OptionalHeader.AddressOfEntryPoint
			&& 0x00202000 == nt->OptionalHeader.BaseOfData
			&& 0x00286000 == nt->OptionalHeader.SizeOfImage
			&& 0x00000600 == nt->OptionalHeader.SizeOfHeaders
			&& GetModuleFileNameW((HMODULE)hInstance, pszPath, _countof(pszPath))
			&& GetFileAttributesExW(pszPath, GetFileExInfoStandard, &attr)
			&& attr.nFileSizeHigh == 0
			&& CRC32FromFileW(pszPath, &attr.ftLastWriteTime.dwHighDateTime)
			&& PathRenameExtensionW(pszPath, L".ini")
			&& (GetPrivateProfileIntW(L"MainModule", L"CRC32", 0, pszPath) == attr.ftLastWriteTime.dwHighDateTime &&
				GetPrivateProfileIntW(L"MainModule", L"Size", -1, pszPath) == attr.nFileSizeLow ||
#ifdef _NO_CRT_STDIO_INLINE// NULL_SUPPORT
				_vsnwprintf(lpString, _countof(lpString), L"CRC32=0x%08X%lcSize=%u", (va_list)&attr.ftLastWriteTime.dwHighDateTime) &&
#else
				_vsnwprintf_l(//_CRT_INTERNAL_LOCAL_PRINTF_OPTIONS | _CRT_INTERNAL_PRINTF_LEGACY_VSPRINTF_NULL_TERMINATION,
					lpString, _countof(lpString), L"CRC32=0x%08X%lcSize=%u", NULL, (va_list)&attr.ftLastWriteTime.dwHighDateTime) &&
#endif
				WritePrivateProfileSectionW(L"MainModule", lpString, pszPath))
			&& VirtualProtect(ib, nt->OptionalHeader.SizeOfHeaders, PAGE_READWRITE, &flOldProtect)
			)
		{
			const IMAGE_OPTIONAL_HEADER opt = ((PIMAGE_NT_HEADERS)&ib[((PIMAGE_DOS_HEADER)ib)->e_lfanew])->OptionalHeader;
#ifdef NDEBUG
			const PIMAGE_TLS_DIRECTORY tls = ((PIMAGE_TLS_DIRECTORY)&it[nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].VirtualAddress]);
			const ULONG tlsIndex = *(PULONG)tls->AddressOfIndex;
			__movsb(ib, it, sec[_countof(lpflOldProtect)].VirtualAddress);
			tlsidx.$ = tlsIndex;// retrieve index
#else
			HMODULE hMod;
			CHAR szLib[MAX_PATH], szSys[MAX_PATH];

			__movsb(ib, it, nt->OptionalHeader.SizeOfHeaders);

			for (ptrdiff_t i = 0; i < _countof(lpflOldProtect); i++)
				if (VirtualProtect(&it[sec[i].VirtualAddress], sec[i].Misc.VirtualSize, PAGE_READONLY, &lpflOldProtect[i]))
					__movsb(&ib[sec[i].VirtualAddress], &it[sec[i].VirtualAddress], sec[i].Misc.VirtualSize);
				else __debugbreak();

			if (GetSystemDirectoryA(szSys, _countof(szSys)))
				for (PIMAGE_IMPORT_DESCRIPTOR imp = (PIMAGE_IMPORT_DESCRIPTOR)&ib[
					nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress
				]; imp->Characteristics; imp++)
					if (hMod = LoadLibraryExA(PathCombineA(szLib, szSys, (LPCSTR)&ib[imp->Name]), NULL, LOAD_WITH_ALTERED_SEARCH_PATH))
						for (PIMAGE_THUNK_DATA thk = (PIMAGE_THUNK_DATA)&ib[imp->FirstThunk];
							 thk->u1.AddressOfData;
							 thk++)
							thk->u1.Function = (DWORD_PTR)GetProcAddress(
								hMod, ((long)thk->u1.AddressOfData >= 0
									   ? ((PIMAGE_IMPORT_BY_NAME)&ib[thk->u1.AddressOfData])->Name
									   : MAKEINTRESOURCEA(thk->u1.Ordinal)));
					else __debugbreak();
			else __debugbreak();

			for (ptrdiff_t i = 0; i < _countof(lpflOldProtect); i++)
				if (!VirtualProtect(&ib[sec[i].VirtualAddress], sec[i].Misc.VirtualSize, lpflOldProtect[i], &flOldProtect))
					__debugbreak();

			tlsidx.$ = _tls_index;// provide own index
#endif
			((PIMAGE_NT_HEADERS)&ib[((PIMAGE_DOS_HEADER)ib)->e_lfanew])->OptionalHeader.SizeOfImage = opt.SizeOfImage;
			return VirtualProtect(ib, nt->OptionalHeader.SizeOfHeaders, PAGE_READONLY, &flOldProtect)
#ifdef NDEBUG
				&& _DllMainCRTStartup((HINSTANCE)hInstance, fdwReason, lpvReserved)
#else
				, !!(__xl[1] = (PIMAGE_TLS_CALLBACK)_DllMainCRTStartup)
#endif
				;
		}
		else
		{
#ifdef NDEBUG
			LPWSTR lpText = NULL;
			if (FormatMessageW(
				FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_ARGUMENT_ARRAY | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				GetLastError(),
				0,
				(LPWSTR)&lpText,
				sizeof(double),
				NULL))
				MessageBoxW(NULL, lpText, NULL, MB_ICONERROR | MB_SERVICE_NOTIFICATION);
			LocalFree(lpText);
#else
			__asm {
				call  GetLastError
				push  0
				mov   ecx, esp
				push  0
				push  8
				push  ecx
				push  0
				push  eax
				push  0
				push  FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_ARGUMENT_ARRAY | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS
				call  FormatMessageW
				test  eax, eax
				jz    FREE

				mov   ecx, [esp]
				push  MB_ICONERROR | MB_SERVICE_NOTIFICATION
				push  0
				push  ecx
				push  0
				call  MessageBoxW
			FREE:
				call  LocalFree
			}
#endif
		}
	}
	return FALSE;
}

#pragma code_seg(pop)
