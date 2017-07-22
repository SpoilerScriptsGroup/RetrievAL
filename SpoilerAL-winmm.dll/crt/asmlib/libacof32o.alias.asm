.386
.model flat, c

includelib libacof32o.lib

extern A_strcmp:proc
extern A_stricmp:proc

alias <strcmp> = <A_strcmp>
alias <stricmp> = <A_stricmp>

end
