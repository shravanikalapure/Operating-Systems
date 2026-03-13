#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main()
{
    int fd;
    char buffer[200];

    fd = open("textpipe", O_RDONLY);

    read(fd, buffer, sizeof(buffer));

    printf("Filtered lines containing 'error':\n");

    char *line = strtok(buffer, "\n");

    while(line != NULL)
    {
        if(strstr(line, "error"))
        {
            printf("%s\n", line);
        }

        line = strtok(NULL, "\n");
    }

    close(fd);

    return 0;
}