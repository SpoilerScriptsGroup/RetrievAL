#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include "TMainForm.h"
#undef MainForm
#include "TMenuItem.h"
#include "BuildDate.h"
#include "version.h"

void __stdcall ModifyVersionString(TMainForm *MainForm)
{
	const char lpVersion[] = {
		'v', 'e', 'r', 's', 'i', 'o', 'n', ' ', ':', ' ',
		VERSION_MAJOR + '0', '.', VERSION_MINOR + '0',
		' ', '-', ' ', BUILD_DATE_ARRAY('.', '.'), ' ',
		'(', '&', 'R', ')',
		'\0'
	};

	TMenuItem_SetCaption(MainForm->M_Version, lpVersion);
}
