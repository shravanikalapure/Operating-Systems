// NAMED PIPE - BI-DIRECTIONAL (Process 2)

#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <cctype>
using namespace std;

int main() {
    int fd1 = open("fifo1", O_RDONLY); // receive
    int fd2 = open("fifo2", O_WRONLY); // send

    char buffer[100];
    read(fd1, buffer, sizeof(buffer));

    // FILTER: Convert to lowercase
    for (int i = 0; buffer[i]; i++) {
        buffer[i] = tolower(buffer[i]);
    }

    write(fd2, buffer, strlen(buffer) + 1);

    close(fd1);
    close(fd2);
    return 0;
}