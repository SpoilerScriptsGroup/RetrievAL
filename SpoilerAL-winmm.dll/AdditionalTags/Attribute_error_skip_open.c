#include <windows.h>
#include "bcb6_std_string.h"
#include "TSSGCtrl.h"
#include "TSSGSubject.h"
#include "TSSGAttributeElement.h"

#define AT_ERRORSKIP 0x2000

void __stdcall Attribute_error_skip_open(TSSGCtrl *SSGCtrl, TSSGSubject *parent, bcb6_std_string *prefix, bcb6_std_string *code)
{
	TSSGAttributeElement *lpNewErrorSkip;

	lpNewErrorSkip = new_TSSGAttributeElement();
	if (lpNewErrorSkip == NULL)
		return;
	lpNewErrorSkip->type = AT_ERRORSKIP;
	TSSGAttributeSelector_PushElement(&SSGCtrl->attributeSelector, lpNewErrorSkip);
}
