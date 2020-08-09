#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_operator.h"
#include "TSSGCtrl.h"
#include "TMainForm.h"
#include "SSGSubjectProperty.h"

EXTERN_C BOOL FixTheProcedure;
EXTERN_C BOOL ExtensionTSSDir;
EXTERN_C void * __cdecl TSSGCtrl_ReadSSG_PushElement(void *, void *);
EXTERN_C unsigned long __cdecl Parsing(IN TSSGCtrl *this, IN TSSGSubject *SSGS, IN const bcb6_std_string *Src, ...);

unsigned seqElement = MAXDWORD;

static __declspec(naked) uint64_t __cdecl TSSGAttributeSelector_StartElementCheck_new_attributeSetMap(size_t __n)
{
	__asm {
		xor  eax, eax
		mov  seqElement, eax
		mov  edx, [esi]TSSGAttributeSelector.attributeSetMap
		test edx, edx
		jnz  REVERT

		mov  ecx, dword ptr [esp + 4]
#if OPTIMIZE_ALLOCATOR
		call bcb6_operator_new
#else
		push ecx
		call bcb6_operator_new
		pop  ecx
#endif
		xor  edx, edx

	REVERT:
		ret
	}
}

static BOOL __fastcall TSSGAttributeSelector_EndElementCheck_delete_attributeSetMap(TSSGAttributeSelector *this, BOOL clear)
{// delete attributeSetMap if return TRUE
	if (clear)
		return TRUE;
	else if (ExtensionTSSDir)
		return FALSE;
	else
	{
		unsigned dicard = 0;
		for (list_iterator It = list_end(&this->allAtteributeVecList);
			 list_iterator_decrement(It) != list_end(&this->allAtteributeVecList);
			 )
		{
			vector *const AttrV = *(vector *const *)It->_M_data;
			if (AttrV->allocator_type[0])
			{
				It = list_erase(It);
				vector_dtor(AttrV);
				bcb6_operator_delete(AttrV);
				dicard++;
			}
		}
		if (MainForm->userMode > 1)
		{
			EXTERN_C int __fastcall GuidePrint(const char *format, ...);

			GuidePrint("Discard vector<TSSGAttributeElement *>[%u]\n", dicard);
		}
		return TRUE;
	}
}

void *__cdecl TSSGAttributeSelector_AddElement_MakeOnlyOneAtteribute(TSSGAttributeSelector *const this, TAdjustmentAttribute *AElem)
{
	static void *(__cdecl *const TSSGAttributeSelector_MakeOnlyOneAtteribute)(TSSGAttributeSelector *, void *) = (void *)0x004D5764;

	AElem = TSSGAttributeSelector_MakeOnlyOneAtteribute(this, AElem);
	AElem->seqElement = TSSGAttributeElement_GetType(AElem) == atDEFINE
		? string_length(&((TDefineAttribute *)AElem)->inputCode)
		: seqElement++// renumbering when redeclared too
		;// store into allocator_type[1], except atSCOPE
	return AElem;
}

static int __cdecl cmpElement(LPCVOID const A, LPCVOID const B)
{
	const TAdjustmentAttribute *const a = *(TAdjustmentAttribute **)A;
	const TAdjustmentAttribute *const b = *(TAdjustmentAttribute **)B;
	signed diff = a->type - b->type;
	return diff ? diff : TSSGAttributeElement_GetType(a) == atDEFINE ?
		strcmp(a->c_str, b->c_str) : a->seqElement - b->seqElement;
}

