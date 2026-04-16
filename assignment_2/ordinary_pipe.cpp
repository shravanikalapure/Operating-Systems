#include <iostream>
#include <unistd.h>
using namespace std;

int main()
{
    int fd[2]; //fd[0] for reading, fd[1] for writing
    if (pipe(fd) == -1) { //creating the pipe
        perror("pipe failed");
        return 1;
    }

    pid_t pid = fork(); //creating a child process
    if (pid < 0) {
        perror("fork failed");
        return 1;
    }

    if (pid > 0)  // CLIENT (Parent) program
    {
        close(fd[0]); //closing the read end of the pipe in the client process

        int n;
        cout << "Enter number of elements: ";
        cin >> n;
        if(n>=100){
            cout << "Please enter a number less than 100\n";
            return 1;
        }
        if(n <= 0){
            cout << "Please enter a positive number\n";
            return 1;
        }
        int arr[100];
        cout << "Enter elements:\n";
        for (int i = 0; i < n; i++)
            cin >> arr[i];

        write(fd[1], &n, sizeof(n)); //writing the size of the array to the pipe
        write(fd[1], arr, n * sizeof(int)); //writing the array elements to the pipe

        cout << "Client: Data sent\n";

        close(fd[1]); //closing the write end of the pipe in the client process
    }
    else  // SERVER (Child)  program
    {
        close(fd[1]); //closing the write end of the pipe in the server process

        int n;
        read(fd[0], &n, sizeof(n)); //reading the size of the array from the pipe
        if(n<=0 || n>=100){
            cout << "Invalid size received\n";
            return 1;
        }

        int arr[100];
        read(fd[0], arr, n * sizeof(int)); //reading the array elements from the pipe

        int sum = 0;
        for (int i = 0; i < n; i++)
            sum += arr[i];

        cout << "Server: Sum = " << sum << endl;

        close(fd[0]); //closing the read end of the pipe in the server process
    }

    return 0;
}