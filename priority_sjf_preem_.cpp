#include <iostream>
#include <vector>
#include <climits>
using namespace std;

struct Process {
    int id, at, bt, rt, ct, wt, tat, priority;
};

// Print 
void printResult(vector<Process> &p) {
    cout << "\nID\tAT\tBT\tCT\tTAT\tWT\n";
    for (auto &pr : p) {
        cout << pr.id << "\t" << pr.at << "\t" << pr.bt << "\t"
             << pr.ct << "\t" << pr.tat << "\t" << pr.wt << endl;
    }
}

//  preemptive SJF 


void SJF_Preemptive(vector<Process> p, int n) {
    int complete = 0, time = 0;

    while (complete != n) {
        int minm = INT_MAX, shortest = -1;
        bool found = false;

        for (int i = 0; i < n; i++) {
            if (p[i].at <= time && p[i].rt > 0 && p[i].rt < minm) {
                minm = p[i].rt;
                shortest = i;
                found = true;
            }
        }

        if (!found) {
            time++;
            continue;
        }

        p[shortest].rt--;

        if (p[shortest].rt == 0) {
            complete++;
            int finish_time = time + 1;

            p[shortest].ct = finish_time;
            p[shortest].tat = p[shortest].ct - p[shortest].at;
            p[shortest].wt = p[shortest].tat - p[shortest].bt;
        }

        time++;
    }

    cout << "\n--- preemptive SJF ---\n";
    printResult(p);
}

// non-preemptive priority scheduling


void Priority_NonPreemptive(vector<Process> p, int n) {
    int complete = 0, time = 0;
    vector<bool> done(n, false);

    while (complete != n) {
        int highest_priority = INT_MAX;
        int index = -1;

        
        for (int i = 0; i < n; i++) {
            if (!done[i] && p[i].at <= time &&
                p[i].priority < highest_priority) {

                highest_priority = p[i].priority;
                index = i;
            }
        }

        if (index == -1) {
            time++;
            continue;
        }

        
        time += p[index].bt;

        p[index].ct = time;
        p[index].tat = p[index].ct - p[index].at;
        p[index].wt = p[index].tat - p[index].bt;

        done[index] = true;
        complete++;
    }

    cout << "\n--- non-preemptive priority scheduling ---\n";
    printResult(p);
}


int main() {
    int n, choice;

    cout << "Enter number of processes: ";
    cin >> n;

    vector<Process> p(n);

    for (int i = 0; i < n; i++) {
        cout << "\nProcess " << i + 1 << endl;
        p[i].id = i + 1;

        cout << "arrival Time: ";
        cin >> p[i].at;

        cout << "burst Time: ";
        cin >> p[i].bt;

        cout << "priority: ";
        cin >> p[i].priority;

        p[i].rt = p[i].bt;
    }

    cout << "\nPress 1 for Preemptive SJF";
    cout << "\nPress 2 for Non-Preemptive Priority\n";
    cin >> choice;

    if (choice == 1) {
        SJF_Preemptive(p, n);
    } else if (choice == 2) {
        Priority_NonPreemptive(p, n);
    } else {
        cout << "invalid Choice!\n";
    }

    return 0;
}