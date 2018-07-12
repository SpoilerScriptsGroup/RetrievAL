#define USING_NAMESPACE_BCB6_STD
#include "TSSDir.h"
#include "TMainForm.h"

#define ST_DIR 1

static void(__cdecl * const TSSGCtrl_SetLock)(TSSGCtrl* this, BOOLEAN IsLock, TSSGSubject* Target, void* Value) = (void*)0x004FE200;

void __cdecl TSSDir_ClearChild(TSSDir *this)
{
	for (TSSGSubject **it = (TSSGSubject **)vector_begin(&this->childVec); it != vector_end(&this->childVec); it++)
	{
		if ((*it)->type == ST_DIR)
			TSSDir_ClearChild((TSSDir *)*it);
		if ((*it)->status & 2)
			TSSGCtrl_SetLock(&MainForm->ssgCtrl, FALSE, *it, NULL);
		delete_TSSGSubject(*it);
	}
	vector_dtor(&this->childVec);
	vector_ctor(&this->childVec);
}
