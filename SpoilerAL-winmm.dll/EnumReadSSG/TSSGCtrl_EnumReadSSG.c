#include <stdint.h>
#include <windows.h>
#define USING_NAMESPACE_BCB6
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_operator.h"
#include "bcb6_std.h"
#include "bcb6_std_string.h"
#include "bcb6_std_stack.h"
#include "TStringDivision.h"
#include "TSSGCtrl.h"
#include "TSSDir.h"
#include "TSSCalc.h"
#include "TSSToggle.h"
#include "TSSString.h"
#include "SSGSubjectProperty.h"
#include "Attribute_variable.h"
#include "Attribute_expr.h"
#include "Attribute_define.h"
#include "Attribute_undef.h"
#include "Attribute_allcate.h"
#include "Attribute_error_skip.h"
#include "Attribute_scope.h"
#include "Attribute_offset.h"

extern void __stdcall ReplaceDefine(TSSGAttributeSelector *attributeSelector, string *line);
extern void __stdcall repeat_ReadSSRFile(TSSGCtrl *this, LPVOID ParentStack, LPVOID ADJElem, const string *LineS, DWORD RepeatIndex, DWORD ParentRepeat, TSSGSubject *SSGS);

extern DWORD  RepeatDepth;
extern string ProcessAttachCode;
extern string ProcessDetachCode;

TSSGSubject dummySSGS = { TSSGSubject_VTable, 0 };

