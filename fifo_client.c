#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char * argv[]) 
{
    char * filename = argv[1];
    if (argc != 2) 
    {
        printf("Expected one argument. Received %d arguments.\n", argc-1);
        printf("Proper usage: c_file_name linux_command text_file_name\n");
        exit(-1);
    }
    FILE * file = fopen(filename, "r");
    if (file == NULL) 
    {
        perror("Error: ");
        exit(-1);
    }
    const int LINE_SIZE = 121;
    char input_file[LINE_SIZE];
    char * fifo = "myfifo";
    int ret = access(fifo, F_OK);
    if (ret != 0) 
    {
        mkfifo(fifo, 0644);
    }
    int fd = open(fifo, O_WRONLY);
    while(fgets(input_file, LINE_SIZE, file) != NULL) 
    {
        for (int i = 0; i < strlen(input_file); i++) 
        {
            input_file[i] = toupper(input_file[i]);
        }
        write(fd, input_file, LINE_SIZE);
        sleep(1);
    }
    write(fd, "Stop\n", 6);
    fclose(file);
    close(fd);
    return(0);
}
