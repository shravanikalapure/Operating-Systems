#include <iostream>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

using namespace std;

int main()
{
    if (mkfifo("myfifo", 0666) == -1) { //creating named pipe
    perror("mkfifo");
}

    int fd = open("myfifo", O_RDONLY); //opening named pipe for reading
    if (fd == -1) {
    perror("open failed");
    return 1;
}

    int n;
    if (read(fd, &n, sizeof(n)) <= 0) { //reading size of array , fd is file descriptor for named pipe
    perror("read failed");
    return 1;
}
if (n <= 0 || n > 1000) {
    cout << "Invalid size received\n";
    return 1;
}

    int arr[n];
    if (read(fd, arr, n * sizeof(int)) <= 0) { //reading array elements
    perror("read array failed");
    return 1;
}

    int sum = 0;
    for(int i=0;i<n;i++)
        sum += arr[i];

    cout << "Server: Sum = " << sum << endl;

    close(fd); //closing the named pipe
}