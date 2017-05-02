
int __cdecl _isnan(double x)
{
	unsigned short int w;

	w = *((unsigned short int *)&x + 3) & 0x7FF8;
	return
		w == 0x7FF0 ?
			(*((unsigned long int *)&x + 1) & 0x0007FFFF) || *(unsigned long int *)&x :
			w == 0x7FF8;
}
