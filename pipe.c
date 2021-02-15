#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include<sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <linux/limits.h>
#include <errno.h>

int main(int argc, char** argv) 
{
	
	FILE *p1;
	FILE *p2;
	char path[PATH_MAX];	
	
	p1 = popen("find -name \\*.c -print ", "r");
	p2 = popen("tr a-z A-Z", "w");
	if (p1 == NULL || p2 == NULL)
	{
		perror("Error: ");
		exit(-1);
	}
	while (fgets(path, PATH_MAX, p1) != NULL)
	{
		fputs(path, p2);
	}
	pclose(p1);
	pclose(p2);	
	return 0; 
} 
