#include <string.h>

// function to trim newline character from the key and value for clean output
void trim_newline(char *str)
{
	char* pos;
        if((pos=strchr(str,'\n'))!=NULL)
		*pos='\0';
}
