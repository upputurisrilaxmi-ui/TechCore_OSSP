#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <iomanip>
#include <string>

using namespace std;

struct Process {
    string name;
    int at;       // Arrival Time
    int bt;       // Burst Time
    int priority;

    int ct = 0;   // Completion Time
    int tat = 0;  // Turnaround Time
    int wt = 0;   // Waiting Time
    int rt = -1;  // Response Time

    int remaining;
};

vector<Process> original = {
    {"P1", 0, 4, 2},
    {"P2", 0, 9, 1},
    {"P3", 0, 6, 2},
    {"P4", 0, 4, 3},
    {"P5", 0, 9, 1},
    {"P6", 0, 6, 2},
    {"P7", 1, 4, 3},
    {"P8", 1, 9, 1},
    {"P9", 1, 6, 2}
};

void calculate(vector<Process>& p) {
    double totalWT = 0, totalTAT = 0;

    for (auto &x : p) {
        x.tat = x.ct - x.at;
        x.wt = x.tat - x.bt;

        totalWT += x.wt;
        totalTAT += x.tat;
    }

    cout << "\nProcess\tAT\tBT\tPriority\tCT\tTAT\tWT\tRT\n";
    cout << "---------------------------------------------------------------\n";

    for (auto &x : p) {
        cout << x.name << "\t"
             << x.at << "\t"
             << x.bt << "\t"
             << x.priority << "\t\t"
             << x.ct << "\t"
             << x.tat << "\t"
             << x.wt << "\t"
             << x.rt << "\n";
    }

    cout << "\nAverage Waiting Time    = "
         << fixed << setprecision(2)
         << totalWT / p.size();

    cout << "\nAverage Turnaround Time = "
         << fixed << setprecision(2)
         << totalTAT / p.size() << "\n";
}


// --------------------------------------------------
// FCFS
// --------------------------------------------------
void FCFS() {
    cout << "\n========== FCFS ==========\n";

    vector<Process> p = original;

    sort(p.begin(), p.end(), [](Process a, Process b) {
        if (a.at != b.at)
            return a.at < b.at;
        return a.name < b.name;
    });

    int time = 0;

    cout << "\nGantt Chart:\n";

    for (auto &x : p) {
        if (time < x.at)
            time = x.at;

        if (x.rt == -1)
            x.rt = time - x.at;

        cout << "| " << x.name << " ";

        time += x.bt;
        x.ct = time;
    }

    cout << "|\n";

    calculate(p);
}


// --------------------------------------------------
// SJF - Non Preemptive
// --------------------------------------------------
void SJF() {
    cout << "\n========== SJF (Non-Preemptive) ==========\n";

    vector<Process> p = original;

    int n = p.size();
    int completed = 0;
    int time = 0;

    vector<bool> done(n, false);

    cout << "\nGantt Chart:\n";

    while (completed < n) {
        int index = -1;

        for (int i = 0; i < n; i++) {
            if (!done[i] && p[i].at <= time) {
                if (index == -1 ||
                    p[i].bt < p[index].bt ||
                    (p[i].bt == p[index].bt && p[i].at < p[index].at)) {
                    index = i;
                }
            }
        }

        if (index == -1) {
            time++;
            continue;
        }

        if (p[index].rt == -1)
            p[index].rt = time - p[index].at;

        cout << "| " << p[index].name << " ";

        time += p[index].bt;
        p[index].ct = time;

        done[index] = true;
        completed++;
    }

    cout << "|\n";

    calculate(p);
}


// --------------------------------------------------
// SRTF - Preemptive SJF
// --------------------------------------------------
void SRTF() {
    cout << "\n========== SRTF (Preemptive SJF) ==========\n";

    vector<Process> p = original;

    int n = p.size();
    int completed = 0;
    int time = 0;

    for (auto &x : p)
        x.remaining = x.bt;

    cout << "\nGantt Chart:\n";

    string previous = "";

    while (completed < n) {

        int index = -1;

        for (int i = 0; i < n; i++) {
            if (p[i].at <= time && p[i].remaining > 0) {

                if (index == -1 ||
                    p[i].remaining < p[index].remaining ||
                    (p[i].remaining == p[index].remaining &&
                     p[i].at < p[index].at)) {

                    index = i;
                }
            }
        }

        if (index == -1) {
            time++;
            continue;
        }

        if (p[index].rt == -1)
            p[index].rt = time - p[index].at;

        if (previous != p[index].name) {
            cout << "| " << p[index].name << " ";
            previous = p[index].name;
        }

        p[index].remaining--;
        time++;

        if (p[index].remaining == 0) {
            p[index].ct = time;
            completed++;
        }
    }

    cout << "|\n";

    calculate(p);
}


