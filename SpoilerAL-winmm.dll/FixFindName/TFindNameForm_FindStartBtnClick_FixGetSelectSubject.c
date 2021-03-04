#include "TMainForm.h"
#undef MainForm

TSSGSubject * __fastcall TFindNameForm_FindStartBtnClick_FixGetSelectSubject(const TMainForm *const MainForm)
{
	return MainForm->selectSubject ? MainForm->selectSubject : &MainForm->ssgCtrl.rootSubject->super;
}
