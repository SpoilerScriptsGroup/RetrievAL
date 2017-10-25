public _f2xm1

.code

_f2xm1 proc
	fld     tbyte ptr [rdx]
	f2xm1
	fstp    tbyte ptr [rcx]
	ret
_f2xm1 endp

end
