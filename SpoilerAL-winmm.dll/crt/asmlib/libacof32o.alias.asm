.386
.model flat, c

includelib libacof32o.lib

extrn A_memcpy:proc
extrn A_memmove:proc
extrn A_memset:proc
extrn A_memcmp:proc
;extrn A_strcat:proc
extrn A_strcpy:proc
extrn A_strlen:proc
;extrn A_strcmp:proc
extrn A_stricmp:proc
extrn A_strstr:proc
;extrn A_strspn:proc
;extrn A_strcspn:proc

alias <memcpy>   = <A_memcpy>
alias <memmove>  = <A_memmove>
alias <memset>   = <A_memset>
alias <memcmp>   = <A_memcmp>
;alias <strcat>   = <A_strcat>
alias <strcpy>   = <A_strcpy>
alias <strlen>   = <A_strlen>
;alias <strcmp>   = <A_strcmp>
alias <stricmp>  = <A_stricmp>
alias <_stricmp> = <A_stricmp>
alias <strstr>   = <A_strstr>
;alias <strspn>   = <A_strspn>
;alias <strcspn>  = <A_strcspn>

end
