#include "geo_table.h"

#include <linux/string.h>

int GeoHash(char *contry)
{
	int len = sizeof(geo)/sizeof(geo_table);
	int i;

	for(i=0; i<len; i++)
		if(strcmp(geo[i].contry, contry) == 0)
			return geo[i].code;

	return -1;
}