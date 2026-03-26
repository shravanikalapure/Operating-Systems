#include<iostream>
#include<vector>
#include<algorithm>
#include<climits>
#include<queue>
#include <iomanip>
#include <limits>
#include <sstream>
using namespace std;

struct Process{
    int pid;
    int arrival_time;
    int burst_time;
    int priority;
    int completion_time;
    int turnaround_time;
    int waiting_time;
    int remaining_time;
};
struct Result {
    string algo_name;
    double avg_wt;
    double avg_tat;
};
void compareResults(vector<Result> results);
Result FCFS(vector<Process> process);
Result SJF_Preemptive(vector<Process> process);
Result  SJF_NonPreemptive(vector<Process> process);
Result RoundRobin(vector<Process> process, int time_quantum);
Result Priority_Preemptive(vector<Process> process);
Result Priority_NonPreemptive(vector<Process> process);

void printGantt(vector<int> gantt);
void printReadyQueue(vector<Process> process, int current_time);
Result display(vector<Process> process, string algo_name);
int getPositiveInteger(string message);

   int main() {
    int n = getPositiveInteger("Enter number of processes: ");

    vector<Process> process(n);
    for(int i = 0; i < n; i++){
        process[i].pid = i + 1;
        process[i].arrival_time = getPositiveInteger("Arrival Time for P" + to_string(i+1) + ": ");
        process[i].burst_time = getPositiveInteger("Burst Time for P" + to_string(i+1) + ": ");
        process[i].priority = getPositiveInteger("Priority for P" + to_string(i+1) + ": ");
        process[i].remaining_time = process[i].burst_time;
        process[i].completion_time = 0;
    }

    vector<Result> results;

    cout << "\nSelect Scheduling Algorithms to Compare:\n";
    cout << "1. FCFS\n";
    cout << "2. SJF Non-Preemptive\n";
    cout << "3. SJF Preemptive\n";
    cout << "4. Priority Non-Preemptive\n";
    cout << "5. Priority Preemptive\n";
    cout << "6. Round Robin\n";
    cout << "Enter choices separated by space (e.g., 1 3 6): ";

    string line;
    cin.ignore(); // clear newline from previous input
    getline(cin, line);
    istringstream iss(line);
    int choice;

    while(iss >> choice){
        switch(choice){
            case 1: results.push_back(FCFS(process)); break;
            case 2: results.push_back(SJF_NonPreemptive(process)); break;
            case 3: results.push_back(SJF_Preemptive(process)); break;
            case 4: results.push_back(Priority_NonPreemptive(process)); break;
            case 5: results.push_back(Priority_Preemptive(process)); break;
            case 6: {
                int quantum = getPositiveInteger("Enter Time Quantum for Round Robin: ");
                results.push_back(RoundRobin(process, quantum));
                break;
            }
            default: cout << "Invalid choice: " << choice << endl;
        }
    }

    compareResults(results);

    return 0;
}
//function to check Edge cases for input
int getPositiveInteger(string message)
{
    int value;

    while(true)
    {
        cout << message;
        cin >> value;

        if(cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Enter a positive integer.\n";
        }
        else if(value < 0)
        {
            cout << "Negative numbers not allowed.\n";
        }
        else
        {
            return value;
        }
    }
}

    ///FCFS Function
    Result FCFS(vector<Process>process){
        vector<int> gantt;
        //Sorting based on arrival time
        sort(process.begin(),process.end(),
        [](const Process &a,const Process &b){ //const to avoid copies
            return a.arrival_time < b.arrival_time;
        });
        int current_time = 0; //Time at which the CPU is free
        for(int i = 0; i<process.size();i++){
            printReadyQueue(process, current_time);
            if(current_time < process[i].arrival_time){ //If the CPU is idle, move the current time to the arrival time of the process
                current_time = process[i].arrival_time;
            }
            for(int k = 0; k < process[i].burst_time; k++)
            gantt.push_back(process[i].pid);
            process[i].completion_time = current_time + process[i].burst_time;
            process[i].turnaround_time = process[i].completion_time - process[i].arrival_time; //TAT = CT - AT
            process[i].waiting_time = process[i].turnaround_time - process[i].burst_time; //WT = TAT - BT
            current_time += process[i].burst_time; //moving current time to CT of the process for next iteration
    }
    printGantt(gantt);
    return display(process, "FCFS");
   
}
Result SJF_Preemptive(vector<Process>process){
     vector<int> gantt;
      for(int i = 0; i < process.size(); i++)
        process[i].remaining_time = process[i].burst_time;
    int current_time = 0;
    int completed = 0; //how many processes have completed
    vector<bool> is_completed(process.size(),false); //To keep track of completed processes
    while(completed < process.size()){
         printReadyQueue(process, current_time);
        int idx = -1; //index of selected process
        int min_burst_time = INT_MAX;
        for(int i = 0; i<process.size();i++){
            if(process[i].arrival_time <=current_time && !is_completed[i]){ //AT is less that current time and not completed
                if(process[i].remaining_time < min_burst_time){ //Finding the process with the minimum burst time
                min_burst_time = process[i].remaining_time;
                idx = i; //index of the process with the minimum burst time
                }    
            }
        }
        if(idx != -1){ //There is a process to execute(With min burst time)
           //Execute for 1 unit of time
           gantt.push_back(process[idx].pid);
            process[idx].remaining_time--; //Decrement the remaining time of the process
            current_time++;
            if(process[idx].remaining_time == 0){ //If the process is completed
                process[idx].completion_time = current_time;
                process[idx].turnaround_time = process[idx].completion_time - process[idx].arrival_time;
                process[idx].waiting_time = process[idx].turnaround_time - process[idx].burst_time;
                is_completed[idx] = true;
                completed++;
            }
           
        }
        else{ //No process is ready to execute, move the current time to the next arrival time
            current_time++;
        }

}
printGantt(gantt);
    return display(process, "SJF_Preemptive");
}
Result SJF_NonPreemptive(vector<Process>process){
     vector<int> gantt;
    int current_time = 0;
    int completed = 0;
    vector<bool> is_completed(process.size(),false); //To keep track of completed processes
    while(completed < process.size()){
        printReadyQueue(process, current_time);
        int idx = -1; //index of selected process
        int min_burst_time = INT_MAX;
        for(int i = 0; i<process.size();i++){
            if(process[i].arrival_time <=current_time && !is_completed[i]){
                if(process[i].burst_time < min_burst_time){ //Finding the process with the minimum burst time
                min_burst_time = process[i].burst_time;
                idx = i; //index of the process with the minimum burst time
                }    
            }
        }
        if(idx != -1){ //There is a process to execute(With min burst time)
        for(int k = 0; k < process[idx].burst_time; k++)
        gantt.push_back(process[idx].pid);
            current_time += process[idx].burst_time; //Move the current time to the completion time of the process
            process[idx].completion_time = current_time;
            process[idx].turnaround_time = process[idx].completion_time - process[idx].arrival_time;
            process[idx].waiting_time = process[idx].turnaround_time - process[idx].burst_time;
            is_completed[idx] = true;
            completed++;
           
        }
        else{
            current_time++;
        }
}
printGantt(gantt);
return display(process, "SJF_NonPreemptive");
}

