#define MSW(value) \
	*((unsigned long int *)&(value) + 1)

int __cdecl signbit(double x)
{
	return (long int)MSW(x) < 0;
}