void __cdecl TSSGCtrl_EnumReadSSG(TSSGCtrl *this, vector_string *SSGFile, LPVOID ParentStack, TDialogAdjustmentAttribute *ADJElem, DWORD RepeatIndex/* = 0*/, DWORD ParentRepeat/* = MAXDWORD*/)
{
	extern unsigned long __cdecl Parsing(IN TSSGCtrl *this, IN TSSGSubject *SSGS, IN const string *Src, ...);

	BOOL cond, invalid = FALSE;

	#define stack_PTSSDir_size(Stack)        stack_dword_size((stack_dword *)(Stack))
	#define stack_PTSSDir_top(Stack)         ((TSSDir *)stack_dword_top((stack_dword *)(Stack)))
	#define stack_PTSSDir_push(Stack, Value) stack_dword_push((stack_dword *)(Stack), (DWORD)(TSSDir *)(Value))
	#define stack_PTSSDir_pop(Stack)         stack_dword_pop((stack_dword *)(Stack))

	for (string *it = vector_begin(SSGFile); it != vector_end(SSGFile); ++it)
	{
		typedef enum {
			OPEN  = 0,
			CLOSE = 1,
		} TAG_TYPE;

		typedef enum {
			IF,
			ELIF,
			ELSE,
			ENDIF,
			SUBJECT,
			INPUT,
			BACK,
			ROOT,
			REPLACE,
			SIZE_OPEN,
			SIZE_CLOSE,
			ADJUSTMENT_OPEN,
			ADJUSTMENT_CLOSE,
			ADJUST_CHECK_OPEN,
			ADJUST_CHECK_CLOSE,
			FUNNEL_OPEN,
			FUNNEL_CLOSE,
			REPEAT,
			IO_FEP_OPEN,
			IO_FEP_CLOSE,
			E_WITH_OPEN,
			E_WITH_CLOSE,
			ENABLED_OPEN,
			ENABLED_CLOSE,
			CHILD_RW_OPEN,
			CHILD_RW_CLOSE,
			CAUTION_OPEN,
			CAUTION_CLOSE,
			INVOLVE,
			NOTE,
			PROCESS,
			TITLE,
			MAKER,
			CREATOR,
			DISTINCTION,
			CALC,
			TOGGLE,
			STRING,
			VAL,
			SCRIPT_CLOSE,
			ATTACH,
			DETACH,
			VARIABLE_OPEN,
			VARIABLE_CLOSE,
			EXPR,
			DEFINE,
			UNDEF,
			ALLOCATE,
			ERROR_SKIP_OPEN,
			ERROR_SKIP_CLOSE,
			SCOPE_OPEN,
			SCOPE_CLOSE,
			OFFSET_OPEN,
			OFFSET_CLOSE,
			FORMAT_OPEN,
			FORMAT_CLOSE,
		} TAG;

		char     *p, c;
		size_t   length;
		DWORD    dw;
		TAG_TYPE close;
		TAG      tag;

		p = string_begin(it);
		while ((c = *(p++)) == ' ' || c == '\t');
		if (c != '[')
			continue;
		p += close = (*p == '/');
		if ((length = (string_end(it) - p)) < 3)
			continue;

		#pragma region switch (*(LPDWORD)p)
		switch ((dw = *(LPDWORD)p) & 0xFF)	// jump by table
		{
		case 'a':
			switch (dw)
			{
			case BSWAP32('adju'): goto CASE_ADJU;
			case BSWAP32('allo'): goto CASE_ALLO;
			case BSWAP32('atta'): goto CASE_ATTA;
			default: continue;
			}
		case 'b':
			if (dw == BSWAP32('back')) goto CASE_BACK;
			continue;
		case 'c':
			switch (dw)
			{
			case BSWAP32('calc'): goto CASE_CALC;
			case BSWAP32('caut'): goto CASE_CAUT;
			case BSWAP32('chil'): goto CASE_CHIL;
			case BSWAP32('crea'): goto CASE_CREA;
			default: continue;
			}
		case 'd':
			switch (dw)
			{
			case BSWAP32('defi'): goto CASE_DEFI;
			case BSWAP32('deta'): goto CASE_DETA;
			case BSWAP32('dist'): goto CASE_DIST;
			default: continue;
			}
		case 'e':
			switch (dw)
			{
			case BSWAP32('e_wi'): goto CASE_E_WI;
			case BSWAP32('elif'): goto CASE_ELIF;
			case BSWAP32('else'): goto CASE_ELSE;
			case BSWAP32('enab'): goto CASE_ENAB;
			case BSWAP32('endi'): goto CASE_ENDI;
			case BSWAP32('erro'): goto CASE_ERRO;
			case BSWAP32('expr'): goto CASE_EXPR;
			default: continue;
			}
		case 'f':
			switch (dw)
			{
			case BSWAP32('form'): goto CASE_FORM;
			case BSWAP32('funn'): goto CASE_FUNN;
			default: continue;
			}
		case 'i':
			switch (dw)
			{
			case BSWAP32('inpu'): goto CASE_INPU;
			case BSWAP32('invo'): goto CASE_INVO;
			case BSWAP32('io_f'): goto CASE_IO_F;
			default:
				if (!close && (dw & USHRT_MAX) == BSWAP16('if'))
				{
					// [if]
					p += 2;
					tag = IF;
					goto SWITCH_BREAK;
				}
				continue;
			}
		case 'm':
			if (dw == BSWAP32('make')) goto CASE_MAKE;
			continue;
		case 'n':
			if (dw == BSWAP32('note')) goto CASE_NOTE;
			continue;
		case 'o':
			if (dw == BSWAP32('offs')) goto CASE_OFFS;
			continue;
		case 'p':
			if (dw == BSWAP32('proc')) goto CASE_PROC;
			continue;
		case 'r':
			switch (dw)
			{
			case BSWAP32('repe'): goto CASE_REPE;
			case BSWAP32('repl'): goto CASE_REPL;
			case BSWAP32('root'): goto CASE_ROOT;
			default: continue;
			}
		case 's':
			switch (dw)
			{
			case BSWAP32('scop'): goto CASE_SCOP;
			case BSWAP32('scri'): goto CASE_SCRI;
			case BSWAP32('size'): goto CASE_SIZE;
			case BSWAP32('stri'): goto CASE_STRI;
			case BSWAP32('subj'): goto CASE_SUBJ;
			default: continue;
			}
		case 't':
			switch (dw)
			{
			case BSWAP32('titl'): goto CASE_TITL;
			case BSWAP32('togg'): goto CASE_TOGG;
			default: continue;
			}
		case 'u':
			if (dw == BSWAP32('unde')) goto CASE_UNDE;
			continue;
		case 'v':
			switch (dw)
			{
			case BSWAP32('val]'): goto CASE_VAL1;
			case BSWAP32('val '):
			case BSWAP32('val\t'): goto CASE_VAL2;
			case BSWAP32('vari'): goto CASE_VARI;
			default: continue;
			}
		default:
			continue;
		}
		// [elif]
		CASE_ELIF:
			if (close || length < 5)
				continue;
			p += 4;
			tag = ELIF;
			goto SWITCH_BREAK;
		// [else]
		CASE_ELSE:
			if (close || length < 5)
				continue;
			p += 4;
			tag = ELSE;
			goto SWITCH_BREAK;
		// [endif]
		CASE_ENDI:
			if (close || length < 6 || p[4] != 'f')
				continue;
			p += 5;
			tag = ENDIF;
			goto SWITCH_BREAK;
		// [subject]
		CASE_SUBJ:
			if (close || length < 8 || *(LPDWORD)(p + 3) != BSWAP32('ject'))
				continue;
			p += 7;
			tag = SUBJECT;
			goto SWITCH_BREAK;
		// [input]
		CASE_INPU:
			if (close || length < 6 || p[4] != 't')
				continue;
			p += 5;
			tag = INPUT;
			goto SWITCH_BREAK;
		// [back]
		CASE_BACK:
			if (close || length < 5)
				continue;
			p += 4;
			tag = BACK;
			goto SWITCH_BREAK;
		// [root]
		CASE_ROOT:
			if (close || length < 5)
				continue;
			p += 4;
			tag = ROOT;
			goto SWITCH_BREAK;
		// [replace]
		CASE_REPL:
			if (close || length < 8 || *(LPDWORD)(p + 3) != BSWAP32('lace'))
				continue;
			p += 7;
			tag = REPLACE;
			goto SWITCH_BREAK;
		// [size], [/size]
		CASE_SIZE:
			if (length < 5)
				continue;
			p += 4;
			tag = SIZE_OPEN + close;
			goto SWITCH_BREAK;
		// [adjustment], [/adjustment], [adjust_check], [/adjust_check]
		CASE_ADJU:
			if (length < 11)
				continue;
			switch (*(LPDWORD)(p + 4))
			{
			// [adjustment], [/adjustment]
			case BSWAP32('stme'):
				if (*(LPWORD)(p + 8) != BSWAP16('nt'))
					continue;
				p += 10;
				tag = ADJUSTMENT_OPEN + close;
				goto SWITCH_BREAK;
			// [adjust_check], [/adjust_check]
			case BSWAP32('st_c'):
				if (length < 13 || *(LPDWORD)(p + 8) != BSWAP32('heck'))
					continue;
				p += 12;
				tag = ADJUST_CHECK_OPEN + close;
				goto SWITCH_BREAK;
			default:
				continue;
			}
			goto SWITCH_BREAK;
		// [funnel], [/funnel]
		CASE_FUNN:
			if (length < 7 || *(LPWORD)(p + 4) != BSWAP16('el'))
				continue;
			p += 6;
			tag = FUNNEL_OPEN + close;
			goto SWITCH_BREAK;
		// [format], [/format]
		CASE_FORM:
			if (length < 7 || *(LPWORD)(p + 4) != BSWAP16('at'))
				continue;
			p += 6;
			tag = FORMAT_OPEN + close;
			goto SWITCH_BREAK;
		// [repeat]
		CASE_REPE:
			if (close || length < 7 || *(LPWORD)(p + 4) != BSWAP16('at'))
				continue;
			p += 6;
			tag = REPEAT;
			goto SWITCH_BREAK;
		// [io_fep], [/io_fep]
		CASE_IO_F:
			if (length < 7 || *(LPWORD)(p + 4) != BSWAP16('ep'))
				continue;
			p += 6;
			tag = IO_FEP_OPEN + close;
			goto SWITCH_BREAK;
		// [e_with], [/e_with]
		CASE_E_WI:
			if (length < 7 || *(LPWORD)(p + 4) != BSWAP16('th'))
				continue;
			p += 6;
			tag = E_WITH_OPEN + close;
			goto SWITCH_BREAK;
		// [enabled], [/enabled]
		CASE_ENAB:
			if (length < 8 || *(LPDWORD)(p + 3) != BSWAP32('bled'))
				continue;
			p += 7;
			tag = ENABLED_OPEN + close;
			goto SWITCH_BREAK;
		// [child_rw], [/child_rw]
		CASE_CHIL:
			if (length < 9 || *(LPDWORD)(p + 4) != BSWAP32('d_rw'))
				continue;
			p += 8;
			tag = CHILD_RW_OPEN + close;
			goto SWITCH_BREAK;
		// [caution], [/caution]
		CASE_CAUT:
			if (length < 8 || *(LPDWORD)(p + 3) != BSWAP32('tion'))
				continue;
			p += 7;
			tag = CAUTION_OPEN + close;
			goto SWITCH_BREAK;
		// [involve]
		CASE_INVO:
			if (close || length < 8 || *(LPDWORD)(p + 3) != BSWAP32('olve'))
				continue;
			p += 7;
			tag = INVOLVE;
			goto SWITCH_BREAK;
		// [note]
		CASE_NOTE:
			if (close || length < 5)
				continue;
			p += 4;
			tag = NOTE;
			goto SWITCH_BREAK;
		// [process]
		CASE_PROC:
			if (close || length < 8 || *(LPDWORD)(p + 3) != BSWAP32('cess'))
				continue;
			p += 7;
			tag = PROCESS;
			goto SWITCH_BREAK;
		// [title]
		CASE_TITL:
			if (close || length < 6 || p[4] != 'e')
				continue;
			p += 5;
			tag = TITLE;
			goto SWITCH_BREAK;
		// [maker]
		CASE_MAKE:
			if (close || length < 6 || p[4] != 'r')
				continue;
			p += 5;
			tag = MAKER;
			goto SWITCH_BREAK;
		// [creator]
		CASE_CREA:
			if (close || length < 8 || *(LPDWORD)(p + 3) != BSWAP32('ator'))
				continue;
			p += 7;
			tag = CREATOR;
			goto SWITCH_BREAK;
		// [distinction]
		CASE_DIST:
			if (close || length < 12 || *(LPDWORD)(p + 4) != BSWAP32('inct') || *(LPDWORD)(p + 7) != BSWAP32('tion'))
				continue;
			p += 11;
			tag = DISTINCTION;
			goto SWITCH_BREAK;
		// [calc]
		CASE_CALC:
			if (close || length < 5)
				continue;
			p += 4;
			tag = CALC;
			goto SWITCH_BREAK;
		// [toggle]
		CASE_TOGG:
			if (close || length < 7 || *(LPWORD)(p + 4) != BSWAP16('le'))
				continue;
			p += 6;
			tag = TOGGLE;
			goto SWITCH_BREAK;
		// [string]
		CASE_STRI:
			if (close || length < 7 || *(LPWORD)(p + 4) != BSWAP16('ng'))
				continue;
			p += 6;
			tag = STRING;
			goto SWITCH_BREAK;
		// [val]
		CASE_VAL1:
			if (close)
				continue;
			p += 3;
			tag = VAL;
			goto SWITCH_BREAK;
		CASE_VAL2:
			if (close || length < 5)
				continue;
			p += 4;
			tag = VAL;
			goto SWITCH_BREAK;
		// [/script]
		CASE_SCRI:
			if (!close || length < 7 || *(LPWORD)(p + 4) != BSWAP16('pt'))
				continue;
			p += 6;
			tag = SCRIPT_CLOSE;
			goto SWITCH_BREAK;
		// [attach]
		CASE_ATTA:
			if (close || length < 7 || *(LPWORD)(p + 4) != BSWAP16('ch'))
				continue;
			p += 6;
			tag = ATTACH;
			goto SWITCH_BREAK;
		// [detach]
		CASE_DETA:
			if (close || length < 7 || *(LPWORD)(p + 4) != BSWAP16('ch'))
				continue;
			p += 6;
			tag = DETACH;
			goto SWITCH_BREAK;
		// [variable], [/variable]
		CASE_VARI:
			if (length < 9 || *(LPDWORD)(p + 4) != BSWAP32('able'))
				continue;
			p += 8;
			tag = VARIABLE_OPEN + close;
			goto SWITCH_BREAK;
		// [expr]
		CASE_EXPR:
			if (close || length < 5)
				continue;
			p += 4;
			tag = EXPR;
			goto SWITCH_BREAK;
		// [define ...]
		CASE_DEFI:
			if (close || length < 9 || *(LPWORD)(p + 4) != BSWAP16('ne') || ((c = p[6]) != ' ' && c != '\t'))
				continue;
			p += 7;
			tag = DEFINE;
			goto SWITCH_BREAK;
		// [undef ...]
		CASE_UNDE:
			if (close || length < 8 || p[4] != 'f' || ((c = p[5]) != ' ' && c != '\t'))
				continue;
			p += 6;
			tag = UNDEF;
			goto SWITCH_BREAK;
		// [allocate]
		CASE_ALLO:
			if (close || length < 9 || *(LPDWORD)(p + 4) != BSWAP32('cate'))
				continue;
			p += 8;
			tag = ALLOCATE;
			goto SWITCH_BREAK;
		// [error_skip], [/error_skip]
		CASE_ERRO:
			if (length < 11 || *(LPDWORD)(p + 4) != BSWAP32('r_sk') || *(LPWORD)(p + 8) != BSWAP16('ip'))
				continue;
			p += 10;
			tag = ERROR_SKIP_OPEN + close;
			goto SWITCH_BREAK;
		// [scope], [/scope]
		CASE_SCOP:
			if (length < 6 || p[4] != 'e')
				continue;
			p += 5;
			tag = SCOPE_OPEN + close;
			goto SWITCH_BREAK;
		// [offset], [/offset]
		CASE_OFFS:
			if (length < 7 || *(LPWORD)(p + 4) != BSWAP16('et'))
				continue;
			p += 6;
			tag = OFFSET_OPEN + close;
			goto SWITCH_BREAK;
		SWITCH_BREAK:
		#pragma endregion

		while ((c = *(p++)) == ' ' || c == '\t');
		if (!c || ((c != ']') ^ (tag == DEFINE || tag == UNDEF)))
			continue;

		switch (tag)
		{
		// [elif]
		case ELIF:
			if (cond)
			{
				invalid = TRUE;
				continue;
			}
		// [if]
		case IF:
			{
				LPSTR Code[2];

				string_begin((string *)Code) = p;
				string_end((string *)Code) = string_end(it);
				invalid = !(cond = Parsing(this, &dummySSGS, (const string *)Code, 0));
			}
			continue;
		// [else]
		case ELSE:
			invalid = cond;
			continue;
		// [endif]
		case ENDIF:
			invalid = FALSE;
			continue;
		default:
			if (invalid)
				continue;
			break;
		}

		switch (tag)	// jump by table
		{
		// [subject], [input]
		case SUBJECT:
		case INPUT:
			{
				string              Name, Code, Tag;
				TSSGSubject         *SSGS; // 項目名、コード部分、項目種別の取得
				TSSGSubjectProperty *prop;

				string_ctor_assign_cstr_with_length(&Code, p, string_end(it) - p);
				TStringDivision_Half_WithoutTokenDtor(&Name, &this->strD, &Code, ":", 1, 0, dtESCAPE);
				TStringDivision_Half_WithoutTokenDtor(&Tag, &this->strD, &Code, ",", 1, 0, dtESCAPE);
				if ((p = string_begin(&Tag))[0] == ',' && p[1] == '\0')
				{
					std_swap(&Tag, &Code);
					string_clear(&Code);
				}

				// この時点でクラス分け
				SSGS = TSSGCtrl_MakeSubjectClass(string_trim_blank(&Tag));
				string_dtor(&Tag);
				if (SSGS)
				{
					if (tag == INPUT)
					{
						string  tmpS;
						BOOLEAN CanUnknown;

						TStringDivision_Half_WithoutTokenDtor(&tmpS, &this->strD, &Code, ",", 1, 0, etTRIM);
						CanUnknown = (p = string_begin(&tmpS))[0] == 'o' && p[1] == 'k' && p[2] == '\0';
						TSSGSubject_SetCanUnknown(SSGS, CanUnknown);
						string_dtor(&tmpS);
						// アドレスは存在しない(その位置に必須入力判定がある)
						string_storage_append(&Code, 2);
						memmove(string_begin(&Code) + 2, string_begin(&Code), string_length(&Code) + 1);
						*(LPWORD)string_begin(&Code) = BSWAP16('0,');
						string_end(&Code) += 2;
					}
					TSSGSubject_SetCode_stdstr(SSGS, &Code);
					TSSGSubject_SetName_stdstr(SSGS, &Name);
				}
				string_dtor(&Code);
				string_dtor(&Name);
				if (!SSGS)
					// 未サポートのタグ
					break;

				vector_push_back(&stack_PTSSDir_top(ParentStack)->childVec, SSGS);
				SSGS->attribute = this->attributeSelector.nowAttributeVec;
				if (SSGS->type == stDIR)
				{
					TDirAttribute *NewAElem;

					// dir
					NewAElem = new_TDirAttribute();
					NewAElem->level = TSSGCtrl_GetDirLevel(this, SSGS) + 1;
					TSSGAttributeSelector_PushElement(&this->attributeSelector, NewAElem);

					stack_PTSSDir_push(ParentStack, (TSSDir *)SSGS);	// 親の切り替え
				}

				TSSGSubject_Setting(SSGS, this);

				prop = GetSubjectProperty(SSGS);
				if (prop)
				{
					prop->RepeatDepth  = RepeatDepth;
					prop->RepeatIndex  = RepeatIndex;
					prop->ParentRepeat = ParentRepeat;
				}
			}
			break;

		// [back]
		case BACK:
			if (stack_PTSSDir_size(ParentStack) > 1)
			{
				TSSGAttributeSelector_PopElementByType(&this->attributeSelector, atDIR_LEVEL);
				stack_PTSSDir_pop(ParentStack);
			}
			break;

		// [root]
		case ROOT:
			{
				unsigned long StackSize;

				StackSize = stack_PTSSDir_size(ParentStack);
				if (StackSize > 1)
				{
					unsigned long i;

					TSSGAttributeSelector_ToRootElementByType(&this->attributeSelector, atDIR_LEVEL);
					i = StackSize - 1;
					do
						stack_PTSSDir_pop(ParentStack);
					while (--i);
				}
			}
			break;

		// [replace]
		case REPLACE:
			{
				string            LineS;
				TReplaceAttribute *NewAElem;
				vector_string     *tmpL;
				string            FName, DefaultExt;

				string_ctor_assign_cstr_with_length(&LineS, p, string_end(it) - p);
				ReplaceDefine(&this->attributeSelector, &LineS);
				NewAElem = new_TReplaceAttribute();
				TReplaceAttribute_Setting(NewAElem, &this->strD, string_c_str(&LineS));
				string_dtor(&LineS);

				string_ctor_assign(&FName, &NewAElem->fileName);
				string_ctor_assign_cstr_with_length(&DefaultExt, ".SSC", 4);
				tmpL = TSSGCtrl_GetSSGDataFile(this, NULL, FName, DefaultExt, NULL);
				if (tmpL == NULL)
				{
					delete_TReplaceAttribute(NewAElem);
					break;
				}
				// [replace]属性を追加
				TSSGAttributeSelector_AddElement(&this->attributeSelector, NewAElem);

				// 再帰
				TSSGCtrl_EnumReadSSG(this, tmpL, ParentStack, ADJElem, RepeatIndex, ParentRepeat);
				// 帰ってきたので、[replace]属性を外す
				TSSGAttributeSelector_EraseElementByType(&this->attributeSelector, atREPLACE);
			}
			break;

		// [size]
		case SIZE_OPEN:
			{
				// アクセスサイズ強制指定命令
				string LineS;
				long   Size;

				string_ctor_assign_cstr_with_length(&LineS, p, string_end(it) - p);
				ReplaceDefine(&this->attributeSelector, &LineS);
				Size = TStringDivision_ToLongDef(&LineS, -1);
				string_dtor(&LineS);
				if (Size >= 0)
				{
					TSizeAttribute *NewAElem;

					// [size]属性を付加
					NewAElem = new_TSizeAttribute(); // size
					TSizeAttribute_Setting(NewAElem, Size);
					TSSGAttributeSelector_PushElement(&this->attributeSelector, NewAElem);
				}
			}
			break;

		// [/size]
		case SIZE_CLOSE:
			// [size]属性を除去
			TSSGAttributeSelector_PopElementByType(&this->attributeSelector, atSIZE);
			break;

		// [adjustment]
		case ADJUSTMENT_OPEN:
			{
				string               LineS, tmpS;
				TAdjustmentAttribute *NewAElem;

				// [adjustment]属性を付加
				string_ctor_assign_cstr_with_length(&LineS, p, string_end(it) - p);
				ReplaceDefine(&this->attributeSelector, &LineS);
				TStringDivision_Half_WithoutTokenDtor(&tmpS, &this->strD, &LineS, ",", 1, 0, 0);
				NewAElem = TSSGCtrl_MakeAdjustmentClass(&tmpS);
				string_dtor(&tmpS);
				if (NewAElem != NULL)
				{
					TAdjustmentAttribute_Setting(NewAElem, this, string_c_str(&LineS));
					TSSGAttributeSelector_PushElement(&this->attributeSelector, NewAElem);
				}
				string_dtor(&LineS);
			}
			break;

		// [/adjustment]
		case ADJUSTMENT_CLOSE:
			// [adjustment]属性を除去
			TSSGAttributeSelector_PopElementByType(&this->attributeSelector, atADJUST);
			break;

		// [adjust_check]
		case ADJUST_CHECK_OPEN:
			{
				string                LineS;
				TAdjustCheckAttribute *NewAElem;

				// [adjust_check]属性を付加
				string_ctor_assign_cstr_with_length(&LineS, p, string_end(it) - p);
				ReplaceDefine(&this->attributeSelector, &LineS);
				NewAElem = new_TAdjustCheckAttribute();
				TAdjustCheckAttribute_Setting(NewAElem, &this->strD, string_c_str(&LineS));
				string_dtor(&LineS);
				TSSGAttributeSelector_PushElement(&this->attributeSelector, NewAElem);
			}
			break;

		// [/adjust_check]
		case ADJUST_CHECK_CLOSE:
			// [adjust_check]属性を除去
			TSSGAttributeSelector_PopElementByType(&this->attributeSelector, atADJUST_CHECK);
			break;

		// [funnel]
		case FUNNEL_OPEN:
			{
				string           LineS;
				TFunnelAttribute *NewAElem;

				// [funnel]属性を付加
				string_ctor_assign_cstr_with_length(&LineS, p, string_end(it) - p);
				ReplaceDefine(&this->attributeSelector, &LineS);
				NewAElem = new_TFunnelAttribute();
				TFunnelAttribute_Setting(NewAElem, &LineS);
				string_dtor(&LineS);
				TSSGAttributeSelector_PushElement(&this->attributeSelector, NewAElem);
			}
			break;

		// [/funnel]
		case FUNNEL_CLOSE:
			// [funnel]属性を除去
			TSSGAttributeSelector_PopElementByType(&this->attributeSelector, atFUNNEL);
			break;

		// [repeat]
		case REPEAT:
			{
				string LineS;

				// 繰り返し書式発動！
				string_ctor_assign_cstr_with_length(&LineS, p, string_end(it) - p);
				ReplaceDefine(&this->attributeSelector, &LineS);
				repeat_ReadSSRFile(this, ParentStack, ADJElem, &LineS, RepeatIndex, ParentRepeat, &dummySSGS);
				string_dtor(&LineS);
			}
			break;

		// [io_fep]
		case IO_FEP_OPEN:
			{
				string           LineS;
				TIO_FEPAttribute *NewAElem;

				// [io_fep]属性を付加
				string_ctor_assign_cstr_with_length(&LineS, p, string_end(it) - p);
				ReplaceDefine(&this->attributeSelector, &LineS);
				NewAElem = new_TIO_FEPAttribute(); // io_fep
				TIO_FEPAttribute_Setting(NewAElem, &this->strD, string_c_str(&LineS));
				string_dtor(&LineS);
				TSSGAttributeSelector_PushElement(&this->attributeSelector, NewAElem);
			}
			break;

		// [/io_fep]
		case IO_FEP_CLOSE:
			// [io_fep]属性を除去
			TSSGAttributeSelector_PopElementByType(&this->attributeSelector, atIO_FEP);
			break;

		// [e_with]
		case E_WITH_OPEN:
			{
				string            LineS;
				TEndWithAttribute *NewAElem;

				// [e_with]属性を付加
				string_ctor_assign_cstr_with_length(&LineS, p, string_end(it) - p);
				ReplaceDefine(&this->attributeSelector, &LineS);
				NewAElem = new_TEndWithAttribute(); // e_with
				TEndWithAttribute_Setting_stdstr(NewAElem, &LineS);
				string_dtor(&LineS);
				TSSGAttributeSelector_PushElement(&this->attributeSelector, NewAElem);
			}
			break;

		// [/e_with]
		case E_WITH_CLOSE:
			// [e_with]属性を除去
			TSSGAttributeSelector_PopElementByType(&this->attributeSelector, atE_WITH);
			break;

		// [enabled]
		case ENABLED_OPEN:
			{
				string            LineS;
				TEnabledAttribute *NewAElem;

				// [enabled]属性を付加
				string_ctor_assign_cstr_with_length(&LineS, p, string_end(it) - p);
				ReplaceDefine(&this->attributeSelector, &LineS);
				NewAElem = new_TEnabledAttribute(); // enabled
				TEnabledAttribute_Setting(NewAElem, &LineS);
				string_dtor(&LineS);
				TSSGAttributeSelector_AddElement(&this->attributeSelector, NewAElem);
			}
			break;

		// [/enabled]
		case ENABLED_CLOSE:
			// [enabled]属性を除去
			TSSGAttributeSelector_EraseElementByType(&this->attributeSelector, atENABLED);
			break;

		// [child_rw]
		case CHILD_RW_OPEN:
			{
				string            LineS;
				TChildRWAttribute *NewAElem;

				// [child_rw]属性を付加
				string_ctor_assign_cstr_with_length(&LineS, p, string_end(it) - p);
				ReplaceDefine(&this->attributeSelector, &LineS);
				NewAElem = new_TChildRWAttribute(); // child_rw
				TChildRWAttribute_Setting(NewAElem, NULL, string_c_str(&LineS));
				string_dtor(&LineS);
				TSSGAttributeSelector_PushElement(&this->attributeSelector, NewAElem);
			}
			break;

		// [/child_rw]
		case CHILD_RW_CLOSE:
			// [child_rw]属性を除去
			TSSGAttributeSelector_PopElementByType(&this->attributeSelector, atCHILD_RW);
			break;

		// [caution]
		case CAUTION_OPEN:
			{
				string            LineS;
				TCautionAttribute *NewAElem;

				// [caution]属性を付加
				string_ctor_assign_cstr_with_length(&LineS, p, string_end(it) - p);
				ReplaceDefine(&this->attributeSelector, &LineS);
				NewAElem = new_TCautionAttribute(); // caution
				TCautionAttribute_Setting(NewAElem, &LineS);
				string_dtor(&LineS);
				TSSGAttributeSelector_PushElement(&this->attributeSelector, NewAElem);
			}
			break;

		// [/caution]
		case CAUTION_CLOSE:
			// [caution]属性を除去
			TSSGAttributeSelector_PopElementByType(&this->attributeSelector, atCAUTION);
			break;

		// [involve]
		case INVOLVE:
			{
				string        FName;
				const char    *end;
				vector_string tmpV;

				// 埋め込みファイル
				string_ctor_assign(&FName, &this->scriptDir);
				end = TrimBlank(&p, string_end(it));
				string_append_cstr_with_length(&FName, p, end - p);
				ReplaceDefine(&this->attributeSelector, &FName);
				string_append_cstr_with_length(&FName, ".ssl", 4);
				vector_ctor(&tmpV);
				vector_string_reserve(&tmpV, vector_end(SSGFile) - it);
				while (++it != vector_end(SSGFile))
				{
					c = *(p = string_begin(it));
					while (c == ' ' || c == '\t')
						c = *(++p);
					if ((string_end(it) - p) >= 10 &&
						*(LPDWORD) p      == BSWAP32('[/in') &&
						*(LPDWORD)(p + 4) == BSWAP32('volv') &&
						*         (p + 8) ==         'e'    )
					{
						p += 9;
						do
							if ((c = *(p++)) == ']')
								goto INVOLVE_NESTED_BREAK;
						while (c == ' ' || c == '\t');
					}
					vector_string_push_back(&tmpV, it);
				}
			INVOLVE_NESTED_BREAK:
				vector_shrink_to_fit(&tmpV);
				TSSGCtrl_SetSSGDataFile(this, &tmpV, &FName, TRUE);
				vector_string_dtor(&tmpV);
				string_dtor(&FName);
				if (it == vector_end(SSGFile))
					return;  // [/involve]が存在しない
			}
			break;

		// [note]
		case NOTE:
			{
				const char    *end;
				string        LineS;
				BOOLEAN       IsMemoWordWrap;
				vector_string *memo;

				end = TrimBlank(&p, string_end(it));
				string_ctor_assign_cstr_with_length(&LineS, p, end - p);
				ReplaceDefine(&this->attributeSelector, &LineS);
				IsMemoWordWrap = (string_length(&LineS) == 8 && *(LPDWORD)(p = string_begin(&LineS)) == BSWAP32('word') && *(LPDWORD)(p + 4) == BSWAP32('wrap'));
				string_dtor(&LineS);
				if (!ADJElem)
				{
					// SSGモード
					this->isMemoWordWrap = IsMemoWordWrap;
					memo = &this->memo;
				}
				else
				{
					// ADJモード
					ADJElem->isMemoWordWrap = IsMemoWordWrap;
					memo = &ADJElem->memo;
				}
				vector_string_dtor(memo);
				vector_ctor(memo);
				vector_string_reserve(memo, vector_end(SSGFile) - it);
				while (++it != vector_end(SSGFile))
				{
					c = *(p = string_begin(it));
					while (c == ' ' || c == '\t')
						c = *(++p);
					if ((string_end(it) - p) >= 7 &&
						*(LPDWORD) p      == BSWAP32('[/no') &&
						*(LPWORD )(p + 4) == BSWAP16('te'  ))
					{
						p += 6;
						do
							if ((c = *(p++)) == ']')
								// [/note]
								goto NOTE_NESTED_BREAK;
						while (c == ' ' && c == '\t');
					}
					vector_string_push_back(memo, it);
				}
			NOTE_NESTED_BREAK:
				vector_shrink_to_fit(memo);
				if (it == vector_end(SSGFile))
					return;  // [/note]が存在しない
			}
			break;

		// [process]
		case PROCESS:
			{
				string LineS;

				string_ctor_assign_cstr_with_length(&LineS, p, string_end(it) - p);
				ReplaceDefine(&this->attributeSelector, &LineS);
				vector_string_push_back(&this->processNameVec, &LineS);
				string_dtor(&LineS);
			}
			break;

		// [title]
		case TITLE:
			{
				string LineS;

				string_ctor_assign_cstr_with_length(&LineS, p, string_end(it) - p);
				ReplaceDefine(&this->attributeSelector, &LineS);
				TSSGScriptStruct_SetDistinction_stdstr(&this->script, "title", &LineS);
				string_dtor(&LineS);
			}
			break;

		// [maker]
		case MAKER:
			{
				string LineS;

				string_ctor_assign_cstr_with_length(&LineS, p, string_end(it) - p);
				ReplaceDefine(&this->attributeSelector, &LineS);
				TSSGScriptStruct_SetDistinction_stdstr(&this->script, "maker", &LineS);
				string_dtor(&LineS);
			}
			break;

		// [creator]
		case CREATOR:
			{
				string LineS;

				string_ctor_assign_cstr_with_length(&LineS, p, string_end(it) - p);
				ReplaceDefine(&this->attributeSelector, &LineS);
				TSSGScriptStruct_SetDistinction_stdstr(&this->script, "creator", &LineS);
				string_dtor(&LineS);
			}
			break;

		// [distinction]
		case DISTINCTION:
			{
				string LineS, tmpS;

				string_ctor_assign_cstr_with_length(&LineS, p, string_end(it) - p);
				ReplaceDefine(&this->attributeSelector, &LineS);
				TStringDivision_Half_WithoutTokenDtor(&tmpS, &this->strD, &LineS, "=", 1, 0, dtESCAPE | etREPLACE | etTRIM);
				if (*(p = string_begin(&tmpS)) != '=' || *(p + 1) != '\0')
					TSSGScriptStruct_SetDistinction_stdstr(&this->script, string_c_str(&tmpS), &LineS);
				string_dtor(&tmpS);
				string_dtor(&LineS);
			}
			break;

		// [calc], [toggle], [string]
		case CALC:
		case TOGGLE:
		case STRING:
			{
				string              LineS, tmpS, Name;
				TSSGSubject         *SSGS;
				BOOLEAN             CanUnknown;
				TSSGSubjectProperty *prop;

				string_ctor_assign_cstr_with_length(&LineS, p, string_end(it) - p);
				ReplaceDefine(&this->attributeSelector, &LineS);
				TStringDivision_Half_WithoutTokenDtor(&tmpS, &this->strD, &LineS, ",", 1, tag != TOGGLE ? 1 : 3, 0);
				if (tag == CALC)
					SSGS = &new_TSSCalc()->super;
				else if (tag == TOGGLE)
					SSGS = &new_TSSToggle()->super;
				else/* if (tag == STRING)*/
					SSGS = &new_TSSString()->super;
				TStringDivision_Half_WithoutTokenDtor(&Name, &this->strD, &tmpS, ",", 1, 0, etTRIM);
				CanUnknown = (p = string_begin(&tmpS))[0] == 'o' && p[1] == 'k' && p[2] == '\0';
				TSSGSubject_SetCanUnknown(SSGS, CanUnknown);
				string_dtor(&tmpS);
				TSSGSubject_SetName_stdstr(SSGS, &Name);
				string_dtor(&Name);
				string_storage_append(&LineS, 2);
				memmove(string_begin(&LineS) + 2, string_begin(&LineS), string_length(&LineS) + 1);
				*(LPWORD)string_begin(&LineS) = BSWAP16('0,');
				string_end(&LineS) += 2;
				TSSGSubject_SetCode_stdstr(SSGS, &LineS);
				string_dtor(&LineS);
				TSSGSubject_Setting(SSGS, this);
				vector_push_back(&stack_PTSSDir_top(ParentStack)->childVec, SSGS);
				SSGS->attribute = this->attributeSelector.nowAttributeVec;
				prop = GetSubjectProperty(SSGS);
				if (prop)
				{
					prop->RepeatDepth  = RepeatDepth;
					prop->RepeatIndex  = RepeatIndex;
					prop->ParentRepeat = ParentRepeat;
				}
			}
			break;

		// [val]
		case VAL:
			if (ADJElem)
			{
				string_assign_cstr_with_length(&ADJElem->valStr, p, string_end(it) - p);
				ReplaceDefine(&this->attributeSelector, &ADJElem->valStr);
				string_shrink_to_fit(&ADJElem->valStr);
			}
			break;

		// [/script]
		case SCRIPT_CLOSE:
			return;

		// [attach]
		case ATTACH:
			{
				string_dtor(&ProcessAttachCode);
				string_ctor_null(&ProcessAttachCode);
				while (++it != vector_end(SSGFile))
				{
					char *first, *last;

					c = *(p = string_begin(it));
					while (c == ' ' || c == '\t')
						c = *(++p);
					if ((string_end(it) - p) >= 9 &&
						*(LPDWORD) p      == BSWAP32('[/at') &&
						*(LPDWORD)(p + 4) == BSWAP32('tach'))
					{
						p += 8;
						do
							if ((c = *(p++)) == ']')
								// [/attach]
								goto ATTACH_NESTED_BREAK;
						while (c == ' ' && c == '\t');
					}
					first = string_begin(it);
					last = TrimSpace(&first, string_end(it));
					string_append_range(&ProcessAttachCode, first, last);
					string_push_back(&ProcessAttachCode, '\n');
				}
			ATTACH_NESTED_BREAK:
				if (!string_empty(&ProcessAttachCode))
				{
					*(--string_end(&ProcessAttachCode)) = '\0';
					string_shrink_to_fit(&ProcessAttachCode);
				}
				if (it == vector_end(SSGFile))
					return;  // [/attach]が存在しない
			}
			break;

		// [detach]
		case DETACH:
			{
				string_dtor(&ProcessDetachCode);
				string_ctor_null(&ProcessDetachCode);
				while (++it != vector_end(SSGFile))
				{
					char *first, *last;

					c = *(p = string_begin(it));
					while (c == ' ' || c == '\t')
						c = *(++p);
					if ((string_end(it) - p) >= 9 &&
						*(LPDWORD) p      == BSWAP32('[/de') &&
						*(LPDWORD)(p + 4) == BSWAP32('tach'))
					{
						p += 8;
						do
							if ((c = *(p++)) == ']')
								// [/detach]
								goto DETACH_NESTED_BREAK;
						while (c == ' ' && c == '\t');
					}
					first = string_begin(it);
					last = TrimSpace(&first, string_end(it));
					string_append_range(&ProcessDetachCode, first, last);
					string_push_back(&ProcessDetachCode, '\n');
				}
			DETACH_NESTED_BREAK:
				if (!string_empty(&ProcessDetachCode))
				{
					*(--string_end(&ProcessDetachCode)) = '\0';
					string_shrink_to_fit(&ProcessDetachCode);
				}
				if (it == vector_end(SSGFile))
					return;  // [/detach]が存在しない
			}
			break;

		// [variable]
		case VARIABLE_OPEN:
			Attribute_variable_open(this, ParentStack, p, string_end(it));
			break;

		// [/variable]
		case VARIABLE_CLOSE:
			// [variable]属性を除去
			Attribute_variable_close(this);
			break;

		// [expr]
		case EXPR:
			Attribute_expr(this, p, string_end(it));
			break;

		// [define ...]
		case DEFINE:
			Attribute_define(this, ParentStack, --p, string_end(it));
			break;

		// [undef ...]
		case UNDEF:
			Attribute_undef(this, --p);
			break;

		// [allocate]
		case ALLOCATE:
			Attribute_allcate(p, string_end(it));
			break;

		// [error_skip]
		case ERROR_SKIP_OPEN:
			Attribute_error_skip_open(this);
			break;

		// [/error_skip]
		case ERROR_SKIP_CLOSE:
			// [error_skip]属性を除去
			Attribute_error_skip_close(this);
			break;

		// [scope]
		case SCOPE_OPEN:
			{
				string code;

				string_ctor_assign_cstr_with_length(&code, p, string_end(it) - p);
				Attribute_scope_open(this, &code);
				string_dtor(&code);
			}
			break;

		// [/scope]
		case SCOPE_CLOSE:
			Attribute_scope_close(this);
			break;

		// [offset]
		case OFFSET_OPEN:
			{
				string code;

				string_ctor_assign_cstr_with_length(&code, p, string_end(it) - p);
				Attribute_offset_open(this, &code);
				string_dtor(&code);
			}
			break;

		// [/offset]
		case OFFSET_CLOSE:
			Attribute_offset_close(this);
			break;

		// [format]
		case FORMAT_OPEN:
			{
				static struct _Constant {
					struct _Term {
						size_t   Length;
						LPCSTR   String;
						uint64_t Value;
					} const terms[10];
				} const constants = { {
					{ 7, "UNKNOWN", 1 << atUNKNOWN     },
					{ 4, "LONG"   , 1 << atLONG        },
					{ 5, "INDEX"  , 1 << atLONG_INDEX  },
					{ 4, "BOOL"   , 1 << atBOOL        },
					{ 6, "STRING" , 1 << atSTRING      },
					{ 6, "VECTOR" , 1 << atBOOL_VECTOR },
					{ 6, "DOUBLE" , 1 << atDOUBLE      },
					{ 3, "ANY"    , ~(-1 << atDIR)     },
					{ 3, "DIR"    , 1 << atDIR         },
					{ 3, "ALT"    , (uint32_t)LONG_MIN },
				} };

				string           LineS, Token;
				TFormatAttribute *NewAElem;
				vector_string    tmpV = { NULL };

				string_ctor_assign_cstr_with_length(&LineS, p, string_end(it) - p);
				ReplaceDefine(&this->attributeSelector, &LineS);
				string_ctor_assign_char(&Token, ',');
				TStringDivision_List(&this->strD, &LineS, Token, &tmpV, dtESCAPE | etREPLACE);
				vector_string_resize(&tmpV, 3);
				string_dtor(&LineS);

				NewAElem = operator_new(sizeof(TReplaceAttribute));
				NewAElem->VTable    = TReplaceAttribute_VTable;
				NewAElem->type      = atFORMAT;
				NewAElem->offsetNum = Parsing(this, &dummySSGS, &vector_at(&tmpV, 0), constants, 0);
				string_ctor_assign(&NewAElem->offsetCode , &vector_at(&tmpV, 1));
				string_ctor_assign(&NewAElem->fileName   , &vector_at(&tmpV, 2));
				vector_dtor(&tmpV);

				TSSGAttributeSelector_AddElement(&this->attributeSelector, NewAElem);
			}
			break;

		// [/format]
		case FORMAT_CLOSE:
			TSSGAttributeSelector_EraseElementByType(&this->attributeSelector, atFORMAT);
			break;
		}
	}

	#undef stack_PTSSDir_size
	#undef stack_PTSSDir_top
	#undef stack_PTSSDir_push
	#undef stack_PTSSDir_pop
}