Result RoundRobin(vector<Process>process , int time_quantum){
     vector<int> gantt;
    int current_time = 0;
    int completed = 0;
    queue<int> q; //Ready Queue
    //initializing remaining time
    for(int i = 0; i<process.size();i++){
        process[i].remaining_time = process[i].burst_time;
    }
    vector<bool> in_queue(process.size(),false); //To keep track of processes in the queue
    while(completed < process.size()){ //not yet reached the end
        printReadyQueue(process, current_time);
        //adding the newly arrived processes to the queue
        for(int i = 0; i<process.size();i++){
            if(process[i].arrival_time <= current_time && !in_queue[i] && process[i].remaining_time > 0){ //Process is not in queue and not completed
                q.push(i);
                in_queue[i] = true;
            }
        }
        if(q.empty()){
            current_time++;
            continue;
        }
        int idx = q.front(); //index of the process at the front of the queue
        q.pop();
        int exec_time = min(time_quantum,process[idx].remaining_time); //Time to execute the process in this round
        for(int k = 0; k < exec_time; k++)
         gantt.push_back(process[idx].pid);
        process[idx].remaining_time -= exec_time; //Decrement the remaining time of the process
        current_time += exec_time; //Move the current time by the execution time

        //adding newliy arrived process during execution
        for(int i = 0; i<process.size();i++){
            if(process[i].arrival_time <= current_time && !in_queue[i] && process[i].remaining_time > 0){ //Process is not in queue and not completed
                q.push(i);
                in_queue[i] = true;
            }
        }
        if(process[idx].remaining_time > 0){ //If the process is not completed,
            q.push(idx); //Add it back to the queue
        }
        else{ //If the process is completed
            process[idx].completion_time = current_time;
            process[idx].turnaround_time = current_time - process[idx].arrival_time;
            process[idx].waiting_time = process[idx].turnaround_time - process[idx].burst_time;
            completed++;
        }
    }    
    printGantt(gantt);
    return display(process,"RoundRobin");  
}

    Result Priority_Preemptive(vector<Process>process){
        vector<int> gantt;
         for(int i = 0; i < process.size(); i++)
        process[i].remaining_time = process[i].burst_time;
        int current_time = 0;
        int completed = 0;
        vector<bool> is_completed(process.size(),false); //To keep track of completed processes
        while(completed < process.size()){
            printReadyQueue(process, current_time);
            int idx = -1; //index of selected process
            int highest_priority = INT_MAX;
            for(int i = 0; i<process.size();i++){
               if(process[i].arrival_time <= current_time && !is_completed[i]){ //AT is less that current time and not completed
                    if(process[i].priority < highest_priority){ //Finding the process with the highest priority (lowest priority number)
                    highest_priority = process[i].priority;
                     idx = i; //index of the process with the highest priority
                    }    
                }
            }
            if(idx != -1){ //There is a process to execute(With highest priority)
                //Execute for 1 unit of time
                gantt.push_back(process[idx].pid);
                process[idx].remaining_time--; //Decrement the remaining time of the process
                current_time++;
                if(process[idx].remaining_time == 0){ //If the process is completed
                    process[idx].completion_time = current_time;
                    process[idx].turnaround_time = process[idx].completion_time - process[idx].arrival_time;
                    process[idx].waiting_time = process[idx].turnaround_time - process[idx].burst_time;
                    is_completed[idx] = true;
                    completed++;
                }
            }
            else{ //No process is ready to execute, move the current time to the next arrival time
                current_time++;
            }
        }
          printGantt(gantt);
        return display(process,"Priority_Preemptive");
    }

    Result Priority_NonPreemptive(vector<Process>process){
        vector<int> gantt;
        int current_time = 0;
        int completed = 0;
        vector<bool> is_completed(process.size(),false); //To keep track of completed processes
        while(completed < process.size()){
            printReadyQueue(process, current_time);
            int idx = -1; //index of selected process
            int highest_priority = INT_MAX;
            for(int i = 0; i<process.size();i++){
               if(process[i].arrival_time <= current_time && !is_completed[i]){ //AT is less that current time and not completed
                    if(process[i].priority < highest_priority){ //Finding the process with the highest priority (lowest priority number)
                    highest_priority = process[i].priority;
                     idx = i; //index of the process with the highest priority
                    }    
                }
            }
            if(idx != -1){ //There is a process to execute(With highest priority)
                for(int k = 0; k < process[idx].burst_time; k++)
                gantt.push_back(process[idx].pid);
                current_time += process[idx].burst_time; //Move the current time to the completion time of the process
                process[idx].completion_time = current_time;
                process[idx].turnaround_time = process[idx].completion_time - process[idx].arrival_time;
                process[idx].waiting_time = process[idx].turnaround_time - process[idx].burst_time;
                is_completed[idx] = true;
                completed++;
            }
            else{ //No process is ready to execute, move the current time to the next arrival time
                current_time++;
            }
        }
        printGantt(gantt);
        return display(process,"Priority_NonPreemptive");
    }

