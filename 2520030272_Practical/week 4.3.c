#include <stdio.h>

int main() {
    int n = 3;
    int at[] = {0, 0, 0};
    int bt[] = {5, 4, 3};
    int rt[] = {5, 4, 3};

    int ct[3], tat[3], wt[3];
    int quantum = 2;
    int time = 0;
    int completed = 0;

    printf("Round Robin Scheduling\n");
    printf("Time Quantum = %d\n\n", quantum);

    while (completed < n) {
        int executed = 0;

        for (int i = 0; i < n; i++) {
            if (rt[i] > 0) {
                executed = 1;

                if (rt[i] > quantum) {
                    time += quantum;
                    rt[i] -= quantum;
                } else {
                    time += rt[i];
                    rt[i] = 0;
                    ct[i] = time;
                    completed++;
                }
            }
        }

        if (!executed)
            break;
    }

    float avg_tat = 0, avg_wt = 0;

    printf("Process\tAT\tBT\tCT\tTAT\tWT\n");

    for (int i = 0; i < n; i++) {
        tat[i] = ct[i] - at[i];
        wt[i] = tat[i] - bt[i];

        avg_tat += tat[i];
        avg_wt += wt[i];

        printf("P%d\t%d\t%d\t%d\t%d\t%d\n",
               i + 1, at[i], bt[i], ct[i], tat[i], wt[i]);
    }

    printf("\nAverage Turnaround Time = %.2f\n", avg_tat / n);
    printf("Average Waiting Time = %.2f\n", avg_wt / n);

    printf("\nRound Robin Gantt Chart:\n");
    printf("| P1 | P2 | P3 | P1 | P2 | P3 | P1 |\n");
    printf("0    2    4    6    8    10   11   12\n");

    return 0;
}
