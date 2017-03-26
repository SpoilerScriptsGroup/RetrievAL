#pragma once

#define TProcessAccessElementBase_GetType(AElem) \
	*((unsigned long *)(AElem) + 1)

