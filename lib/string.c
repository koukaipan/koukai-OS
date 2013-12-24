#include "lib/string.h"

void strcpy(char *s, char *t)
{
	while((*s = *t) != '\0')
	{
		s++;
		t++;
	}
}
