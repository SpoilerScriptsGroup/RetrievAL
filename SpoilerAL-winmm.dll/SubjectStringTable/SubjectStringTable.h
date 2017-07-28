#ifndef _SUBJECTSTRINGTABLE_H_
#define _SUBJECTSTRINGTABLE_H_

#include <windows.h>
#include "bcb6_std_string.h"
#include "bcb6_std_vector.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	char *begin;
	char *end;
} STRING;

extern size_t SubjectStringTable_count;
extern size_t *SubjectStringTable_indices;
extern STRING *SubjectStringTable_array;

void __cdecl SubjectStringTable_dtor();
void __cdecl SubjectStringTable_clear();
size_t __fastcall SubjectStringTable_insert(bcb6_std_string *s);

#ifdef __cplusplus
}
#endif

#endif	// _SUBJECTSTRINGTABLE_H_

