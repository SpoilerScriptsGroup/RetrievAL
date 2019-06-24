#ifndef _PAGESIZE_H_
#define _PAGESIZE_H_

#if _MSC_VER > 1000
#pragma once
#endif

#if defined(_M_IX86) || defined(_M_X64)
#  define PAGE_SIZE 0x1000
#elif defined(_M_IA64)
#  define PAGE_SIZE 0x2000
#else
#  error "Unsupported platform. _M_IX86 or _M_X64 or _M_IA64 must be defined."
#endif

#endif	// _PAGESIZE_H_
