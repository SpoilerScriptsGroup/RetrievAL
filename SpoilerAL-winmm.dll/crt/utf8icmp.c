
int __cdecl _utf8icmp(const unsigned char *string1, const unsigned char *string2)
{
	for (; ; )
	{
		unsigned char c1, c2;

		c1 = *(string1++);
		c2 = *(string2++);
		if ((char)c1 >= 0)
		{
			if (!(c1 -= c2))
				if (c2)
					continue;
				else
					break;
			if (c1 == 'A' - 'a')
			{
				if ((char)c2 >= 'a' && c2 <= 'z')
					continue;
			}
			else if (c1 == 'a' - 'A')
			{
				if ((char)c2 >= 'A' && c2 <= 'Z')
					continue;
			}
			c1 += c2;
		}
		else if (c1 == c2)
		{
			if (c1 <= 0xDF)
				goto LENGTH2;
			if (c1 <= 0xEF)
				goto LENGTH3;
			c1 = *(string1++);
			c2 = *(string2++);
			if (c1 == c2)
			{
				if (!c1)
					break;
		LENGTH3:
				c1 = *(string1++);
				c2 = *(string2++);
				if (c1 == c2)
				{
					if (!c1)
						break;
		LENGTH2:
					c1 = *(string1++);
					c2 = *(string2++);
					if (c1 == c2)
						if (c1)
							continue;
						else
							break;
				}
			}
		}
		return (int)c1 - (int)c2;
	}
	return 0;
}
