public _fsub

.code

_fsub proc
	fld     tbyte ptr [rdx]
	fld     tbyte ptr [r8]
	fsub
	fstp    tbyte ptr [rcx]
	ret
_fsub endp

end
