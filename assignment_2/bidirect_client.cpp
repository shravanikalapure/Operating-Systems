#include <iostream>
#include <fcntl.h>
#include <unistd.h>

using namespace std;

int main()
{
    int fd1 = open("fifo1", O_WRONLY); //opening first named pipe for writing
    int fd2 = open("fifo2", O_RDONLY); //opening second named pipe for reading
    if (fd1 == -1 || fd2 == -1) {
    perror("open failed");
    return 1;
}

    int n;
    cout<<"Enter number of elements: ";
    cin>>n;
    if (n <= 0) {
    cout << "Invalid input\n";
    return 1;
    }
    if (n > 1000) {
    cout << "Too many elements\n";
    return 1;
}


    int arr[n];

    cout<<"Enter numbers:\n";
    for(int i=0;i<n;i++)
        cin>>arr[i];

    if (write(fd1, &n, sizeof(n)) <= 0) {
    perror("write failed");
    return 1;
}
    if (write(fd1, arr, n * sizeof(int)) <= 0) {
    perror("write array failed");
    return 1;
}

    int sum;
    if (read(fd2, &sum, sizeof(sum)) <= 0) {
    perror("read failed");
    return 1;
}

    cout<<"Client: Sum received = "<<sum<<endl;

    close(fd1);
    close(fd2);
}
