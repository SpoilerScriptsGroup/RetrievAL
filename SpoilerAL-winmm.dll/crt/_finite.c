#define MSW(value) \
	*((unsigned long int *)&(value) + 1)

int __cdecl _finite(double x)
{
	return (MSW(x) & 0x7FF00000) != 0x7FF00000;
}
