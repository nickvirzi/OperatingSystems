#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <string.h>

int main(int argc, char * argv[]) 
{
    const int LINE_SIZE = 121;
    char input_file[LINE_SIZE];
    char * fifo = "myfifo"; 
    int ret = access(fifo, F_OK);
    if (ret != 0) 
    {
        mkfifo(fifo, 0644);
    }
    int fd = open(fifo, O_RDONLY);
    ret = access(fifo, F_OK);
    if (ret != 0)
    {
        mkfifo(fifo, 0644);
    }
    fd = open(fifo, O_RDONLY);
    while(1) 
    {
        read(fd, input_file, LINE_SIZE);
        puts(input_file);
        if (strcmp(input_file, "Stop\n") == 0)
        {
            break;
        }
    }
    unlink(fifo);
    close(fd);
    return 0;
}
