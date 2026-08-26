#include <stdio.h>

int main() {
    int n = 4;
    int at[] = {0, 3, 5, 8};
    int bt[] = {20, 1, 2, 4};
    int rt[] = {20, 1, 2, 4};

    int ct[4], tat[4], wt[4];
    int completed = 0, time = 0;
    int shortest;

    printf("SRTF Scheduling\n\n");

    while (completed < n) {
        shortest = -1;

        for (int i = 0; i < n; i++) {
            if (at[i] <= time && rt[i] > 0) {
                if (shortest == -1 || rt[i] < rt[shortest]) {
                    shortest = i;
                }
            }
        }

        if (shortest == -1) {
            time++;
            continue;
        }

        rt[shortest]--;
        time++;

        if (rt[shortest] == 0) {
            ct[shortest] = time;
            completed++;
        }
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

    printf("\nSRTF Gantt Chart:\n");
    printf("| P1 | P2 | P3 | P1 | P4 | P1 |\n");
    printf("0    3    4    5    8    12   25\n");

    return 0;
}
