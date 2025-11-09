#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

void preemptiveSJF(int n, int arrival[], int burst[]) {
    int i, completed = 0, current_time = 0;
    int *rem = malloc(n * sizeof(int));
    int *completion = malloc(n * sizeof(int));
    int *turnaround = malloc(n * sizeof(int));
    int *waiting = malloc(n * sizeof(int));

    for (i = 0; i < n; i++) {
        rem[i] = burst[i];
    }

    while (completed < n) {
        int min_rem = INT_MAX, shortest = -1;
        for (i = 0; i < n; i++) {
            if (arrival[i] <= current_time && rem[i] > 0 && rem[i] < min_rem) {
                min_rem = rem[i];
                shortest = i;
            }
        }
        if (shortest == -1) {
            current_time++;
            continue;
        }

        rem[shortest]--;
        current_time++;

        if (rem[shortest] == 0) {
            completed++;
            completion[shortest] = current_time;
            turnaround[shortest] = completion[shortest] - arrival[shortest];
            waiting[shortest] = turnaround[shortest] - burst[shortest];
        }
    }

    printf("\n--- Preemptive SJF Results ---\n");
    printf("P#\tAT\tBT\tCT\tTAT\tWT\n");
    double sumTAT = 0, sumWT = 0;
    for (i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\n", i+1, arrival[i], burst[i],
               completion[i], turnaround[i], waiting[i]);
        sumTAT += turnaround[i];
        sumWT += waiting[i];
    }
    printf("Average Turnaround Time = %.2f\n", sumTAT/n);
    printf("Average Waiting Time    = %.2f\n", sumWT/n);

    free(rem); free(completion); free(turnaround); free(waiting);
}

void roundRobin(int n, int arrival[], int burst[], int quantum) {
    int i, current_time = 0, completed = 0;
    int *rem = malloc(n * sizeof(int));
    int *completion = malloc(n * sizeof(int));
    int *turnaround = malloc(n * sizeof(int));
    int *waiting = malloc(n * sizeof(int));
    int *inQueue = calloc(n, sizeof(int));
    int *queue = malloc(n * sizeof(int));
    int front = 0, rear = 0;

    for (i = 0; i < n; i++) {
        rem[i] = burst[i];
    }

    // Enqueue first arrived processes at time 0
    for (i = 0; i < n; i++) {
        if (arrival[i] <= current_time && inQueue[i] == 0) {
            queue[rear++] = i;
            inQueue[i] = 1;
        }
    }

    while (completed < n) {
        if (front == rear) {
            current_time++;
            for (i = 0; i < n; i++) {
                if (arrival[i] <= current_time && inQueue[i] == 0 && rem[i] > 0) {
                    queue[rear++] = i;
                    inQueue[i] = 1;
                }
            }
            continue;
        }

        int idx = queue[front++];
        if (front == n) front = 0;

        if (rem[idx] > 0) {
            if (rem[idx] > quantum) {
                current_time += quantum;
                rem[idx] -= quantum;
            } else {
                current_time += rem[idx];
                rem[idx] = 0;
                completed++;
                completion[idx] = current_time;
                turnaround[idx] = completion[idx] - arrival[idx];
                waiting[idx] = turnaround[idx] - burst[idx];
            }

            for (i = 0; i < n; i++) {
                if (arrival[i] <= current_time && inQueue[i] == 0 && rem[i] > 0) {
                    queue[rear++] = i;
                    inQueue[i] = 1;
                }
            }

            if (rem[idx] > 0) {
                queue[rear++] = idx;
            }
        }
    }

    printf("\n--- Round Robin Results (Quantum = %d) ---\n", quantum);
    printf("P#\tAT\tBT\tCT\tTAT\tWT\n");
    double sumTAT = 0, sumWT = 0;
    for (i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\n", i+1, arrival[i], burst[i],
               completion[i], turnaround[i], waiting[i]);
        sumTAT += turnaround[i];
        sumWT += waiting[i];
    }
    printf("Average Turnaround Time = %.2f\n", sumTAT/n);
    printf("Average Waiting Time    = %.2f\n", sumWT/n);

    free(rem); free(completion); free(turnaround); free(waiting);
    free(inQueue); free(queue);
}

int main() {
    int choice, n, i, quantum;
    printf("CPU Scheduling Algorithms\n");
    printf("1. Preemptive Shortest Job First (SJF)\n");
    printf("2. Round Robin (with different arrival times)\n");
    printf("Enter your choice (1 or 2): ");
    scanf("%d", &choice);

    printf("Enter number of processes: ");
    scanf("%d", &n);
    int *arrival = malloc(n * sizeof(int));
    int *burst   = malloc(n * sizeof(int));

    for (i = 0; i < n; i++) {
        printf("Process %d — Arrival time: ", i+1);
        scanf("%d", &arrival[i]);
        printf("Process %d — Burst time:   ", i+1);
        scanf("%d", &burst[i]);
    }

    if (choice == 1) {
        preemptiveSJF(n, arrival, burst);
    } else if (choice == 2) {
        printf("Enter Time Quantum: ");
        scanf("%d", &quantum);
        roundRobin(n, arrival, burst, quantum);
    } else {
        printf("Invalid choice\n");
    }

    free(arrival); free(burst);
    return 0;
}