#pragma intrinsic(_BitScanForward)
void __stdcall TSSGAttributeSelector_MakeNowAttributeVec_attributeSetMap_insert(const struct
{
	map_iterator first;
	bool         second;
} *const retVal, const map *const attributeSetMap, const struct
{
	set     AESet;
	vector *NewVec;
} *const __v, const list_iterator It)
{
	COORD coords[0x20] = { { 0, 0 } };
	vector const index = { coords, coords + _countof(coords) - 1, NULL, NULL, coords, MAXDWORD };

	vector *const NewVec = __v->NewVec;
	qsort(vector_begin(NewVec), vector_size_by_type(NewVec, void *), sizeof(void *), cmpElement);

	for (TSSGAttributeElement **VIt = vector_begin(NewVec);
		 VIt < (TSSGAttributeElement **)vector_end(NewVec);
		 VIt++)
	{
		DWORD Index;
		_BitScanForward(&Index, TSSGAttributeElement_GetType(*VIt));
		if (coords[Index].X++ == 0)
			coords[Index].Y = VIt - (TSSGAttributeElement **)vector_begin(NewVec);
	}

	NewVec->allocator_type[0] = retVal->first;
	NewVec->allocator_type[1] = NULL;
	NewVec->tblIndex = SubjectStringTable_insert((string *)&index);
}

CONSOLE_FONT_INFO __fastcall TSSGAttributeElement_GetViaCoord(AeType const Type, const vector *const AttrV)
{
	DWORD Index;
	_BitScanForward(&Index, Type);
	COORD coord = vector_type_at(&vector_at(&SubjectStringTable_array, AttrV->tblIndex), COORD, Index);
	return (CONSOLE_FONT_INFO) { coord.X ? vector_type_at(AttrV, DWORD, coord.Y) : 0, coord };
}

struct GetAddressStack
{
	TSSGCtrl *SSGCtrl;
	TSSGSubject *SSGS;
};

static struct GetAddressStack __stdcall TSSGCtrl_AddressAttributeFilter_GetAddress(TReplaceAttribute const *const *const it, LPCVOID const Address, va_list fp)
{
	TSSGCtrl *SSGCtrl = (va_arg(fp, void *), va_arg(fp, void *), va_arg(fp, void *));
	TSSGSubject *SSGS = va_arg(fp, void *);
	switch ((*it)->displace)
	{
#if EMBED_BREADTH
		TSSGSubjectProperty *prop;
	case TRUE:
		if (SSGS && (prop = GetSubjectProperty(SSGS)) && prop->ParentEntry)
			SSGS = &prop->ParentEntry->super;
		break;
#endif
	case TRUE + TRUE:
		*va_arg(fp, LPCVOID *) = Address;
		break;
	}
	return (struct GetAddressStack) { SSGCtrl, SSGS };
}

static TFunnelAttribute *__fastcall TSSGCtrl_CheckFunnel_MultiStage(va_list args, TSSGSubject *SSGS)
{
	if (SSGS->attribute)
	{
		const COORD coord = TSSGAttributeElement_GetViaCoord(atFUNNEL, SSGS->attribute).dwFontSize;
		if (coord.X)
		{
			TSSGCtrl *this = va_arg(args, void *);
			unsigned long Val = (va_arg(args, void *), va_arg(args, unsigned long));
			TOnErrorAttribute *err = TSSGCtrl_GetAttribute(this, SSGS, atON_ERROR);
			for (TFunnelAttribute **base = &vector_type_at(SSGS->attribute, TFunnelAttribute *, coord.Y),
					 **cur  = base + coord.X;
					 --cur >= base; )
				if (!TSSGCtrl_Funneling(this, SSGS, &(*cur)->fileName, Val) && !err)
					break;
		}
	}
	return NULL;// means processed
}

static __declspec(naked) TSSGAttributeElement *TSSGCtrl_CheckFunnel_GetAttribute(TSSGCtrl *this, TSSGSubject *SSGS, AeType Type)
{
	__asm {// edx is SSGS already
		lea ecx, [ebp + 8]// "this" on frame
		cmp FixTheProcedure, 0
#pragma warning(suppress: 4414)
		jne TSSGCtrl_CheckFunnel_MultiStage
		jmp TSSGCtrl_GetAttribute
		ud2
	}
}

