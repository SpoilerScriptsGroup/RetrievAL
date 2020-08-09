#ifndef _SSGSUBJECTPROPERTY_H_
#define _SSGSUBJECTPROPERTY_H_

#include <windows.h>
#define EMBED_BREADTH 1
#include "TSSDir.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _TSSGSubjectProperty {
	INT     OuterRepeat;
	INT32   RepeatDepth;
	DWORD   RepeatIndex;
#if	EMBED_BREADTH
	TSSDir *ParentEntry;
#else
	LONG    Breadth;
#endif
} TSSGSubjectProperty;

extern TSSGSubjectProperty *SubjectProperty;
extern DWORD               SubjectPropertyCount;
extern DWORD               TitleWidth;

void __cdecl ClearSubjectProperty();
TSSGSubjectProperty * __fastcall GrowSubjectProperty(DWORD *lpdwIndex);
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
