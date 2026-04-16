#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
using namespace std;

struct Process {
    int pid, at, bt, ct, tat, wt, rt;
};

//  FCFS 
void fcfs(vector<Process> p) {
    int n = p.size();
    sort(p.begin(), p.end(), [](Process a, Process b){
        return a.at < b.at;
    });

    int time = 0;
    for(int i = 0; i < n; i++) {
        if(time < p[i].at)
            time = p[i].at;

        p[i].ct = time + p[i].bt;
        time = p[i].ct;

        p[i].tat = p[i].ct - p[i].at;
        p[i].wt = p[i].tat - p[i].bt;
    }

    cout << "\nPID\tAT\tBT\tCT\tTAT\tWT\n";
    for(auto pr : p) {
        cout << pr.pid << "\t" << pr.at << "\t" << pr.bt << "\t"
             << pr.ct << "\t" << pr.tat << "\t" << pr.wt << endl;
    }
}

//  SJF (Non-preemptive) 
void sjf(vector<Process> p) {
    int n = p.size();
    vector<bool> done(n, false);
    int completed = 0, time = 0;

    while(completed < n) {
        int idx = -1;
        int min_bt = 1e9;

        for(int i = 0; i < n; i++) {
            if(p[i].at <= time && !done[i] && p[i].bt < min_bt) {
                min_bt = p[i].bt;
                idx = i;
            }
        }

        if(idx == -1) {
            time++;
            continue;
        }

        p[idx].ct = time + p[idx].bt;
        time = p[idx].ct;

        p[idx].tat = p[idx].ct - p[idx].at;
        p[idx].wt = p[idx].tat - p[idx].bt;

        done[idx] = true;
        completed++;
    }

    cout << "\nPID\tAT\tBT\tCT\tTAT\tWT\n";
    for(auto pr : p) {
        cout << pr.pid << "\t" << pr.at << "\t" << pr.bt << "\t"
             << pr.ct << "\t" << pr.tat << "\t" << pr.wt << endl;
    }
}

//  Round Robin 
void roundRobin(vector<Process> p, int quantum) {
    int n = p.size();
    queue<int> q;
    vector<int> rem_bt(n);

    for(int i = 0; i < n; i++)
        rem_bt[i] = p[i].bt;

    int time = 0;
    q.push(0);

    vector<bool> inQueue(n, false);
    inQueue[0] = true;

    while(!q.empty()) {
        int i = q.front();
        q.pop();

        if(rem_bt[i] > quantum) {
            time += quantum;
            rem_bt[i] -= quantum;
        } else {
            time += rem_bt[i];
            rem_bt[i] = 0;

            p[i].ct = time;
            p[i].tat = p[i].ct - p[i].at;
            p[i].wt = p[i].tat - p[i].bt;
        }

        for(int j = 0; j < n; j++) {
            if(p[j].at <= time && !inQueue[j] && rem_bt[j] > 0) {
                q.push(j);
                inQueue[j] = true;
            }
        }

        if(rem_bt[i] > 0)
            q.push(i);
    }

    cout << "\nPID\tAT\tBT\tCT\tTAT\tWT\n";
    for(auto pr : p) {
        cout << pr.pid << "\t" << pr.at << "\t" << pr.bt << "\t"
             << pr.ct << "\t" << pr.tat << "\t" << pr.wt << endl;
    }
}


int main() {
    int n, choice;

    cout << "Enter number of processes: ";
    cin >> n;

    vector<Process> p(n);

    for(int i = 0; i < n; i++) {
        cout << "Process " << i+1 << " AT BT: ";
        cin >> p[i].at >> p[i].bt;
        p[i].pid = i + 1;
    }

    cout << "\npress 1 for FCFS\n";
    cout << "press 2 for SJF\n";
    cout << "press 3 for Round Robin\n";
    cout << "enter choice: ";
    cin >> choice;

    if(choice == 1) {
        fcfs(p);
    }
    else if(choice == 2) {
        sjf(p);
    }
    else if(choice == 3) {
        int quantum;
        cout << " quantum time: ";
        cin >> quantum;
        roundRobin(p, quantum);
    }
    else {
        cout << "invalid choice";
    }

    return 0;
}