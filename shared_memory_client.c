#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <sys/shm.h>
#include <errno.h>
#include <sys/ipc.h>

const int SHM_SIZE = 2048;
const char FILE_NAME[] = "shared_memory_input.dat";
//The .dat file is used to return a key when we use run the ftok function
//If we wanted to we could just set the key value to an integer value like "key_t key = 21;"

int main(int argc, char * argv[]) 
{
    char * filename = argv[1];
    if (argc != 2) 
    {
        printf("You entered an improper amount of inputs\n");
        printf("Proper usage: <program> <file_name>\n");
        exit(-1);
    }
    FILE * input = fopen(filename, "r");
    if (input == NULL) 
    {
        printf("File not found.");
        exit(-1);
    }
    const int LINE_SIZE = 121;
    char arr[LINE_SIZE];

    key_t key;
    int shmid;
    int * address;

    if ((key = ftok(FILE_NAME, 1)) == -1) 
    {
        perror("Error: ");
        exit(-1);
    }
    if ((shmid = shmget(key, SHM_SIZE, 0644 | IPC_CREAT)) == -1)
    //shmget return the identifier of the System V shared memory segement associated
    //our key value that we set earlier
    {
        perror("Error: ");
        exit(-1);
    }
    address = shmat(shmid, (void *)0, 0);
    if (address == (int *)(-1)) 
    {
        perror("Error: ");
        exit(-1);
    }
    *address = 0;
    char * msg = (char*) address + 4;
    memset(msg, 0, SHM_SIZE-4);
    //The memset command fills a block of memory with a particular value

    while(fgets(arr, LINE_SIZE, input) != NULL) 
    {
        for (int i = 0; i < strlen(arr); i++) 
        {
            arr[i] = toupper(arr[i]);
        }
        strncpy(msg, arr, SHM_SIZE);
        puts(msg);
        ++*address;
        sleep(1);
    }
    strncpy(msg, "STOP", 5);
    ++*address;
    puts("STOP");
    fclose(input);
    if (shmdt(address) == -1) 
    {
        perror("Error: ");
        exit(-1);
    }

    return 0;
}
