#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <iomanip>

using namespace std;

struct Process {
    int pid;
    int at, bt, priority;
    int ct = 0, tat = 0, wt = 0;
    int remaining_bt;
};

// ---------------- GANTT CHART ----------------
void printGantt(vector<int> gantt) {
    cout << "\nGantt Chart:\n|";

    for (int x : gantt) {
        if (x == -1)
            cout << " Idle |";
        else
            cout << " P" << x << " |";
    }

    cout << "\n0";

    for (int i = 1; i <= gantt.size(); i++) {
        cout << setw(7) << i;
    }
    cout << "\n";
}

// ---------------- RESULT TABLE ----------------
void printResult(vector<Process> p) {
    float totalWT = 0, totalTAT = 0;

    cout << "\nPID\tAT\tBT\tCT\tTAT\tWT\n";

    for (auto &x : p) {
        x.tat = x.ct - x.at;
        x.wt = x.tat - x.bt;

        totalWT += x.wt;
        totalTAT += x.tat;

        cout << "P" << x.pid << "\t"
             << x.at << "\t"
             << x.bt << "\t"
             << x.ct << "\t"
             << x.tat << "\t"
             << x.wt << "\n";
    }

    cout << "\nAvg WT = " << totalWT / p.size();
    cout << "\nAvg TAT = " << totalTAT / p.size() << "\n";
}

// =======================================================
// 1. FCFS PREEMPTIVE
// =======================================================
void fcfsPreemptive(vector<Process> p) {

    int n = p.size(), completed = 0, time = 0;
    vector<int> gantt;

    for (auto &x : p)
        x.remaining_bt = x.bt;

    while (completed < n) {

        int idx = -1;

        for (int i = 0; i < n; i++) {
            if (p[i].at <= time && p[i].remaining_bt > 0) {
                idx = i;
                break;
            }
        }

        if (idx == -1) {
            gantt.push_back(-1);
            time++;
            continue;
        }

        p[idx].remaining_bt--;
        gantt.push_back(p[idx].pid);
        time++;

        if (p[idx].remaining_bt == 0) {
            p[idx].ct = time;
            completed++;
        }
    }

    printGantt(gantt);
    printResult(p);
}

// =======================================================
// 2. FCFS NON-PREEMPTIVE
// =======================================================
void fcfsNonPreemptive(vector<Process> p) {

    sort(p.begin(), p.end(),
         [](Process a, Process b) {
             return a.at < b.at;
         });

    int time = 0;
    vector<int> gantt;

    for (auto &x : p) {

        if (time < x.at) {
            while (time < x.at) {
                gantt.push_back(-1);
                time++;
            }
        }

        for (int i = 0; i < x.bt; i++) {
            gantt.push_back(x.pid);
            time++;
        }

        x.ct = time;
    }

    printGantt(gantt);
    printResult(p);
}

// =======================================================
// 3. PRIORITY PREEMPTIVE
// =======================================================
void priorityPreemptive(vector<Process> p) {

    int n = p.size(), completed = 0, time = 0;
    vector<int> gantt;

    for (auto &x : p)
        x.remaining_bt = x.bt;

    while (completed < n) {

        int idx = -1, highest = 9999;

        for (int i = 0; i < n; i++) {

            if (p[i].at <= time &&
                p[i].remaining_bt > 0 &&
                p[i].priority < highest) {

                highest = p[i].priority;
                idx = i;
            }
        }

        if (idx == -1) {
            gantt.push_back(-1);
            time++;
            continue;
        }

        p[idx].remaining_bt--;
        gantt.push_back(p[idx].pid);
        time++;

        if (p[idx].remaining_bt == 0) {
            p[idx].ct = time;
            completed++;
        }
    }

    printGantt(gantt);
    printResult(p);
}

// =======================================================
// 4. PRIORITY NON-PREEMPTIVE
// =======================================================
void priorityNonPreemptive(vector<Process> p) {

    int n = p.size(), completed = 0, time = 0;
    vector<bool> done(n, false);
    vector<int> gantt;

    while (completed < n) {

        int idx = -1, highest = 9999;

        for (int i = 0; i < n; i++) {

            if (!done[i] &&
                p[i].at <= time &&
                p[i].priority < highest) {

                highest = p[i].priority;
                idx = i;
            }
        }

        if (idx == -1) {
            gantt.push_back(-1);
            time++;
            continue;
        }

        done[idx] = true;

        for (int i = 0; i < p[idx].bt; i++) {
            gantt.push_back(p[idx].pid);
            time++;
        }

        p[idx].ct = time;
        completed++;
    }

    printGantt(gantt);
    printResult(p);
}