void printGantt(vector<int> gantt)
{
    cout << "\nGantt Chart:\n\n";

    int width = 5;  // width of each block

    // Print process blocks
    for(int i = 0; i < gantt.size(); i++)
    {
        cout << "|"
             << setw(width-1)
             << ("P" + to_string(gantt[i]));
    }

    cout << "|\n";

    // Print timeline numbers
    for(int i = 0; i <= gantt.size(); i++)
    {
        cout << setw(width) << i;
    }

    cout << endl;
}
void printReadyQueue(vector<Process> process, int current_time){
    cout << "Time " << current_time << " Ready Queue: ";
    for(int i = 0; i < process.size(); i++){
        if(process[i].arrival_time <= current_time &&
   process[i].remaining_time > 0 &&
   process[i].completion_time == 0){

            cout << "P" << process[i].pid << " ";
        }
    }
    cout << endl;
}

Result display(vector<Process> process, string algo_name){
    double total_wt = 0;
    double total_tat = 0;

    cout << "\n--- " << algo_name << " ---\n";

    for(int i = 0; i < process.size(); i++){
        total_wt += process[i].waiting_time;
        total_tat += process[i].turnaround_time;
    }

    Result res;
    res.algo_name = algo_name;
    res.avg_wt = total_wt / process.size();
    res.avg_tat = total_tat / process.size();

    cout << "Average WT: " << res.avg_wt << endl;
    cout << "Average TAT: " << res.avg_tat << endl;

    return res;
}

