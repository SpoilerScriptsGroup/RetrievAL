.386
.model flat, c

includelib libacof32o.lib

extern A_memcpy:proc
extern A_memmove:proc
extern A_memset:proc
extern A_memcmp:proc
;extern A_strcat:proc
extern A_strcpy:proc
extern A_strlen:proc
extern A_strcmp:proc
extern A_stricmp:proc
;extern A_strstr:proc
;extern A_strspn:proc
;extern A_strcspn:proc

alias <memcpy>   = <A_memcpy>
alias <memmove>  = <A_memmove>
alias <memset>   = <A_memset>
alias <memcmp>   = <A_memcmp>
;alias <strcat>   = <A_strcat>
alias <strcpy>   = <A_strcpy>
alias <strlen>   = <A_strlen>
alias <strcmp>   = <A_strcmp>
alias <stricmp>  = <A_stricmp>
alias <_stricmp> = <A_stricmp>
;alias <strstr>   = <A_strstr>
;alias <strspn>   = <A_strspn>
;alias <strcspn>  = <A_strcspn>

end
