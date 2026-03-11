#include <iostream>
using namespace std;

// Maximum number of processes allowed
#define MAX 10

// Structure to store process information
struct Process
{
    int pid;   // Process ID
    int at;    // Arrival Time
    int bt;    // Burst Time
    int rt;    // Remaining Time (used in preemptive algorithms)
    int ct;    // Completion Time
    int wt;    // Waiting Time
    int tat;   // Turnaround Time
    int pr;    // Priority
};

// Function declarations
void printTable(Process p[], int n);
void printGantt(int order[], int size);

void FCFS(Process p[],int n);
void SJF_NP(Process p[],int n);
void SJF_P(Process p[],int n);
void RoundRobin(Process p[],int n,int tq);
void Priority_NP(Process p[],int n);
void Priority_P(Process p[],int n);

int main()
{
    int n,choice,tq;

    // Array to store processes
    Process p[MAX];

    cout<<"Enter number of processes: ";
    cin>>n;

    // Taking input for each process
    for(int i=0;i<n;i++)
    {
        p[i].pid=i+1; // assigning process ID automatically

        cout<<"Arrival Time P"<<i+1<<": ";
        cin>>p[i].at;

        cout<<"Burst Time P"<<i+1<<": ";
        cin>>p[i].bt;

        cout<<"Priority P"<<i+1<<": ";
        cin>>p[i].pr;

        // Remaining time initially equals burst time
        p[i].rt=p[i].bt;
    }

    // Menu driven scheduling program
    do
    {
        cout<<"\n----- CPU Scheduling Menu -----\n";
        cout<<"1. FCFS\n";
        cout<<"2. SJF Non Preemptive\n";
        cout<<"3. SJF Preemptive\n";
        cout<<"4. Round Robin\n";
        cout<<"5. Priority Non Preemptive\n";
        cout<<"6. Priority Preemptive\n";
        cout<<"0. Exit\n";

        cout<<"Enter choice: ";
        cin>>choice;

        // Copy original processes to temporary array
        // so that each algorithm runs independently
        Process temp[MAX];

        for(int i=0;i<n;i++)
        temp[i]=p[i];

        switch(choice)
        {
            case 1:
                FCFS(temp,n);
                break;

            case 2:
                SJF_NP(temp,n);
                break;

            case 3:
                SJF_P(temp,n);
                break;

            case 4:
                cout<<"Enter Time Quantum: ";
                cin>>tq;
                RoundRobin(temp,n,tq);
                break;

            case 5:
                Priority_NP(temp,n);
                break;

            case 6:
                Priority_P(temp,n);
                break;
        }

    }while(choice!=0);

    return 0;
}

// FCFS SCHEDULING

void FCFS(Process p[],int n)
{
    int time=0;         // CPU clock
    int order[MAX];     // to store execution order for Gantt chart
    int k=0;

    cout<<"\nFCFS Scheduling\n";

    // Processes executed in order of arrival
    for(int i=0;i<n;i++)
    {
        // If CPU is idle wait for process arrival
        if(time<p[i].at)
        time=p[i].at;

        // Execute process
        time+=p[i].bt;

        // Calculate completion time
        p[i].ct=time;

        // Turnaround Time = CT - AT
        p[i].tat=p[i].ct-p[i].at;

        // Waiting Time = TAT - BT
        p[i].wt=p[i].tat-p[i].bt;

        // Store process execution order
        order[k++]=p[i].pid;
    }

    cout<<"\nGantt Chart:\n";
    printGantt(order,k);

    printTable(p,n);
}

////////////////////////////////////////////////////////
// SJF NON PREEMPTIVE
////////////////////////////////////////////////////////

void SJF_NP(Process p[],int n)
{
    int time=0,completed=0;

    bool done[MAX]={false}; // track completed processes

    int order[MAX],k=0;

    cout<<"\nSJF Non Preemptive\n";

    // Run until all processes are completed
    while(completed<n)
    {
        int idx=-1;
        int min=9999;

        // Find process with minimum burst time
        for(int i=0;i<n;i++)
        {
            if(!done[i] && p[i].at<=time && p[i].bt<min)
            {
                min=p[i].bt;
                idx=i;
            }
        }

        // If process found
        if(idx!=-1)
        {
            time+=p[idx].bt;

            p[idx].ct=time;
            p[idx].tat=p[idx].ct-p[idx].at;
            p[idx].wt=p[idx].tat-p[idx].bt;

            done[idx]=true;
            completed++;

            order[k++]=p[idx].pid;
        }
        else
        {
            // CPU idle
            time++;
        }
    }

    cout<<"\nGantt Chart:\n";
    printGantt(order,k);

    printTable(p,n);
}

////////////////////////////////////////////////////////
// SJF PREEMPTIVE (Shortest Remaining Time First)
////////////////////////////////////////////////////////

