#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"
#include "TSSGCtrl.h"
#include "TSSGSubject.h"

#define AT_VARIABLE 0x0800

void __stdcall Attribute_variable_close(TSSGCtrl *SSGCtrl, TSSGSubject *parent, string *prefix, string *code)
{
	TSSGAttributeSelector_PopElementByType(&SSGCtrl->attributeSelector, AT_VARIABLE);
}
