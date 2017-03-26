#include <windows.h>
#include "bcb6_std_string.h"
#include "TSSGCtrl.h"
#include "TSSGSubject.h"

#define AT_ERRORSKIP 0x2000

void __stdcall Attribute_error_skip_close(TSSGCtrl *SSGCtrl, TSSGSubject *parent, bcb6_std_string *prefix, bcb6_std_string *code)
{
	TSSGAttributeSelector_PopElementByType(&SSGCtrl->attributeSelector, AT_ERRORSKIP);
}
