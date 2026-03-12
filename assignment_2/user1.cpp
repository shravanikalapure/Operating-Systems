#include <iostream>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstring>

using namespace std;

int main() {

    char msg[100];
    char reply[100];

    // Create named pipes
    mkfifo("fifo1", 0666);
    mkfifo("fifo2", 0666);

    while(true) {

        int fd1 = open("fifo1", O_WRONLY);

        cout << "User1: ";
        cin.getline(msg,100);

        write(fd1, msg, strlen(msg)+1);
        close(fd1);

        int fd2 = open("fifo2", O_RDONLY);
        read(fd2, reply, sizeof(reply));

        cout << "User2: " << reply << endl;

        close(fd2);
    }

    return 0;
}