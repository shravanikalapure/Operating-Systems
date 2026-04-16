#include <iostream>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

using namespace std;

int main()
{
   if (mkfifo("fifo1", 0666) == -1) { //creating first named pipe
    perror("fifo1");
}
    if (mkfifo("fifo2", 0666) == -1) { //creating second named pipe for bidirectional communication
    perror("fifo2");
}

    int fd1 = open("fifo1", O_RDONLY); //opening first named pipe for reading
    int fd2 = open("fifo2", O_WRONLY); //opening second named pipe for writing
    if (fd1 == -1 || fd2 == -1) {
    perror("open failed");
    return 1;
}

    int n;
    if (read(fd1, &n, sizeof(n)) <= 0) {
    perror("read failed");
    return 1;
}
if (n <= 0 || n > 1000) {
    cout << "Invalid size received\n";
    return 1;
}

    int arr[n];
   if (read(fd1, arr, n * sizeof(int)) <= 0) {
    perror("read array failed");
    return 1;
}

    cout << "Server: Received numbers\n";

    int sum=0;
    for(int i=0;i<n;i++)
        sum+=arr[i];

    cout << "Server: Sum calculated = " << sum << endl;

   if (write(fd2, &sum, sizeof(sum)) <= 0) {
    perror("write failed");
    return 1;
}

    cout << "Server: Sent sum back to client\n";

    close(fd1);
    close(fd2);
}
