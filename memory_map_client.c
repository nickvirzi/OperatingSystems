//Need to run the client file before the server for memory_map scripts
//Otherwise you will get an error where the sharedspace cannot be accessed
//To fix this issue you will need to delete the sharespace file in the directory and rerun the code
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <string.h>
#include <sys/mman.h>
#include <ctype.h>

const int MEM_SIZE = 2048;

int main(int argc, char * argv[]) 
{
    char * filename = argv[1];
    if (argc != 2) 
    {
        printf("You entered an improper amount of inputs\n");
        printf("Proper usage: c_file_name linux_command text_file_name\n");
        exit(-1);
    }
    char input_file[MEM_SIZE];
    int file_descriptor_out;
    int * address;
    char * msg;
    FILE * input = fopen(filename, "r");
    if (input == NULL) 
    {
        perror("Error: ");
        exit(-1);
    }
    if ((file_descriptor_out = open("sharedspace", O_RDWR | O_CREAT | O_TRUNC, 0644)) < 0)
    {
        perror("Error: ");  
        exit(-1);  
    }
    ftruncate(file_descriptor_out, MEM_SIZE);
    //This functions causes the file to be truncated to a size of precisely 2048 bytes
    //If the file is larger than the 2048 bytes then the extra data will be lost
    if ((address = (int*) mmap(0, MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, file_descriptor_out, 0)) == MAP_FAILED) 
    {
        perror("Error: ");
	exit(-1);
    }
    msg = (char*) address + 4;
    while(fgets(input_file, MEM_SIZE, input) != NULL) 
    {
        for (int i = 0; i < strlen(input_file); i++) 
        {
            input_file[i] = toupper(input_file[i]);
        }
        strncpy(msg, input_file, MEM_SIZE);
        puts(msg);
        ++*address;
        sleep(1);
    }
    strncpy(msg, "STOP", 5);
    ++*address;
    puts("STOP");
}
