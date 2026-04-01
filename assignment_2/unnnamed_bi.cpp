// UNNAMED PIPE - BI-DIRECTIONAL
// Text Filtering: Convert to UPPERCASE

#include <iostream>
#include <unistd.h>
#include <cstring>
#include <cctype>
using namespace std;

int main() {
    int p1[2], p2[2]; // p1: Parent->Child, p2: Child->Parent
    pipe(p1);
    pipe(p2);

    pid_t pid = fork();

    if (pid > 0) { // Parent
        close(p1[0]); // close read end
        close(p2[1]); // close write end

        string text;
        cout << "Enter text: ";
        getline(cin, text);

        write(p1[1], text.c_str(), text.size() + 1);

        char result[100];
        read(p2[0], result, sizeof(result));

        cout << "Filtered Text (UPPERCASE): " << result << endl;

        close(p1[1]);
        close(p2[0]);
    }
    else { // Child
        close(p1[1]);
        close(p2[0]);

        char buffer[100];
        read(p1[0], buffer, sizeof(buffer));

        // FILTER: convert to uppercase
        for (int i = 0; buffer[i]; i++) {
            buffer[i] = toupper(buffer[i]);
        }

        write(p2[1], buffer, strlen(buffer) + 1);

        close(p1[0]);
        close(p2[1]);
    }

    return 0;
}