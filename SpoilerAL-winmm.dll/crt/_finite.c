
int __cdecl _finite(double x)
{
	return (*((unsigned short int *)&x + 3) & 0x7FF0) != 0x7FF0;
}
