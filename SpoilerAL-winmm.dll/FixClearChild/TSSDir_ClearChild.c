#define USING_NAMESPACE_BCB6_STD
#include "TSSDir.h"
#include "TMainForm.h"

void __cdecl TSSDir_ClearChild(TSSDir *this)
{
	for (TSSGSubject **it = (TSSGSubject **)vector_begin(&this->childVec); it != vector_end(&this->childVec); it++)
	{
		if ((*it)->status & ssLOCK)
			TSSGCtrl_SetLock(&MainForm->ssgCtrl, FALSE, *it, NULL);
		if ((*it)->type == stDIR)
			TSSDir_ClearChild((TSSDir *)*it);
		delete_TSSGSubject(*it);
	}
	vector_dtor(&this->childVec);
	vector_ctor(&this->childVec);
}
