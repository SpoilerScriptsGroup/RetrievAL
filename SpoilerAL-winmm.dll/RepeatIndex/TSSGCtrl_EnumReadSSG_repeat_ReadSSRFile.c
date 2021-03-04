#include <windows.h>
#include <assert.h>
#define USING_NAMESPACE_BCB6_STD
#include "TSSGCtrl.h"
#include "SSGSubjectProperty.h"

extern DWORD RepeatDepth;
extern DWORD IndexRoot, IndexTemp;
extern TSSGSubject dummySSGS;

void __stdcall repeat_ReadSSRFile(
	TSSGCtrl     *this,
	stack        *ParentStack,
	LPVOID       ADJElem,
	const string *LineS,
	DWORD        RepeatIndex,
	DWORD        OuterRepeat,
	TSSGSubject  *SSGS)
{
	vector_string       tmpV;
	vector_dword        indices;
	TSSGSubjectProperty *prop = NULL;

	vector_ctor(&indices);
	if (SSGS == &dummySSGS)
	{
		if (prop = IndexTemp == MAXDWORD ? NULL : SubjectProperty + IndexTemp)
		{
			prop->OuterRepeat = OuterRepeat;
			prop->RepeatDepth = RepeatDepth;
			prop->RepeatIndex = RepeatIndex;
#if EMBED_BREADTH
			prop->ParentEntry = *stack_top(ParentStack, TSSDir *);
#endif
		}
		dummySSGS.propertyIndex = IndexTemp;
	}
	else
		dummySSGS.propertyIndex = SSGS->propertyIndex;
	TSSGCtrl_ReadSSRFile(&tmpV, this, LineS, &indices, SSGS);
	dummySSGS.propertyIndex = IndexRoot;
	if (prop && map_end(prop)) TSSGSubjectProperty_dtor(prop, TRUE);
	if (!vector_empty(&tmpV))
	{
		if (vector_size(&indices) > 1)
		{
			DWORD   outer;
			string  *it;
			LPDWORD repeat;
			size_t  elementSize;

			if (!RepeatDepth)
				outer = MAXDWORD;
			else if (SSGS->type && SSGS->propertyIndex != MAXDWORD)
				outer = SSGS->propertyIndex;
			else if (prop = GrowSubjectProperty(&outer))
			{
				prop->OuterRepeat  = OuterRepeat;
				prop->RepeatDepth  = RepeatDepth;
				prop->RepeatIndex  = RepeatIndex;
			}
			RepeatDepth++;
			it = vector_begin(&tmpV);
			repeat = vector_begin(&indices);
			elementSize = *(repeat++);
			assert(vector_size(&tmpV) == (vector_size(&indices) - 1) * elementSize);
			do
			{
				vector_string constElem = { ._M_start = it, ._M_end_of_storage = it };

				constElem._M_finish = it += elementSize;
				TSSGCtrl_EnumReadSSG(this, &constElem, ParentStack, ADJElem, *(repeat++), outer);
			} while (it < vector_end(&tmpV));
			RepeatDepth--;
		}
		else
		{
			TSSGCtrl_EnumReadSSG(this, &tmpV, ParentStack, ADJElem, RepeatIndex, OuterRepeat);
		}
	}
	vector_dtor(&indices);
	vector_string_dtor(&tmpV);
}

#if 0
__declspec(naked) void __cdecl TSSGCtrl_EnumReadSSG_repeat_ReadSSRFile()
{
	static TSSGSubject SSGS = { TSSGSubject_VTable, 0 };
	__asm
	{
		#define ReturnAddress   004EB508H
		#define this            (ebp +   8H)
		#define ParentStack     (ebp +  10H)
		#define ADJElem         (ebp +  14H)
		#define RepeatIndex     (ebp +  18H)
		#define OuterRepeat     (ebp +  1CH)
		#define LineS           (ebp -  38H)

		mov     edx, dword ptr [OuterRepeat]
		mov     ecx, dword ptr [RepeatIndex]
		push    offset SSGS
		lea     eax, [LineS]
		push    edx
		push    ecx
		push    eax
		mov     edx, dword ptr [ADJElem]
		mov     ecx, dword ptr [ParentStack]
		mov     eax, dword ptr [this]
		push    edx
		push    ecx
		push    eax
		push    ReturnAddress
		jmp     repeat_ReadSSRFile

		#undef ReturnAddress
		#undef this
		#undef ParentStack
		#undef ADJElem
		#undef RepeatIndex
		#undef OuterRepeat
		#undef LineS
	}
}
#endif
