#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"
#include "TSSGCtrl.h"
#include "TSSGSubject.h"
#include "TSSGAttributeElement.h"

#define AT_ERRORSKIP 0x2000

void __stdcall Attribute_error_skip_open(TSSGCtrl *SSGCtrl, TSSGSubject *parent, string *prefix, string *code)
{
	TSSGAttributeElement *lpNewErrorSkip;

	lpNewErrorSkip = new_TSSGAttributeElement();
	if (lpNewErrorSkip == NULL)
		return;
	lpNewErrorSkip->type = AT_ERRORSKIP;
	TSSGAttributeSelector_PushElement(&SSGCtrl->attributeSelector, lpNewErrorSkip);
}
