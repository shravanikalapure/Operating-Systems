// NAMED PIPE - ONE WAY (Sender)

#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
using namespace std;

int main() {
    int fd = open("myfifo", O_WRONLY);

    string text;
    cout << "Enter text: ";
    getline(cin, text);

    write(fd, text.c_str(), text.size() + 1);

    close(fd);
    return 0;
}