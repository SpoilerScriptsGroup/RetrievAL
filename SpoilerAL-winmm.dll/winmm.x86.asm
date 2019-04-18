.486
.model flat, c

public _exp_timeBeginPeriod
public _exp_timeEndPeriod
public _exp_timeGetDevCaps
public _exp_timeGetSystemTime
public _exp_timeKillEvent
public _exp_timeSetEvent

.code

_exp_timeBeginPeriod:
_exp_timeEndPeriod:
_exp_timeKillEvent proc near

	int     3
	mov     eax, -1
	ret     4

_exp_timeKillEvent endp

_exp_timeGetDevCaps:
_exp_timeGetSystemTime proc near

	int     3
	mov     eax, -1
	ret     8

_exp_timeGetSystemTime endp

_exp_timeSetEvent proc near

	int     3
	mov     eax, -1
	ret     20

_exp_timeSetEvent endp

end
