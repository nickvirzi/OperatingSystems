//Note: The memory map uses the virutal memory system of our operating system to read and write files
//This can be useful to quicky move data back and forth between disk and RAM
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <string.h>
#include <sys/mman.h>

const int MEM_SIZE = 2048;

int main(int argc, char * argv[]) 
{
    char input_file[MEM_SIZE];
    int file_descriptor_in;
    int * address;
    char * msg;

    if ((file_descriptor_in = open("sharedspace", O_RDWR | O_CREAT)) < 0)
    {
        perror("Error: ");
	exit(-1);
    }
    ftruncate(file_descriptor_in, MEM_SIZE);
    if ((address = (int*) mmap(0, MEM_SIZE, PROT_READ, MAP_SHARED, file_descriptor_in, 0)) == MAP_FAILED)
    {
        perror("Error: ");
	exit(-1);
    }
    msg = (char*) address + 4;
    int n = -1;
    while (1) 
    {
        if (n == address[0]) 
        {
            continue;
        }
        n = address[0];
        if (strcmp(msg, "STOP") == 0)
	{
            break;
	}
        printf("%s", msg);
    }
    unlink("sharedspace");
}