// --------------------------------------------------
// Priority Scheduling - Non Preemptive
// Smaller number = Higher priority
// --------------------------------------------------
void PriorityScheduling() {
    cout << "\n========== PRIORITY (Non-Preemptive) ==========\n";

    vector<Process> p = original;

    int n = p.size();
    int completed = 0;
    int time = 0;

    vector<bool> done(n, false);

    cout << "\nGantt Chart:\n";

    while (completed < n) {

        int index = -1;

        for (int i = 0; i < n; i++) {

            if (!done[i] && p[i].at <= time) {

                if (index == -1 ||
                    p[i].priority < p[index].priority ||
                    (p[i].priority == p[index].priority &&
                     p[i].at < p[index].at)) {

                    index = i;
                }
            }
        }

        if (index == -1) {
            time++;
            continue;
        }

        if (p[index].rt == -1)
            p[index].rt = time - p[index].at;

        cout << "| " << p[index].name << " ";

        time += p[index].bt;
        p[index].ct = time;

        done[index] = true;
        completed++;
    }

    cout << "|\n";

    calculate(p);
}


// --------------------------------------------------
// Round Robin
// --------------------------------------------------
void RoundRobin(int quantum) {
    cout << "\n========== ROUND ROBIN ==========\n";
    cout << "Time Quantum = " << quantum << "\n";

    vector<Process> p = original;

    int n = p.size();

    for (auto &x : p)
        x.remaining = x.bt;

    queue<int> q;
    vector<bool> added(n, false);

    int time = 0;
    int completed = 0;

    // Add processes that arrive at time 0
    for (int i = 0; i < n; i++) {
        if (p[i].at == 0) {
            q.push(i);
            added[i] = true;
        }
    }

    cout << "\nGantt Chart:\n";

    while (completed < n) {

        if (q.empty()) {
            time++;

            for (int i = 0; i < n; i++) {
                if (!added[i] && p[i].at <= time) {
                    q.push(i);
                    added[i] = true;
                }
            }

            continue;
        }

        int index = q.front();
        q.pop();

        if (p[index].rt == -1)
            p[index].rt = time - p[index].at;

        cout << "| " << p[index].name << " ";

        int execution = min(quantum, p[index].remaining);

        for (int t = 0; t < execution; t++) {

            p[index].remaining--;
            time++;

            // Add newly arrived processes
            for (int i = 0; i < n; i++) {
                if (!added[i] && p[i].at <= time) {
                    q.push(i);
                    added[i] = true;
                }
            }
        }

        if (p[index].remaining == 0) {
            p[index].ct = time;
            completed++;
        }
        else {
            q.push(index);
        }
    }

    cout << "|\n";

    calculate(p);
}


// --------------------------------------------------
// Main
// --------------------------------------------------
int main() {

    int choice;

    do {

        cout << "\n=====================================\n";
        cout << "       CPU SCHEDULING ALGORITHMS\n";
        cout << "=====================================\n";

        cout << "1. FCFS\n";
        cout << "2. SJF (Non-Preemptive)\n";
        cout << "3. SRTF (Preemptive SJF)\n";
        cout << "4. Priority Scheduling\n";
        cout << "5. Round Robin\n";
        cout << "6. Run All Algorithms\n";
        cout << "0. Exit\n";

        cout << "\nEnter your choice: ";
        cin >> choice;

        switch (choice) {

            case 1:
                FCFS();
                break;

            case 2:
                SJF();
                break;

            case 3:
                SRTF();
                break;

            case 4:
                PriorityScheduling();
                break;

            case 5: {
                int quantum;
                cout << "Enter Time Quantum: ";
                cin >> quantum;
                RoundRobin(quantum);
                break;
            }

            case 6:
                FCFS();
                SJF();
                SRTF();
                PriorityScheduling();
                RoundRobin(2);
                break;

            case 0:
                cout << "\nProgram ended.\n";
                break;

            default:
                cout << "\nInvalid choice!\n";
        }

    } while (choice != 0);

    return 0;
}