void compareResults(vector<Result> results){
    int width_algo = 25;
    int width_wt = 15;
    int width_tat = 15;
   
    cout << "\n\n===== Comparison Table =====\n";

    // Print top border
    cout << "+" << string(width_algo, '-')
         << "+" << string(width_wt, '-')
         << "+" << string(width_tat, '-') << "+\n";

    // Print header row
    cout << "|" << left << setw(width_algo) << "Algorithm"
         << "|" << setw(width_wt) << "Avg WT"
         << "|" << setw(width_tat) << "Avg TAT" << "|\n";

    // Print header bottom border
    cout << "+" << string(width_algo, '-')
         << "+" << string(width_wt, '-')
         << "+" << string(width_tat, '-') << "+\n";

    double best_wt = numeric_limits<double>::max();
    string best_algo_wt;
    double worst_wt = numeric_limits<double>::lowest();
    string worst_algo_wt;

    for(auto &r : results){
        cout << "|" << left << setw(width_algo) << r.algo_name
             << "|" << setw(width_wt) << fixed << setprecision(2) << r.avg_wt
             << "|" << setw(width_tat) << fixed << setprecision(2) << r.avg_tat << "|\n";

        if(r.avg_wt < best_wt){
            best_wt = r.avg_wt;
            best_algo_wt = r.algo_name;
        }
        if(r.avg_wt > worst_wt){
            worst_wt = r.avg_wt;
            worst_algo_wt = r.algo_name;
        }
    }

    // Print bottom border
    cout << "+" << string(width_algo, '-')
         << "+" << string(width_wt, '-')
         << "+" << string(width_tat, '-') << "+\n";

    cout << "\nBest Algorithm (Lowest WT): " << best_algo_wt << " with WT = " << best_wt << endl;
    cout << "Worst Algorithm (Highest WT): " << worst_algo_wt << " with WT = " << worst_wt << endl;
}