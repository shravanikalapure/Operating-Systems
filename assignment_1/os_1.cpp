#include <iostream>
using namespace std;

#define MAX 10   // maximum number of processes

//------------------------------------------------------
// Structure to store process information
//------------------------------------------------------
struct Process
{
    int pid;    // Process ID
    int at;     // Arrival Time
    int bt;     // Burst Time
    int rt;     // Remaining Time (used in preemptive scheduling)
    int ct;     // Completion Time
    int wt;     // Waiting Time
    int tat;    // Turnaround Time
    int pr;     // Priority
};

//------------------------------------------------------
// Function declarations
//------------------------------------------------------
void printTable(Process p[], int n);          // prints final process table
void printGantt(int order[], int timeline[], int size); // prints gantt chart

void FCFS(Process p[], int n);
void SJF_NP(Process p[], int n);
void SJF_P(Process p[], int n);
void RoundRobin(Process p[], int n, int tq);
void Priority_NP(Process p[], int n);
void Priority_P(Process p[], int n);

//------------------------------------------------------
// MAIN FUNCTION
//------------------------------------------------------
int main()
{
    int n, choice, tq;
    Process p[MAX];

    cout << "Enter number of processes: ";
    cin >> n;

    // Input process details
    for (int i = 0; i < n; i++)
    {
        p[i].pid = i + 1;

        cout << "Arrival Time P" << i + 1 << ": ";
        cin >> p[i].at;

        cout << "Burst Time P" << i + 1 << ": ";
        cin >> p[i].bt;

        cout << "Priority P" << i + 1 << ": ";
        cin >> p[i].pr;

        p[i].rt = p[i].bt;  // remaining time initially = burst time
    }

    // Menu driven program
    do
    {
        cout << "\n----- CPU Scheduling Menu -----\n";
        cout << "1. FCFS\n";
        cout << "2. SJF Non Preemptive\n";
        cout << "3. SJF Preemptive\n";
        cout << "4. Round Robin\n";
        cout << "5. Priority Non Preemptive\n";
        cout << "6. Priority Preemptive\n";
        cout << "0. Exit\n";

        cout << "Enter choice: ";
        cin >> choice;

        // Copy original processes so each algorithm runs fresh
        Process temp[MAX];
        for (int i = 0; i < n; i++)
            temp[i] = p[i];

        switch (choice)
        {
        case 1:
            FCFS(temp, n);
            break;

        case 2:
            SJF_NP(temp, n);
            break;

        case 3:
            SJF_P(temp, n);
            break;

        case 4:
            cout << "Enter Time Quantum: ";
            cin >> tq;
            RoundRobin(temp, n, tq);
            break;

        case 5:
            Priority_NP(temp, n);
            break;

        case 6:
            Priority_P(temp, n);
            break;
        }

    } while (choice != 0);

    return 0;
}

//------------------------------------------------------
// FUNCTION TO PRINT PROCESS TABLE
//------------------------------------------------------
void printTable(Process p[], int n)
{
    float totalWT = 0, totalTAT = 0;

    cout << "\nPID\tAT\tBT\tPR\tCT\tWT\tTAT\n";

    for (int i = 0; i < n; i++)
    {
        cout << p[i].pid << "\t"
             << p[i].at << "\t"
             << p[i].bt << "\t"
             << p[i].pr << "\t"
             << p[i].ct << "\t"
             << p[i].wt << "\t"
             << p[i].tat << endl;

        totalWT += p[i].wt;
        totalTAT += p[i].tat;
    }

    cout << "\nAverage Waiting Time = " << totalWT / n;
    cout << "\nAverage Turnaround Time = " << totalTAT / n << endl;
}

//------------------------------------------------------
// FUNCTION TO PRINT GANTT CHART
//------------------------------------------------------
void printGantt(int order[], int timeline[], int size)
{
    cout << "\nGantt Chart\n";

    // print timeline
    for (int i = 0; i <= size; i++)
        cout << timeline[i] << "\t";

    cout << endl;

    // print processes
    for (int i = 0; i < size; i++)
        cout << "|P" << order[i] << "|";

    cout << endl;
}

//------------------------------------------------------
// FCFS SCHEDULING
//------------------------------------------------------
void FCFS(Process p[], int n)
{
    int time = 0;
    int order[MAX];
    int timeline[MAX + 1];
    int k = 0;

    timeline[0] = 0;

    cout << "\nFCFS Scheduling\n";

    for (int i = 0; i < n; i++)
    {
        if (time < p[i].at)
            time = p[i].at;

        order[k] = p[i].pid;

        time += p[i].bt;
        timeline[k + 1] = time;

        p[i].ct = time;
        p[i].tat = p[i].ct - p[i].at;
        p[i].wt = p[i].tat - p[i].bt;

        k++;
    }

    printGantt(order, timeline, k);
    printTable(p, n);
}

