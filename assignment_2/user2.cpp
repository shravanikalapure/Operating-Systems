#include <iostream>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstring>

using namespace std;

int main() {

    char msg[100];
    char reply[100];

    mkfifo("fifo1", 0666);
    mkfifo("fifo2", 0666);

    while(true) {

        int fd1 = open("fifo1", O_RDONLY);
        read(fd1, msg, sizeof(msg));

        cout << "User1: " << msg << endl;
        close(fd1);

        int fd2 = open("fifo2", O_WRONLY);

        cout << "User2: ";
        cin.getline(reply,100);

        write(fd2, reply, strlen(reply)+1);

        close(fd2);
    }

    return 0;
}