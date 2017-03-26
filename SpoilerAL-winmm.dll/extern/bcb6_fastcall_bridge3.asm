.486
.model flat

public @_bcb6_fastcall_bridge3@16

.code

align 16

;EXTERN_C DWORD __fastcall _bcb6_fastcall_bridge3(DWORD P1, DWORD P2, DWORD Function, DWORD P3);
;#define bcb6_fastcall_bridge3(Function, P1, P2, P3) \
;	_bcb6_fastcall_bridge3(P1, P2, Function, P3)
@_bcb6_fastcall_bridge3@16 proc near

	mov     eax, dword ptr [esp]
	mov     dword ptr [esp], ecx
	mov     ecx, dword ptr [esp + 8]
	mov     dword ptr [esp + 8], eax
	pop     eax
	ret

@_bcb6_fastcall_bridge3@16 endp

end
