#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"
#include "TSSGCtrl.h"
#include "TSSGSubject.h"
#include "TSSGAttributeElement.h"

void __stdcall Attribute_error_skip_open(TSSGCtrl *this)
{
	TSSGAttributeElement *lpNewErrorSkip;

	lpNewErrorSkip = new_TSSGAttributeElement();
	if (lpNewErrorSkip == NULL)
		return;
	lpNewErrorSkip->type = atERRORSKIP;
	TSSGAttributeSelector_PushElement(&this->attributeSelector, lpNewErrorSkip);
}

void __stdcall Attribute_error_skip_close(TSSGCtrl *this)
{
	TSSGAttributeSelector_PopElementByType(&this->attributeSelector, atERRORSKIP);
}