//------------------------------------------------------
// SJF NON PREEMPTIVE
//------------------------------------------------------
void SJF_NP(Process p[], int n)
{
    int time = 0, completed = 0;
    bool done[MAX] = {false};

    int order[MAX], timeline[MAX + 1];
    int k = 0;

    timeline[0] = 0;

    cout << "\nSJF Non Preemptive\n";

    while (completed < n)
    {
        int idx = -1;
        int min = 9999;

        for (int i = 0; i < n; i++)
        {
            if (!done[i] && p[i].at <= time && p[i].bt < min)
            {
                min = p[i].bt;
                idx = i;
            }
        }

        if (idx != -1)
        {
            order[k] = p[idx].pid;

            time += p[idx].bt;
            timeline[k + 1] = time;

            p[idx].ct = time;
            p[idx].tat = p[idx].ct - p[idx].at;
            p[idx].wt = p[idx].tat - p[idx].bt;

            done[idx] = true;
            completed++;
            k++;
        }
        else
            time++;
    }

    printGantt(order, timeline, k);
    printTable(p, n);
}

//------------------------------------------------------
// SJF PREEMPTIVE (Shortest Remaining Time First)
//------------------------------------------------------
void SJF_P(Process p[], int n)
{
    int time = 0, completed = 0;

    int order[100], timeline[101];
    int k = 0;

    timeline[0] = 0;

    cout << "\nSJF Preemptive\n";

    while (completed < n)
    {
        int idx = -1;
        int min = 9999;

        for (int i = 0; i < n; i++)
        {
            if (p[i].at <= time && p[i].rt > 0 && p[i].rt < min)
            {
                min = p[i].rt;
                idx = i;
            }
        }

        if (idx != -1)
        {
            p[idx].rt--;
            time++;

            order[k] = p[idx].pid;
            timeline[k + 1] = time;
            k++;

            if (p[idx].rt == 0)
            {
                p[idx].ct = time;
                p[idx].tat = p[idx].ct - p[idx].at;
                p[idx].wt = p[idx].tat - p[idx].bt;
                completed++;
            }
        }
        else
            time++;
    }

    printGantt(order, timeline, k);
    printTable(p, n);
}

//------------------------------------------------------
// ROUND ROBIN SCHEDULING
//------------------------------------------------------
void RoundRobin(Process p[], int n, int tq)
{
    int time = 0, completed = 0;

    int order[100], timeline[101];
    int k = 0;

    timeline[0] = 0;

    cout << "\nRound Robin Scheduling\n";

    while (completed < n)
    {
        bool executed = false;

        for (int i = 0; i < n; i++)
        {
            if (p[i].at <= time && p[i].rt > 0)
            {
                executed = true;
                order[k] = p[i].pid;

                if (p[i].rt > tq)
                {
                    time += tq;
                    p[i].rt -= tq;
                }
                else
                {
                    time += p[i].rt;
                    p[i].rt = 0;

                    p[i].ct = time;
                    p[i].tat = p[i].ct - p[i].at;
                    p[i].wt = p[i].tat - p[i].bt;

                    completed++;
                }

                timeline[k + 1] = time;
                k++;
            }
        }

        if (!executed)
            time++;
    }

    printGantt(order, timeline, k);
    printTable(p, n);
}

//------------------------------------------------------
// PRIORITY NON PREEMPTIVE
//------------------------------------------------------
void Priority_NP(Process p[], int n)
{
    int time = 0, completed = 0;
    bool done[MAX] = {false};

    int order[MAX], timeline[MAX + 1];
    int k = 0;

    timeline[0] = 0;

    cout << "\nPriority Non Preemptive\n";

    while (completed < n)
    {
        int idx = -1;
        int high = 9999;

        for (int i = 0; i < n; i++)
        {
            if (!done[i] && p[i].at <= time && p[i].pr < high)
            {
                high = p[i].pr;
                idx = i;
            }
        }

        if (idx != -1)
        {
            order[k] = p[idx].pid;

            time += p[idx].bt;
            timeline[k + 1] = time;

            p[idx].ct = time;
            p[idx].tat = p[idx].ct - p[idx].at;
            p[idx].wt = p[idx].tat - p[idx].bt;

            done[idx] = true;
            completed++;
            k++;
        }
        else
            time++;
    }

    printGantt(order, timeline, k);
    printTable(p, n);
}

//------------------------------------------------------
// PRIORITY PREEMPTIVE
//------------------------------------------------------
void Priority_P(Process p[], int n)
{
    int time = 0, completed = 0;

    int order[100], timeline[101];
    int k = 0;

    timeline[0] = 0;

    cout << "\nPriority Preemptive\n";

    while (completed < n)
    {
        int idx = -1;
        int high = 9999;

        for (int i = 0; i < n; i++)
        {
            if (p[i].at <= time && p[i].rt > 0 && p[i].pr < high)
            {
                high = p[i].pr;
                idx = i;
            }
        }

        if (idx != -1)
        {
            p[idx].rt--;
            time++;

            order[k] = p[idx].pid;
            timeline[k + 1] = time;
            k++;

            if (p[idx].rt == 0)
            {
                p[idx].ct = time;
                p[idx].tat = p[idx].ct - p[idx].at;
                p[idx].wt = p[idx].tat - p[idx].bt;
                completed++;
            }
        }
        else
            time++;
    }

    printGantt(order, timeline, k);
    printTable(p, n);
}