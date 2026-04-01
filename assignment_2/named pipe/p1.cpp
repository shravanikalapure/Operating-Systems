// NAMED PIPE - BI-DIRECTIONAL (Process 1)

#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
using namespace std;

int main() {
    int fd1 = open("fifo1", O_WRONLY); // send
    int fd2 = open("fifo2", O_RDONLY); // receive

    string text;
    cout << "Enter text: ";
    getline(cin, text);

    write(fd1, text.c_str(), text.size() + 1);

    char buffer[100];
    read(fd2, buffer, sizeof(buffer));

    cout << "Filtered Text: " << buffer << endl;

    close(fd1);
    close(fd2);
    return 0;
}