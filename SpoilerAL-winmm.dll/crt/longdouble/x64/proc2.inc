public name

.code

name proc
	fld     tbyte ptr [rdx]
	fld     tbyte ptr [r8]
	instruction
	fstp    tbyte ptr [rcx]
	ret
name endp

end
