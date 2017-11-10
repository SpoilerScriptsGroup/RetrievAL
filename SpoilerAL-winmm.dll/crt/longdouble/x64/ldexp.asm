public __ldexp

.code

__ldexp proc
	mov     dword ptr [rsp + 8], r8d
	fild    dword ptr [rsp + 8]
	fld     tbyte ptr [rdx]
	fscale
	fstp    tbyte ptr [rcx]
	fstp    st(0)
	ret
__ldexp endp

end
