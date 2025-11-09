#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main() {
    int n, m, i, j, k;
    printf("Number of processes: ");
    scanf("%d", &n);
    printf("Number of resource types: ");
    scanf("%d", &m);

    int **Allocation = malloc(n * sizeof(int *));
    int **Max        = malloc(n * sizeof(int *));
    int **Need       = malloc(n * sizeof(int *));
    int *Available   = malloc(m * sizeof(int));
    bool  *Finish    = malloc(n * sizeof(bool));
    int *SafeSeq     = malloc(n * sizeof(int));

    for (i = 0; i < n; i++) {
        Allocation[i] = malloc(m * sizeof(int));
        Max[i]        = malloc(m * sizeof(int));
        Need[i]       = malloc(m * sizeof(int));
    }

    printf("Enter Allocation matrix (n × m):\n");
    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++) {
            scanf("%d", &Allocation[i][j]);
        }
    }

    printf("Enter Max matrix (n × m):\n");
    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++) {
            scanf("%d", &Max[i][j]);
        }
    }

    printf("Enter Available resources (m values):\n");
    for (j = 0; j < m; j++) {
        scanf("%d", &Available[j]);
    }

    // Calculate Need = Max – Allocation
    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++) {
            Need[i][j] = Max[i][j] - Allocation[i][j];
        }
    }

    // Initialize Finish[i] = false
    for (i = 0; i < n; i++) {
        Finish[i] = false;
    }

    int count = 0;
    int *Work = malloc(m * sizeof(int));
    for (j = 0; j < m; j++) {
        Work[j] = Available[j];
    }

    // Safety algorithm
    while (count < n) {
        bool found = false;
        for (i = 0; i < n; i++) {
            if (!Finish[i]) {
                bool canAllocate = true;
                for (j = 0; j < m; j++) {
                    if (Need[i][j] > Work[j]) {
                        canAllocate = false;
                        break;
                    }
                }
                if (canAllocate) {
                    // pretend allocate
                    for (j = 0; j < m; j++) {
                        Work[j] += Allocation[i][j];
                    }
                    SafeSeq[count++] = i;
                    Finish[i] = true;
                    found = true;
                }
            }
        }
        if (!found) {
            break;
        }
    }

    if (count == n) {
        printf("System is in a SAFE state.\nSafe sequence is: ");
        for (i = 0; i < n; i++) {
            printf("P%d ", SafeSeq[i]);
        }
        printf("\n");
    } else {
        printf("System is NOT in a safe state — deadlock may occur.\n");
    }

    // Free memory
    for (i = 0; i < n; i++) {
        free(Allocation[i]);
        free(Max[i]);
        free(Need[i]);
    }
    free(Allocation);
    free(Max);
    free(Need);
    free(Available);
    free(Work);
    free(Finish);
    free(SafeSeq);

    return 0;
}
