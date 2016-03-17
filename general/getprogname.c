#include <stdio.h>
#include <string.h>

char *getprogname(void)
{
	char pname[256];

	FILE *f = fopen("/proc/self/cmdline" , "r");
	if (!f)
		return NULL;
	fgets(pname, 256, f);
	fclose(f);

	char *ptr = strchr(&pname[0], ' ');
	if (ptr)
		*ptr = '\0';
	
	return strdup(pname);
}
