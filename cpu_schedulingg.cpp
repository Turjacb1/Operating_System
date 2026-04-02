#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct Process {
    string name;
    int at, bt, ct, tat, wt;
};

int main() {
    int n;
    cout << "Enter number of processes: ";
    cin >> n;

    vector<Process> p(n);

    // Input
    for(int i = 0; i < n; i++) {
        cout << "\nEnter Process Name: ";
        cin >> p[i].name;

        cout << "Enter Arrival Time: ";
        cin >> p[i].at;

        cout << "Enter Burst Time: ";
        cin >> p[i].bt;
    }

   
    sort(p.begin(), p.end(), [](Process a, Process b) {
        return a.at < b.at;
    });

   
    p[0].ct = p[0].at + p[0].bt;

    for(int i = 1; i < n; i++) {
        if(p[i-1].ct < p[i].at)
            p[i].ct = p[i].at + p[i].bt;
        else
            p[i].ct = p[i-1].ct + p[i].bt;
    }

    
    for(int i = 0; i < n; i++) {
        p[i].tat = p[i].ct - p[i].at;
        p[i].wt = p[i].tat - p[i].bt;
    }

    cout << "\nGantt Chart:\n|";
    for(int i = 0; i < n; i++) {
        cout << " " << p[i].name << " |";
    }

    cout << "\n0";
    for(int i = 0; i < n; i++) {
        cout << "   " << p[i].ct;
    }

    
    float total_wt = 0;
    cout << "\n\nProcess\tAT\tBT\tCT\tTAT\tWT\n";

    for(int i = 0; i < n; i++) {
        cout << p[i].name << "\t"
             << p[i].at << "\t"
             << p[i].bt << "\t"
             << p[i].ct << "\t"
             << p[i].tat << "\t"
             << p[i].wt << "\n";

        total_wt += p[i].wt;
    }

    cout << "\nAverage Waiting Time = " << total_wt / n << endl;

    return 0;
}