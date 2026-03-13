#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main()
{
    int p1[2],p2[2];

    pipe(p1); // Parent → Child
    pipe(p2); // Child → Parent

    pid_t pid = fork();

    if(pid>0) // Parent
    {
        close(p1[0]);
        close(p2[1]);

        char text[] =
        "system started\n"
        "error in module\n"
        "process running\n"
        "error detected\n";

        write(p1[1],text,sizeof(text));

        char result[200];
        read(p2[0],result,sizeof(result));

        printf("Filtered text from child:\n%s",result);

        close(p1[1]);
        close(p2[0]);

        wait(NULL);
    }

    else // Child
    {
        close(p1[1]);
        close(p2[0]);

        char buffer[200];
        read(p1[0],buffer,sizeof(buffer));

        char result[200]="";

        char *line=strtok(buffer,"\n");

        while(line!=NULL)
        {
            if(strstr(line,"error"))
            {
                strcat(result,line);
                strcat(result,"\n");
            }

            line=strtok(NULL,"\n");
        }

        write(p2[1],result,sizeof(result));

        close(p1[0]);
        close(p2[1]);
    }

    return 0;
}