static TIO_FEPAttribute *__fastcall TSSGCtrl_CheckIO_FEP_MultiStage(va_list args, TSSGSubject *SSGS)
{
	static const char VarName[] = "Val";
	if (SSGS->attribute)
	{
		const COORD coord = TSSGAttributeElement_GetViaCoord(atIO_FEP, SSGS->attribute).dwFontSize;
		if (coord.X)
		{
			TSSGCtrl *this = va_arg(args, void *);
			unsigned long *Val = (va_arg(args, void *), &va_arg(args, unsigned long));
			if (!va_arg(args, bool))
				for (TIO_FEPAttribute **cur  = &vector_type_at(SSGS->attribute, TIO_FEPAttribute *, coord.Y), **apex = cur + coord.X;
					 cur < apex;
					 cur++)
					*Val = Parsing(this, SSGS, &(*cur)->outputCode, sizeof(VarName) - sizeof(*VarName), VarName, (uint64_t)*Val, 0);
			else
				for (TIO_FEPAttribute **base = &vector_type_at(SSGS->attribute, TIO_FEPAttribute *, coord.Y),
					 **cur  = base + coord.X;
					 --cur >= base; )
					*Val = Parsing(this, SSGS, &(*cur)->inputCode , sizeof(VarName) - sizeof(*VarName), VarName, (uint64_t)*Val, 0);
		}
	}
	return NULL;// means processed
}

static __declspec(naked) TSSGAttributeElement *TSSGCtrl_CheckIO_FEP_GetAttribute(TSSGCtrl *this, TSSGSubject *SSGS, AeType Type)
{
	__asm {// edx is SSGS already
		lea ecx, [ebp + 8]// "this" on frame
		cmp FixTheProcedure, 0
#pragma warning(suppress: 4414)
		jne TSSGCtrl_CheckIO_FEP_MultiStage
		jmp TSSGCtrl_GetAttribute
		ud2
	}
}

static __declspec(naked) void __fastcall THeapAdjustmentAttribute_dtor_heapMap(THeapAdjustmentAttribute *this, struct _Rb_tree_node *root)
{
	static void(__cdecl *const tree_ulong_pair_uu_erase)() = (void *)0x004D0A80;
	static void(__cdecl *const tree_string_double_erase)() = (void *)0x0051C168;

	__asm {
		mov   eax, tree_ulong_pair_uu_erase
		cmp   [ecx]TSSGAttributeElement.type, atSCOPE
		cmove eax, tree_string_double_erase
		jmp   eax
		ud2
	}
}

#define PUSH_EDX  (BYTE )0x52
#define PUSH_EBX  (BYTE )0x53
#define PUSH_EBP  (BYTE )0x55
#define CALL_REL  (BYTE )0xE8
#define JMP_REL32 (BYTE )0xE9
#define JMP_REL8  (BYTE )0xEB
#define JZ_SHORT  (BYTE )0x74
#define JNZ_SHORT (BYTE )0x75
#define JECXZ     (BYTE )0xE3
#define NOP       (BYTE )0x90
#define NOP_X4    (DWORD)0x00401F0F

