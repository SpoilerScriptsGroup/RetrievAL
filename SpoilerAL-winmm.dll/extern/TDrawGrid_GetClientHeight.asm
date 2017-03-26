.486
.model flat

public @TDrawGrid_GetClientHeight@4

.code

align 16

;EXTERN_C int __msfastcall TDrawGrid_GetClientHeight(TDrawGrid *DrawGrid)
;{
;	return DrawGrid->Handle;
;}
@TDrawGrid_GetClientHeight@4 proc near

	mov     edx, 005806E4H
	mov     eax, ecx
	jmp     edx

@TDrawGrid_GetClientHeight@4 endp

end
