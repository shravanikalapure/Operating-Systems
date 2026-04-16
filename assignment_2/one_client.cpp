#include <iostream>
#include <fcntl.h>
#include <unistd.h>

using namespace std;

int main()
{
    int fd = open("myfifo", O_WRONLY); //opeming named pipe for writing
    if (fd == -1) {
    perror("open failed");
    return 1;
}
    int n;
    cout << "Enter number of elements: ";
    cin >> n;
    if (n <= 0) {
    cout << "Invalid input\n";
    return 1;
    }
    if (n > 1000) {
    cout << "Too many elements\n";
    return 1;
}


    int arr[n];

    cout << "Enter numbers:\n";
    for(int i=0;i<n;i++)
        cin >> arr[i];

    if (write(fd, &n, sizeof(n)) <= 0) {
    perror("write failed");
    return 1;
}
    if (write(fd, arr, n * sizeof(int)) <= 0) {
    perror("write array failed");
    return 1;
}

    close(fd);
}
