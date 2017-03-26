.486
.model flat

public @TDrawGrid_GetClientWidth@4

.code

align 16

;EXTERN_C int __msfastcall TDrawGrid_GetClientWidth(TDrawGrid *DrawGrid)
;{
;	return DrawGrid->Handle;
;}
@TDrawGrid_GetClientWidth@4 proc near

	mov     edx, 005806A0H
	mov     eax, ecx
	jmp     edx

@TDrawGrid_GetClientWidth@4 endp

end
