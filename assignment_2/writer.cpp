#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main()
{
    int fd;

    fd = open("textpipe", O_WRONLY);

    char text[] =
    "system started\n"
    "error in module\n"
    "process running\n"
    "error detected\n"
    "task completed\n";

    write(fd, text, sizeof(text));

    close(fd);

    printf("Text sent to pipe\n");

    return 0;
}