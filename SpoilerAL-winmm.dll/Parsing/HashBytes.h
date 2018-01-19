#ifndef _HASHBYTES_H_
#define _HASHBYTES_H_

#if 1
#include "fnv1a.h"
#define HashBytes fnv1a32
#else
EXTERN_C unsigned long int CRC32Combine(const void *lpBuffer, unsigned long int nSize, unsigned long int crc);
#define CRC32(lpBuffer, nSize) CRC32Combine(lpBuffer, nSize, 0)
#define HashBytes CRC32
#endif

#endif	// _HASHBYTES_H_
