#include <windows.h>
#include "bcb6_std_string.h"
#include "TSSGCtrl.h"
#include "TSSGSubject.h"

#define AT_VARIABLE 0x0800

void __stdcall Attribute_variable_close(TSSGCtrl *SSGCtrl, TSSGSubject *parent, bcb6_std_string *prefix, bcb6_std_string *code)
{
	TSSGAttributeSelector_PopElementByType(&SSGCtrl->attributeSelector, AT_VARIABLE);
}
