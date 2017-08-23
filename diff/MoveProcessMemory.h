#ifndef _MOVEPROCESSMEMORY_H_
#define _MOVEPROCESSMEMORY_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include <windows.h>
#ifdef _MSC_VER
#include <winternl.h>
#else
typedef LONG NTSTATUS;
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef STATUS_SEVERITY_SUCCESS
#define STATUS_SEVERITY_SUCCESS 0x0
#endif
#ifndef STATUS_SEVERITY_ERROR
#define STATUS_SEVERITY_ERROR   0x3
#endif
#ifndef FACILITY_NULL
#define FACILITY_NULL           0x000
#endif
#ifndef FACILITY_USER_DEFINE
#define FACILITY_USER_DEFINE    0xFFF
#endif

#ifndef MAKE_NTSTATUS
#define MAKE_NTSTATUS(Severity, Facility, Code) \
    (NTSTATUS)(                                 \
        ( (DWORD)(Severity)           << 30) |  \
        (((DWORD)(Facility) & 0x0FFF) << 16) |  \
        ( (DWORD)(Code    ) & 0xFFFF       ))
#endif

#ifndef STATUS_SUCCESS
#define STATUS_SUCCESS               MAKE_NTSTATUS(STATUS_SEVERITY_SUCCESS, FACILITY_NULL, 0)
#endif
#ifndef STATUS_PROCESS_ACCESS_DENIED
#define STATUS_PROCESS_ACCESS_DENIED MAKE_NTSTATUS(STATUS_SEVERITY_ERROR, FACILITY_USER_DEFINE, 0)
#endif
#ifndef STATUS_MEMORY_READ_FAILED
#define STATUS_MEMORY_READ_FAILED    MAKE_NTSTATUS(STATUS_SEVERITY_ERROR, FACILITY_USER_DEFINE, 1)
#endif
#ifndef STATUS_MEMORY_WRITE_FAILED
#define STATUS_MEMORY_WRITE_FAILED   MAKE_NTSTATUS(STATUS_SEVERITY_ERROR, FACILITY_USER_DEFINE, 2)
#endif

#ifndef NT_SUCCESS
#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)
#endif

NTSTATUS __stdcall MoveProcessMemory(
	IN OPTIONAL HANDLE  hDestProcess,
	IN          LPVOID  lpDest,
	IN OPTIONAL HANDLE  hSrcProcess,
	IN          LPCVOID lpSrc,
	IN          size_t  nSize);

#ifdef __cplusplus
}
#endif

#endif	// _MOVEPROCESSMEMORY_H_
