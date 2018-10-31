.386
.model flat, c

extrn _strtoui64:proc
extrn _finite:proc
extrn _isnan:proc

alias <strtoull> = <_strtoui64>
alias <isfinite> = <_finite>
alias <isnan> = <_isnan>

end
