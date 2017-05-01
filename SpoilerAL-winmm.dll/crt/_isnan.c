
int __cdecl _isnan(double x)
{
	float f = (float)x;
	return
		((*((long *)&f) & 0x7f800000) == 0x7f800000 &&
		(*((long *)&f) & 0x007fffff) != 0);
}