EXTERN_C void __cdecl Attach_EnumReadSSG()
{
	// TSSGAttributeSelector::StartElementCheck
	//   attributeSetMap!=NULL => nowAttributeList!=NULL
	*(LPBYTE )(0x004D2D6D + 2) = offsetof(TSSGAttributeSelector, nowAttributeList);
	*(LPDWORD)(0x004D2D85 + 1) = (DWORD)TSSGAttributeSelector_StartElementCheck_new_attributeSetMap - (0x004D2D85 + 1 + sizeof(DWORD));
	*(LPDWORD)(0x004D2D90 + 2) = 0x004D2E25 - (0x004D2D90 + 2 + sizeof(DWORD));// EAX is NULL then use EDX

	// TSSGAttributeSelector::EndElementCheck
	//   retain attributeSetMap if caller isn't TSSGAttributeSelector::Clear
	*(LPWORD )0x004D3686 = BSWAP16(0x8BCB    );// mov    ecx, ebx  
	*(LPDWORD)0x004D3688 = BSWAP32(0x8B45042D);// mov    eax, dword ptr [_AddressOfReturnAddress]
	*(LPDWORD)0x004D368C =         0x004D2D15 ;// sub    eax, TSSGAttributeSelector::Clear
	*(LPDWORD)0x004D3690 = BSWAP32(0xF7D81BD2);// neg    eax; sbb edx, edx  
	*(LPDWORD)0x004D3694 = BSWAP32(0x83C201E8);// add    edx, 1
	*(LPDWORD)0x004D3698 = (DWORD)TSSGAttributeSelector_EndElementCheck_delete_attributeSetMap - (0x004D3698 + sizeof(DWORD));
	*(LPDWORD)0x004D369C = BSWAP32(0x83632800);// and    dword ptr [nowAttributeVec], 0
	*(LPWORD )0x004D36A0 = BSWAP16(0x85C0    );// test   eax, eax
	*(LPBYTE )0x004D36A3 =         0x4B       ;// eax => ecx
	*(LPBYTE )0x004D36A6 =         0x4D       ;// eax => ecx
	*(LPDWORD)0x004D36A8 = BSWAP32(0x0F44C890);// cmovz  ecx, eax
	*(LPBYTE )0x004D36AC = JECXZ;
	*(LPBYTE )0x004D36AD = 0x004D3722 - (0x004D36AD + sizeof(BYTE));

	// TSSGAttributeSelector::AddElement
	*(LPDWORD)0x004D3DCA = (DWORD)TSSGAttributeSelector_AddElement_MakeOnlyOneAtteribute - (0x004D3DCA + sizeof(DWORD));

	// TSSGAttributeSelector::MakeNowAttributeVec
	*(LPBYTE )(0x004D5CF1 + 0) =         0x83   ;// add  dword ptr [ebx + 0x1C], 2
	*(LPWORD )(0x004D5CF1 + 3) = BSWAP16(0x0257);// push edi
	*(LPBYTE ) 0x004D5CF6      = NOP;

	*(LPBYTE )(0x004D5DFB + 0) = CALL_REL;
	*(LPDWORD)(0x004D5DFB + 1) = (DWORD)TSSGAttributeSelector_MakeNowAttributeVec_attributeSetMap_insert - (0x004D5DFB + 1 + sizeof(DWORD));
	*(LPBYTE ) 0x004D5E00      = NOP;
	*(LPBYTE )(0x004D5E01 + 0) =         0x83;
	*(LPBYTE )(0x004D5E01 + 1) =         0x6B;
	*(LPWORD )(0x004D5E01 + 3) = BSWAP16(0x0366);
	*(LPBYTE ) 0x004D5E06      = NOP;

	// TSSGCtrl::ReadSSG
	*(LPBYTE )(0x004E45C8 + 1) = sizeof(TDirAttribute);

	*(LPBYTE )(0x004E45F7 + 0) = 0x89;// mov [eax]
	*(LPBYTE )(0x004E45F7 + 1) = 0x08;// ecx
	*(LPBYTE )(0x004E45F9 + 0) = 0x8B;// mov edx, 
	*(LPBYTE )(0x004E45F9 + 1) = 0x93;// [ebx + ... 
	*(LPDWORD)(0x004E45F9 + 2) = offsetof(TSSGCtrl, rootSubject);
	*(LPBYTE )(0x004E4601 + 1) = 0x48;// edx => eax 
	*(LPWORD )(0x004E4604 + 0) = BSWAP16(0x8948);// mov [eax + ...], ecx
	*(LPBYTE )(0x004E4604 + 2) = offsetof(TDirAttribute, identity);
	*(LPBYTE )(0x004E4617 + 0) = 0x89;// mov [eax + ...]
	*(LPBYTE )(0x004E4617 + 1) = 0x50;// edx
	*(LPBYTE )(0x004E4617 + 2) = offsetof(TDirAttribute, arbitrary);

	*(LPDWORD)(0x004E4629 + 1) = (DWORD)TSSGCtrl_ReadSSG_PushElement - (0x004E4629 + 1 + sizeof(DWORD));

	// TSSGCtrl::EnumReadSSG
	*(LPBYTE )0x004E5090 = JMP_REL32;
	*(LPDWORD)0x004E5091 = (DWORD)TSSGCtrl_EnumReadSSG - (0x004E5091 + sizeof(DWORD));
	*(LPDWORD)0x004E5095 = NOP_X4;

	// TSSGCtrl::MakeSubjectClass
	*(LPDWORD)(0x004EC409 + 1) = (DWORD)TSSGCtrl_MakeSubjectType - (0x004EC409 + 1 + sizeof(DWORD));

	// TSSGCtrl::ReadADJFile
	//   attributeSelector.IsElementCheckMode()
	*(LPBYTE )(0x004FD49A + 2) = offsetof(TSSGCtrl, attributeSelector.nowAttributeList);

	*(LPBYTE )(0x004FD549 + 1) = sizeof(TDirAttribute);
	*(LPWORD )(0x004FD594 + 0) = BSWAP16(0x8948);// mov [eax + ...], ecx
	*(LPBYTE )(0x004FD594 + 2) = offsetof(TDirAttribute, identity);
	*(LPBYTE )(0x004FD597 + 0) = 0x89;// mov [eax + ...]
	*(LPBYTE )(0x004FD597 + 1) = 0x48;// ecx
	*(LPBYTE )(0x004FD597 + 2) = offsetof(TDirAttribute, arbitrary);

	// TSSGCtrl::GetAttribute
	*(LPWORD )0x005038FE = BSWAP16(0x8BCB);// mov ecx, ebx
	*(LPWORD )0x00503900 = BSWAP16(0x5B5D);// pop ebx
	*(LPBYTE )0x00503902 = JMP_REL32      ;// pop ebp
	*(LPDWORD)0x00503903 = (DWORD)TSSGAttributeElement_GetViaCoord - (0x00503903 + sizeof(DWORD));

	// TSSGCtrl::AddressAttributeFilter
	//   keeping Address initial value.
	*(LPWORD )(0x00504133 + 1) = BSWAP16(0xEC48);// sub  esp, 0x48
	*(LPDWORD) 0x0050414C      = BSWAP16(0x8B0F);// mov  ecx, dword ptr [Address]
	*(LPWORD ) 0x0050414E      = BSWAP16(0x85C0);// test eax, eax 
	*(LPBYTE )(0x00504150 + 0) = JNZ_SHORT;
	*(LPBYTE )(0x00504150 + 1) = 0x00504161 - (0x00504150 + 1 + sizeof(BYTE));
	*(LPBYTE )(0x00504169 + 0) =         0x89   ;// mov dword ptr
	*(LPWORD )(0x00504169 + 1) = BSWAP16(0x4DB8);// [ebp - 0x48], ecx
	*(LPBYTE )(0x0050416C + 0) = JMP_REL32;
	*(LPDWORD)(0x0050416C + 1) = 0x00504266 - (0x0050416C + 1 + sizeof(DWORD));
	*(LPBYTE ) 0x00504171      = NOP;
	//   GetAddress(SSGS, tmpAE->GetOffsetCode(), 0)
	*(LPBYTE )(0x005041BB + 3) = 0x08;
	*(LPBYTE ) 0x005041C3      = PUSH_EBP;
	*(LPWORD )(0x005041C4 + 0) = BSWAP16(0xFF75);// push
	*(LPWORD )(0x005041C4 + 2) =         0xB8   ;// dword ptr [ebp - 0x48]
	*(LPWORD ) 0x005041C7      = PUSH_EBX;
	*(LPBYTE )(0x005041C8 + 0) = CALL_REL;
	*(LPDWORD)(0x005041C8 + 1) = (DWORD)TSSGCtrl_AddressAttributeFilter_GetAddress - (0x005041C8 + 1 + sizeof(DWORD));
	*(LPBYTE ) 0x005041CD      = PUSH_EDX;

	// TSSGCtrl::CheckFunnel
	*(LPDWORD)(0x00510169 + 1) = (DWORD)TSSGCtrl_CheckFunnel_GetAttribute - (0x00510169 + 1 + sizeof(DWORD));

	// TSSGCtrl::CheckIO_FEP
	*(LPDWORD)(0x00510F69 + 1) = (DWORD)TSSGCtrl_CheckIO_FEP_GetAttribute - (0x00510F69 + 1 + sizeof(DWORD));

	// THeapAdjustmentAttribute::~THeapAdjustmentAttribute
#ifdef SCOPE_USE_STRING_KEY
	*(LPDWORD)(0x00523531 + 1) = (DWORD)THeapAdjustmentAttribute_dtor_heapMap - (0x00523531 + 1 + sizeof(DWORD));
#endif

	// THeapAdjustmentAttribute::IsEqual
	//   if (type == atSCOPE) return TRUE;
	*(LPBYTE )0x005235CA = 0x0052363A - (0x005235CA + sizeof(BYTE));
	*(LPBYTE )0x005235CB = 0x81;// cmp dword ptr
	*(LPBYTE )0x005235CC = 0x7B;// [ebx + ... 
	*(LPBYTE )0x005235CD = offsetof(THeapAdjustmentAttribute, type);
	*(LPDWORD)0x005235CE = atSCOPE;
	*(LPBYTE )0x005235D3 = 0x0052363E - (0x005235D3 + sizeof(BYTE));

	// TChildRWAttribute::IsEqual
	//   Compare 'prohibit' as DWORD
	*(LPBYTE )0x005268D1 = 0x8B;// al => eax
	*(LPBYTE )0x005268D4 = 0x3B;// al => eax

	// TReplaceAttribute::IsEqual
	//   if (displace != X->displace) return FALSE;
	*(LPBYTE )0x00527250 = JNZ_SHORT;
	*(LPWORD )0x00527251 = 0x0052726A - (0x00527251 + sizeof(BYTE));
	*(LPWORD )0x00527252 = BSWAP16 (0x8B47);// mov eax, dword ptr [edi + ...
	*(LPBYTE )0x00527254 = offsetof(TReplaceAttribute, displace);
	*(LPBYTE )0x00527255 =          0x8B   ;// mov ecx, dword ptr [ebp +   
	*(LPWORD )0x00527256 = BSWAP16 (0x4D0C);//                           0x0C]
	*(LPWORD )0x00527258 = BSWAP16 (0x8B51);// mov edx, dword ptr [ecx + ...
	*(LPBYTE )0x0052725A = offsetof(TReplaceAttribute, displace);
	*(LPBYTE )0x0052725B =          0x3B   ;// cmp eax, edx  
	*(LPWORD )0x0052725C = BSWAP16 (0xC2 << 8 | JNZ_SHORT);

	// TDirAttribute::IsEqual
	//   if (identity != X->identity) return FALSE;
	*(LPBYTE )0x00527403 = JNZ_SHORT;
	*(LPBYTE )0x00527404 = 0x00527415 - (0x00527404 + sizeof(BYTE));
	*(LPBYTE )0x00527405 = 0x8B;// mov ecx, 
	*(LPBYTE )0x00527406 = 0x48;// [eax + ...
	*(LPBYTE )0x00527407 = offsetof(TDirAttribute, identity);
	*(LPWORD )0x00527408 = BSWAP16(0x3B4A);// cmp ecx, [edx + ..
	*(LPBYTE )0x0052740A = offsetof(TDirAttribute, identity);
	*(LPBYTE )0x0052740B = JNZ_SHORT;
}
