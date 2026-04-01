// NAMED PIPE - ONE WAY (Receiver)
// Command before running: mkfifo myfifo

#include <iostream>
#include <fcntl.h>
#include <unistd.h>
using namespace std;

int main() {
    int fd = open("myfifo", O_RDONLY);

    char buffer[100];
    read(fd, buffer, sizeof(buffer));

    // FILTER: Remove vowels
    cout << "Filtered Text (no vowels): ";
    for (int i = 0; buffer[i]; i++) {
        char ch = tolower(buffer[i]);
        if (!(ch=='a'||ch=='e'||ch=='i'||ch=='o'||ch=='u')) {
            cout << buffer[i];
        }
    }
    cout << endl;

    close(fd);
    return 0;
}