// =======================================================
// 5. ROUND ROBIN PREEMPTIVE
// =======================================================
void roundRobinPreemptive(vector<Process> p, int tq) {

    int n = p.size(), completed = 0, time = 0;
    queue<int> q;
    vector<int> gantt;
    vector<bool> visited(n, false);

    for (auto &x : p)
        x.remaining_bt = x.bt;

    q.push(0);
    visited[0] = true;

    while (completed < n) {

        if (q.empty()) {
            gantt.push_back(-1);
            time++;
            continue;
        }

        int i = q.front();
        q.pop();

        int run = min(tq, p[i].remaining_bt);

        for (int j = 0; j < run; j++) {
            gantt.push_back(p[i].pid);
            time++;
        }

        p[i].remaining_bt -= run;

        for (int k = 0; k < n; k++) {
            if (!visited[k] && p[k].at <= time) {
                q.push(k);
                visited[k] = true;
            }
        }

        if (p[i].remaining_bt > 0)
            q.push(i);
        else {
            p[i].ct = time;
            completed++;
        }
    }

    printGantt(gantt);
    printResult(p);
}

// =======================================================
// 6. ROUND ROBIN NON-PREEMPTIVE
// =======================================================
void roundRobinNonPreemptive(vector<Process> p) {

    sort(p.begin(), p.end(),
         [](Process a, Process b) {
             return a.at < b.at;
         });

    int time = 0;
    vector<int> gantt;

    for (auto &x : p) {

        if (time < x.at) {
            while (time < x.at) {
                gantt.push_back(-1);
                time++;
            }
        }

        for (int i = 0; i < x.bt; i++) {
            gantt.push_back(x.pid);
            time++;
        }

        x.ct = time;
    }

    printGantt(gantt);
    printResult(p);
}

// =======================================================
// 7. SJF NON-PREEMPTIVE
// =======================================================
void sjfNonPreemptive(vector<Process> p) {

    int n = p.size(), completed = 0, time = 0;
    vector<bool> done(n, false);
    vector<int> gantt;

    while (completed < n) {

        int idx = -1, shortest = 9999;

        for (int i = 0; i < n; i++) {

            if (!done[i] &&
                p[i].at <= time &&
                p[i].bt < shortest) {

                shortest = p[i].bt;
                idx = i;
            }
        }

        if (idx == -1) {
            gantt.push_back(-1);
            time++;
            continue;
        }

        done[idx] = true;

        for (int i = 0; i < p[idx].bt; i++) {
            gantt.push_back(p[idx].pid);
            time++;
        }

        p[idx].ct = time;
        completed++;
    }

    printGantt(gantt);
    printResult(p);
}

// =======================================================
// 8. SRTF PREEMPTIVE
// =======================================================
void srtf(vector<Process> p) {

    int n = p.size(), completed = 0, time = 0;
    vector<int> gantt;

    for (auto &x : p)
        x.remaining_bt = x.bt;

    while (completed < n) {

        int idx = -1, shortest = 9999;

        for (int i = 0; i < n; i++) {

            if (p[i].at <= time &&
                p[i].remaining_bt > 0 &&
                p[i].remaining_bt < shortest) {

                shortest = p[i].remaining_bt;
                idx = i;
            }
        }

        if (idx == -1) {
            gantt.push_back(-1);
            time++;
            continue;
        }

        p[idx].remaining_bt--;
        gantt.push_back(p[idx].pid);
        time++;

        if (p[idx].remaining_bt == 0) {
            p[idx].ct = time;
            completed++;
        }
    }

    printGantt(gantt);
    printResult(p);
}

// =======================================================
// MAIN
// =======================================================
int main() {

    int n;
    cout << "Enter number of processes: ";
    cin >> n;

    vector<Process> p(n);

    for (int i = 0; i < n; i++) {

        p[i].pid = i + 1;

        cout << "\nP" << i + 1 << ":\n";
        cout << "AT: ";
        cin >> p[i].at;
        cout << "BT: ";
        cin >> p[i].bt;
        cout << "Priority: ";
        cin >> p[i].priority;
    }

    int choice;

    cout << "\n1 FCFS Preemptive\n";
    cout << "2 FCFS Non-Preemptive\n";
    cout << "3 Priority Preemptive\n";
    cout << "4 Priority Non-Preemptive\n";
    cout << "5 Round Robin Preemptive\n";
    cout << "6 Round Robin Non-Preemptive\n";
    cout << "7 SJF Non-Preemptive\n";
    cout << "8 SRTF Preemptive\n";

    cout << "\nEnter choice: ";
    cin >> choice;

    switch (choice) {

        case 1: fcfsPreemptive(p); break;
        case 2: fcfsNonPreemptive(p); break;
        case 3: priorityPreemptive(p); break;
        case 4: priorityNonPreemptive(p); break;

        case 5: {
            int tq;
            cout << "Time Quantum: ";
            cin >> tq;
            roundRobinPreemptive(p, tq);
            break;
        }

        case 6: roundRobinNonPreemptive(p); break;
        case 7: sjfNonPreemptive(p); break;
        case 8: srtf(p); break;

        default:
            cout << "Invalid choice";
    }

    return 0;
}