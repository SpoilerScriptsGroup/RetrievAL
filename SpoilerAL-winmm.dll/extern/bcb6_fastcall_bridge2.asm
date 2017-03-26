.486
.model flat

public @_bcb6_fastcall_bridge2@12

.code

align 16

;EXTERN_C DWORD __fastcall _bcb6_fastcall_bridge2(DWORD Function, DWORD P2, DWORD P1);
;#define bcb6_fastcall_bridge2(Function, P1, P2) \
;	_bcb6_fastcall_bridge2(Function, P2, P1)
@_bcb6_fastcall_bridge2@12 proc near

	mov     eax, ecx
	mov     ecx, dword ptr [esp]
	mov     dword ptr [esp], eax
	mov     eax, dword ptr [esp + 4]
	mov     dword ptr [esp + 4], ecx
	ret

@_bcb6_fastcall_bridge2@12 endp

end
