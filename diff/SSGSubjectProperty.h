#ifndef _SSGSUBJECTPROPERTY_H_
#define _SSGSUBJECTPROPERTY_H_

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _TSSGSubjectProperty {
	DWORD Width;
	DWORD RepeatDepth;
	DWORD RepeatIndex;
	DWORD ParentRepeat;
} TSSGSubjectProperty;

extern TSSGSubjectProperty *SubjectProperty;
extern DWORD               SubjectPropertyCount;
extern DWORD               TitleWidth;

void __cdecl ClearSubjectProperty();
TSSGSubjectProperty * __fastcall GrowSubjectProperty(DWORD *lpdwIndex);
#define AppendSubjectProperty(SSGS) \
	GrowSubjectProperty((LPDWORD)(SSGS) + 4)
#define GetSubjectProperty(SSGS) \
	((SSGS) && *((LPDWORD)(SSGS) + 4) != MAXDWORD ? SubjectProperty + *((LPDWORD)(SSGS) + 4) : NULL)
#define GetParentRepeat(Property) \
	(((Property)->ParentRepeat) != MAXDWORD ? SubjectProperty + (Property)->ParentRepeat : NULL)

#ifdef __cplusplus
}
#endif

#endif	// _SSGSUBJECTPROPERTY_H_
