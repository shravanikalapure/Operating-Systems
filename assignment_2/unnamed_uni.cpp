// UNNAMED PIPE - ONE WAY (Parent -> Child)
// Text Filtering: Remove digits from input string

#include <iostream>
#include <unistd.h>
#include <cstring>
using namespace std;

int main() {
    int fd[2]; // pipe file descriptors
    pipe(fd);

    pid_t pid = fork();

    if (pid > 0) { // Parent Process
        close(fd[0]); // Close read end

        string text;
        cout << "Enter a string: ";
        getline(cin, text);

        write(fd[1], text.c_str(), text.size() + 1); // send text
        close(fd[1]);
    }
    else { // Child Process
        close(fd[1]); // Close write end

        char buffer[100];
        read(fd[0], buffer, sizeof(buffer)); // receive text

        // FILTER: remove digits
        cout << "Filtered Text (no digits): ";
        for (int i = 0; buffer[i] != '\0'; i++) {
            if (!isdigit(buffer[i])) {
                cout << buffer[i];
            }
        }
        cout << endl;

        close(fd[0]);
    }
    return 0;
}