void SJF_P(Process p[],int n)
{
    int time=0,completed=0;

    int order[100],k=0;

    cout<<"\nSJF Preemptive (SRTF)\n";

    while(completed<n)
    {
        int idx=-1;
        int min=9999;

        // Find process with smallest remaining time
        for(int i=0;i<n;i++)
        {
            if(p[i].at<=time && p[i].rt>0 && p[i].rt<min)
            {
                min=p[i].rt;
                idx=i;
            }
        }

        if(idx!=-1)
        {
            // Execute process for 1 unit time
            p[idx].rt--;

            order[k++]=p[idx].pid;

            time++;

            // If process completed
            if(p[idx].rt==0)
            {
                p[idx].ct=time;
                p[idx].tat=p[idx].ct-p[idx].at;
                p[idx].wt=p[idx].tat-p[idx].bt;

                completed++;
            }
        }
        else
        {
            time++;
        }
    }

    cout<<"\nGantt Chart:\n";
    printGantt(order,k);

    printTable(p,n);
}

////////////////////////////////////////////////////////
// ROUND ROBIN
////////////////////////////////////////////////////////

void RoundRobin(Process p[],int n,int tq)
{
    int time=0,completed=0;

    int order[100],k=0;

    cout<<"\nRound Robin Scheduling\n";

    while(completed<n)
    {
        bool executed=false;

        for(int i=0;i<n;i++)
        {
            if(p[i].at<=time && p[i].rt>0)
            {
                executed=true;

                // If remaining time greater than time quantum
                if(p[i].rt>tq)
                {
                    time+=tq;
                    p[i].rt-=tq;

                    order[k++]=p[i].pid;
                }
                else
                {
                    // Process finishes
                    time+=p[i].rt;

                    p[i].rt=0;

                    p[i].ct=time;
                    p[i].tat=p[i].ct-p[i].at;
                    p[i].wt=p[i].tat-p[i].bt;

                    completed++;

                    order[k++]=p[i].pid;
                }
            }
        }

        if(!executed)
        time++;
    }

    cout<<"\nGantt Chart:\n";
    printGantt(order,k);

    printTable(p,n);
}

////////////////////////////////////////////////////////
// PRIORITY NON PREEMPTIVE
////////////////////////////////////////////////////////

void Priority_NP(Process p[],int n)
{
    int time=0,completed=0;

    bool done[MAX]={false};

    int order[MAX],k=0;

    cout<<"\nPriority Non Preemptive\n";

    while(completed<n)
    {
        int idx=-1;
        int high=9999;

        // Find process with highest priority
        // (smaller number = higher priority)
        for(int i=0;i<n;i++)
        {
            if(!done[i] && p[i].at<=time && p[i].pr<high)
            {
                high=p[i].pr;
                idx=i;
            }
        }

        if(idx!=-1)
        {
            time+=p[idx].bt;

            p[idx].ct=time;
            p[idx].tat=p[idx].ct-p[idx].at;
            p[idx].wt=p[idx].tat-p[idx].bt;

            done[idx]=true;
            completed++;

            order[k++]=p[idx].pid;
        }
        else
        time++;
    }

    cout<<"\nGantt Chart:\n";
    printGantt(order,k);

    printTable(p,n);
}

////////////////////////////////////////////////////////
// PRIORITY PREEMPTIVE
////////////////////////////////////////////////////////

void Priority_P(Process p[],int n)
{
    int time=0,completed=0;

    int order[100],k=0;

    cout<<"\nPriority Preemptive\n";

    while(completed<n)
    {
        int idx=-1;
        int high=9999;

        for(int i=0;i<n;i++)
        {
            if(p[i].at<=time && p[i].rt>0 && p[i].pr<high)
            {
                high=p[i].pr;
                idx=i;
            }
        }

        if(idx!=-1)
        {
            p[idx].rt--;

            order[k++]=p[idx].pid;

            time++;

            if(p[idx].rt==0)
            {
                p[idx].ct=time;
                p[idx].tat=p[idx].ct-p[idx].at;
                p[idx].wt=p[idx].tat-p[idx].bt;

                completed++;
            }
        }
        else
        time++;
    }

    cout<<"\nGantt Chart:\n";
    printGantt(order,k);

    printTable(p,n);
}

////////////////////////////////////////////////////////
// PRINT PROCESS TABLE
////////////////////////////////////////////////////////

void printTable(Process p[], int n)
{
    float totalWT=0,totalTAT=0;

    cout<<"\nPID AT BT PR CT WT TAT\n";

    for(int i=0;i<n;i++)
    {
        cout<<p[i].pid<<"\t"
            <<p[i].at<<"\t"
            <<p[i].bt<<"\t"
            <<p[i].pr<<"\t"
            <<p[i].ct<<"\t"
            <<p[i].wt<<"\t"
            <<p[i].tat<<endl;

        totalWT+=p[i].wt;
        totalTAT+=p[i].tat;
    }

    cout<<"\nAverage WT = "<<totalWT/n;
    cout<<"\nAverage TAT = "<<totalTAT/n<<endl;
}

////////////////////////////////////////////////////////
// PRINT SIMPLE GANTT CHART
////////////////////////////////////////////////////////

void printGantt(int order[], int time[], int size)
{
    cout<<"\nGantt Chart\n";

    // Print timeline
    for(int i=0;i<=size;i++)
    cout<<time[i]<<"\t";

    cout<<endl;

    // Print process blocks
    for(int i=0;i<size;i++)
    cout<<"|P"<<order[i]<<"|";

    cout<<endl;
}