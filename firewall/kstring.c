#include "kstring.h"

int kstrcmp(char *str1, char *str2)
{
    while(*str1 && *str2 && *str1==*str2)
    {
        str1+=1;
        str2+=1;
    }
    return *str1 - *str2;
}

char *kstrcpy(char *dst, const char *src)
{
	char *_dst = dst;
	const char *_src = src;

	while(*src)
	{
		*_dst++ = *_src++;
	}
	return dst;
}