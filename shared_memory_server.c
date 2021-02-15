#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <string.h>
#include <sys/shm.h>
#include <errno.h>
#include <sys/ipc.h>

const int SHM_SIZE = 2048;
const char FILE_NAME[] = "shared_memory_input.dat";

int main(int argc, char * argv[]) 
{
    key_t key;
    int shmid;
    int * address;

    if ((key = ftok(FILE_NAME, 1)) == -1) 
    {
        perror("Error: ");
        exit(-1);
    }
    if ((shmid = shmget(key, SHM_SIZE, 0644 | IPC_CREAT)) == -1) 
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
    char * msg = (char*) address + 4;
    memset(msg, 0, SHM_SIZE-4);
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
    if (shmdt(address) == -1) 
    {
        perror("Error: ");
        exit(-1);
    }

    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}
