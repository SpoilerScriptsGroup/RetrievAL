.386
.model flat, c

includelib libacof32o.lib

extern A_stricmp:proc

alias <_stricmp> = <A_stricmp>
alias <stricmp> = <A_stricmp>

end
