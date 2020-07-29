#ifndef _SSGSUBJECTPROPERTY_H_
#define _SSGSUBJECTPROPERTY_H_

#include <windows.h>
#ifndef __BORLANDC__
#include "TSSGSubject.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _TSSGSubjectProperty {
	LONG  Breadth;
	DWORD RepeatDepth;
	DWORD RepeatIndex;
	INT   OuterRepeat;
} TSSGSubjectProperty;

extern TSSGSubjectProperty *SubjectProperty;
extern DWORD               SubjectPropertyCount;
extern DWORD               TitleWidth;

void __cdecl ClearSubjectProperty();
TSSGSubjectProperty * __fastcall GrowSubjectProperty(DWORD *lpdwIndex);
#define AppendSubjectProperty(SSGS) \
	GrowSubjectProperty(&(SSGS)->propertyIndex)
#define GetSubjectProperty(SSGS) \
	((SSGS) && (SSGS)->propertyIndex != MAXDWORD ? SubjectProperty + (SSGS)->propertyIndex : NULL)
#define GetOuterRepeat(Property) \
	(((Property)->OuterRepeat) != MAXDWORD ? SubjectProperty + (Property)->OuterRepeat : NULL)

#define EMBED_BREADTH 1

#ifdef __cplusplus
}
#endif

#endif	// _SSGSUBJECTPROPERTY_H_
