#include "TMainForm.h"
#undef MainForm

TSSGSubject * __fastcall TFindNameForm_FindStartBtnClick_FixGetSelectSubject(TMainForm *MainForm)
{
	return MainForm->selectSubject ? MainForm->selectSubject : &MainForm->ssgCtrl.rootSubject->super;
}
