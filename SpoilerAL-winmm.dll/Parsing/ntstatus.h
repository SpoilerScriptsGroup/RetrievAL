#ifndef _NTSTATUS_H_
#define _NTSTATUS_H_

#if _MSC_VER > 1000
#pragma once
#endif

#ifdef _MSC_VER
#include <winternl.h>
#else
typedef long NTSTATUS;
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
#define MAKE_NTSTATUS(Severity, Facility, Code)         \
    (NTSTATUS)(                                         \
        ( (unsigned long)(Severity)           << 30) |  \
        (((unsigned long)(Facility) & 0x0FFF) << 16) |  \
        ( (unsigned long)(Code    ) & 0xFFFF       ))
#endif

#ifndef STATUS_SUCCESS
#define STATUS_SUCCESS               MAKE_NTSTATUS(STATUS_SEVERITY_SUCCESS, FACILITY_NULL, 0)
#endif
#ifndef STATUS_MEMORY_READ_FAILED
#define STATUS_MEMORY_READ_FAILED    MAKE_NTSTATUS(STATUS_SEVERITY_ERROR, FACILITY_USER_DEFINE, 0)
#endif
#ifndef STATUS_MEMORY_WRITE_FAILED
#define STATUS_MEMORY_WRITE_FAILED   MAKE_NTSTATUS(STATUS_SEVERITY_ERROR, FACILITY_USER_DEFINE, 1)
#endif
#ifndef STATUS_MEMORY_READ1_FAILED
#define STATUS_MEMORY_READ1_FAILED   MAKE_NTSTATUS(STATUS_SEVERITY_ERROR, FACILITY_USER_DEFINE, 2)
#endif
#ifndef STATUS_MEMORY_READ2_FAILED
#define STATUS_MEMORY_READ2_FAILED   MAKE_NTSTATUS(STATUS_SEVERITY_ERROR, FACILITY_USER_DEFINE, 3)
#endif

#ifndef NT_SUCCESS
#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)
#endif

#endif	// _NTSTATUS_H_
