#include <windows.h>
#include "TSSGSubject.h"
#include "SubjectProperty\SSGSubjectProperty.h"

EXTERN_C DWORD RepeatDepth;

void __fastcall TSSGCtrl_EnumReadSSG_SetSubjectProperty(TSSGSubject *SSGS, DWORD RepeatIndex, DWORD ParentRepeat)
{
	TSSGSubjectProperty *prop = GetSubjectProperty(SSGS);
	if (prop)
	{
		prop->RepeatDepth  = RepeatDepth;
		prop->RepeatIndex  = RepeatIndex;
		prop->ParentRepeat = ParentRepeat;
	}
}
