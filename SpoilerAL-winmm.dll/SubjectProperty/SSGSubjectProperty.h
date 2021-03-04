#ifndef _SSGSUBJECTPROPERTY_H_
#define _SSGSUBJECTPROPERTY_H_

#include <windows.h>
#include "bcb6_std_map.h"
#include "TSSDir.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef bcb6_std_map TSSGSubjectProperty;

extern TSSGSubjectProperty *SubjectProperty;
extern DWORD               SubjectPropertyCount;
extern DWORD               TitleWidth;

EXTERN_C void __fastcall TSSGSubjectProperty_dtor(TSSGSubjectProperty *prop, BOOL dealloc);
EXTERN_C void __cdecl ClearSubjectProperty();
EXTERN_C TSSGSubjectProperty * __fastcall GrowSubjectProperty(DWORD *lpdwIndex);
#define AppendSubjectProperty(SSGS) \
	GrowSubjectProperty((LPDWORD)&(SSGS)->propertyIndex)
#define GetSubjectProperty(SSGS) \
	((SSGS) && (SSGS)->propertyIndex != MAXDWORD ? SubjectProperty + (SSGS)->propertyIndex : NULL)
#define GetOuterRepeat(Property) \
	(((Property)->OuterRepeat) != MAXDWORD ? SubjectProperty + (Property)->OuterRepeat : NULL)

#ifdef __cplusplus
}
#endif

#endif	// _SSGSUBJECTPROPERTY_H_
