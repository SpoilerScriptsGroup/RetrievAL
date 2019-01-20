#ifndef _COMPAREPROCESSMEMORY_H_
#define _COMPAREPROCESSMEMORY_H_

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
#ifndef STATUS_MEMORY_READ1_FAILED
#define STATUS_MEMORY_READ1_FAILED    MAKE_NTSTATUS(STATUS_SEVERITY_ERROR, FACILITY_USER_DEFINE, 3)
#endif
#ifndef STATUS_MEMORY_READ2_FAILED
#define STATUS_MEMORY_READ2_FAILED    MAKE_NTSTATUS(STATUS_SEVERITY_ERROR, FACILITY_USER_DEFINE, 4)
#endif

#ifndef NT_SUCCESS
#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)
#endif

NTSTATUS __stdcall CompareProcessMemory(
	OUT         int     *lpiResult,
	IN OPTIONAL HANDLE  hProcess1,
	IN          LPCVOID lpAddress1,
	IN OPTIONAL HANDLE  hProcess2,
	IN          LPCVOID lpAddress2,
	IN          size_t  nSize);

#ifdef __cplusplus
}
#endif

#endif	// _COMPAREPROCESSMEMORY_H_
