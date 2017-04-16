.486
.model flat

extrn _bcb6_std_string_append:dword

public @bcb6_std_string_resize@8

.code

align 16

;EXTERN_C void __msfastcall bcb6_std_string_resize(string *s, size_t n)
;{
;	char *end;
;
;	end = s->_M_start + n;
;	if (end > s->_M_finish)
;		bcb6_std_string_append(s, end - s->_M_finish, '\0');
;	else
;		*(s->_M_finish = end) = '\0';
;}
@bcb6_std_string_resize@8 proc near

	push    ebx
	mov     eax, dword ptr [ecx]
	mov     ebx, dword ptr [ecx + 4]
	add     edx, eax
	sub     edx, ebx
	jbe     L1
	push    0
	push    edx
	push    ecx
	call    dword ptr [_bcb6_std_string_append]
	mov     ebx, dword ptr [esp + 12]
	add     esp, 12 + 4
	ret
	align   16
L1:
	add     edx, ebx
	pop     ebx
	mov     byte ptr [edx], 0
	mov     dword ptr [ecx + 4], edx
	ret

@bcb6_std_string_resize@8 endp

end
