public name

.code

name proc
	fld     tbyte ptr [rdx]
	instruction
	fstp    tbyte ptr [rcx]
	ret
name endp

end
