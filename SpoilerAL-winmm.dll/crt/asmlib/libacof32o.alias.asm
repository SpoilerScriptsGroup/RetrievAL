.386
.model flat, c

includelib libacof32o.lib

extern A_stricmp:proc

alias <stricmp> = <A_stricmp>

end
