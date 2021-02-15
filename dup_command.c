#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

int main(int argc, char** argv) 
{
	if (argc != 3)
	{
		printf("You entered an improper amount of inputs\n");
		printf("Proper usage: c_file_name linux_command text_file_name\n");
	}
	
	char * filename = argv[2];
	char * command = argv[1];
	int pid;
	pid = fork();
	if (pid == 0) 
	{ 
		//We are inside the child process
		int file_desc = open(filename, O_WRONLY | O_APPEND);
		if (file_desc < 0)
		{
			perror("Error: ");
			exit(-1);
		}
		int copy_desc = dup2(file_desc, 1);
		execl(command,filename,(char*)0);
	}
	else
	{
		wait(NULL);
	}
	return 0; 
} 
