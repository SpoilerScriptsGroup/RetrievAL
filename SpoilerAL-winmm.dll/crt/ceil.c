
double __cdecl ceil(double x)
{
	double val;
	return modf(x, &val) > 0 ? val + 1.0 : val;
}
