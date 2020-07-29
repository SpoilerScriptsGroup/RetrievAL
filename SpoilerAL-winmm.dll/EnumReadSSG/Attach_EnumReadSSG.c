#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_operator.h"
#include "TSSGCtrl.h"

EXTERN_C BOOL ExtensionTSSDir;
EXTERN_C void * __cdecl TSSGCtrl_ReadSSG_PushElement(void *, void *);

static unsigned seqElement = MAXDWORD;

static __declspec(naked) uint64_t __cdecl TSSGAttributeSelector_StartElementCheck_new_attributeSetMap(size_t __n)
{
	__asm {
		xor  eax, eax
		mov  seqElement, eax
		mov  edx, dword ptr [esi + TSSGAttributeSelector.attributeSetMap]
		test edx, edx
		jnz  PASS

		mov  ecx, dword ptr [esp + 4]
#if OPTIMIZE_ALLOCATOR
		call bcb6_operator_new
#else
		push ecx
		call bcb6_operator_new
		pop  ecx
#endif
		xor  edx, edx

	PASS:
		ret
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
void __cdecl TSSGAttributeSelector_MakeNowAttributeVec_push_back(list *const allAtteributeVecList, vector *const *const __x)
{
	static void(__cdecl *const list_vector_push_back)(list *, const vector *const *) = (void *)0x004D5FBC;

	COORD coords[0x20] = { { 0, 0 } };
	vector const index = { coords, coords + _countof(coords) - 1, NULL, NULL, coords, MAXDWORD };

	vector *const NewVec = *__x;
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

	NewVec->allocator_type[0] = NULL;
	NewVec->allocator_type[1] = NULL;
	NewVec->tblIndex = SubjectStringTable_insert((string *)&index);
	list_vector_push_back(allAtteributeVecList, __x);
}

CONSOLE_FONT_INFO __fastcall TSSGAttributeElement_GetViaCoord(AtType const Type, const vector *const AttrV)
{
	DWORD Index;
	_BitScanForward(&Index, Type);
	COORD coord = vector_type_at(&vector_at(&SubjectStringTable_array, AttrV->tblIndex), COORD, Index);
	return (CONSOLE_FONT_INFO) { coord.X ? vector_type_at(AttrV, DWORD, coord.Y) : 0, coord };
}

static BOOL __fastcall TSSGCtrl_AddressAttributeFilter_GetAddress_replace(TReplaceAttribute *tmpAE, TSSGSubject **SSGS)
{
	if (tmpAE->context)
		*SSGS = tmpAE->context;
	return !!tmpAE->context;
}

static __declspec(naked) void __cdecl TSSGCtrl_AddressAttributeFilter_GetAddress()
{	
	__asm {
		lea  edx, [esp + 8]
		mov  ecx, [ebx]
		call TSSGCtrl_AddressAttributeFilter_GetAddress_replace
		test eax, eax
		jz   GET
		mov  ecx, [ebp - 0x48]
		mov  [edi], ecx// restore initial address
	GET:
		jmp  TSSGCtrl_GetAddress
		ud2
	}
}

static __declspec(naked) void __fastcall THeapAdjustmentAttribute_dtor_heapMap(THeapAdjustmentAttribute *this, struct _Rb_tree_node *root)
{
	static void(__cdecl *const tree_ulong_pair_uu_erase)() = (void *)0x004D0A80;
	static void(__cdecl *const tree_string_double_erase)() = (void *)0x0051C168;

	__asm {
		mov   eax, tree_ulong_pair_uu_erase
		cmp   dword ptr [ecx + TSSGAttributeElement.type], atSCOPE
		cmove eax, tree_string_double_erase
		jmp   eax
		ud2
	}
}

#define CALL_REL  (BYTE )0xE8
#define JMP_REL32 (BYTE )0xE9
#define JMP_REL8  (BYTE )0xEB
#define JZ_SHORT  (BYTE )0x74
#define JNZ_SHORT (BYTE )0x75
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
	*(LPWORD )0x004D3686 = BSWAP16(0x33C9    );// xor ecx, ecx  
	*(LPDWORD)0x004D3688 = BSWAP32(0x8B45042D);// mov eax, dword ptr [_AddressOfReturnAddress]
	*(LPDWORD)0x004D368C =         0x004D2D15 ;// sub eax, ...
	*(LPDWORD)0x004D3690 = BSWAP32(0xF7D81BD2);// neg eax; sbb edx, edx  
	*(LPDWORD)0x004D3694 = BSWAP32(0x8B432089);// mov eax, dword ptr [attributeSetMap] 
	*(LPDWORD)0x004D3698 = BSWAP32(0x4B288515);// mov dword ptr [nowAttributeVec], ecx  
	*(LPDWORD)0x004D369C = (DWORD)&ExtensionTSSDir;// test  edx, ds:
	*(LPBYTE )0x004D36A0 = JZ_SHORT;
	*(LPBYTE )0x004D36A1 = 0x004D36A5 - (0x004D36A1 + sizeof(BYTE));
	*(LPBYTE )0x004D36A2 = JMP_REL8;
	*(LPBYTE )0x004D36A3 = 0x004D3722 - (0x004D36A3 + sizeof(BYTE));
	*(LPBYTE )0x004D36A4 = NOP;

	// TSSGAttributeSelector::AddElement
	*(LPDWORD)0x004D3DCA = (DWORD)TSSGAttributeSelector_AddElement_MakeOnlyOneAtteribute - (0x004D3DCA + sizeof(DWORD));

	// TSSGAttributeSelector::MakeNowAttributeVec
	*(LPDWORD)0x004D5C88 = (DWORD)TSSGAttributeSelector_MakeNowAttributeVec_push_back - (0x004D5C88 + sizeof(DWORD));

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
	*(LPBYTE )(0x004E4617 + 2) = offsetof(TDirAttribute, ref);

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
	*(LPWORD )(0x004FD594 + 0) = BSWAP16(0x894A);// mov [edx + ...], ecx
	*(LPBYTE )(0x004FD594 + 2) = offsetof(TDirAttribute, identity);
	*(LPBYTE )(0x004FD597 + 0) = 0x89;// mov [edx + ...]
	*(LPBYTE )(0x004FD597 + 1) = 0x4A;// ecx
	*(LPBYTE )(0x004FD597 + 2) = offsetof(TDirAttribute, ref);
	*(LPBYTE )(0x004FD59A + 1) = 0x42;// edx => eax 

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
	*(LPBYTE ) 0x00504150      = JNZ_SHORT;
	*(LPBYTE )(0x00504150 + 1) = 0x00504161 - (0x00504150 + 1 + sizeof(BYTE));
	*(LPBYTE )(0x00504169 + 0) =         0x89   ;// mov dword ptr
	*(LPWORD )(0x00504169 + 1) = BSWAP16(0x4DB8);// [ebp - 0x48], ecx
	*(LPBYTE )(0x0050416C + 0) = JMP_REL32;
	*(LPDWORD)(0x0050416C + 1) = 0x00504266 - (0x0050416C + 1 + sizeof(DWORD));
	*(LPBYTE )(0x0050416C + 5) = NOP;
	//   GetAddress(SSGS, tmpAE->GetOffsetCode(), 0)
	*(LPDWORD)(0x005041CF + 1) = (DWORD)TSSGCtrl_AddressAttributeFilter_GetAddress - (0x005041CF + 1 + sizeof(DWORD));

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

	// TReplaceAttribute::IsEqual
	//   if (context != X->context) return FALSE;
	*(LPBYTE )0x00527250 = JNZ_SHORT;
	*(LPWORD )0x00527251 = 0x0052726A - (0x00527251 + sizeof(BYTE));
	*(LPWORD )0x00527252 = BSWAP16 (0x8B47);// mov eax, dword ptr [edi + ...
	*(LPBYTE )0x00527254 = offsetof(TReplaceAttribute, context);
	*(LPBYTE )0x00527255 =          0x8B   ;// mov ecx, dword ptr [ebp +   
	*(LPWORD )0x00527256 = BSWAP16 (0x4D0C);//                           0x0C]
	*(LPWORD )0x00527258 = BSWAP16 (0x8B51);// mov edx, dword ptr [ecx + ...
	*(LPBYTE )0x0052725A = offsetof(TReplaceAttribute, context);
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
