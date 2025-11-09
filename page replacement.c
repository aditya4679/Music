#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_PAGES 100
#define MAX_FRAMES 10

void fifo(int ref[], int n, int frames, int *faults) {
    int frameArr[MAX_FRAMES];
    int i, j, k = 0;
    for (i = 0; i < frames; i++) frameArr[i] = -1;
    *faults = 0;
    for (i = 0; i < n; i++) {
        int page = ref[i];
        int found = 0;
        for (j = 0; j < frames; j++) {
            if (frameArr[j] == page) {
                found = 1;
                break;
            }
        }
        if (found == 0) {
            frameArr[k] = page;
            k = (k + 1) % frames;
            (*faults)++;
        }
    }
}

void lru(int ref[], int n, int frames, int *faults) {
    int frameArr[MAX_FRAMES];
    int timeArr[MAX_FRAMES];
    int i, j, pos, minTime;
    for (i = 0; i < frames; i++) {
        frameArr[i] = -1;
        timeArr[i] = 0;
    }
    *faults = 0;
    for (i = 0; i < n; i++) {
        int page = ref[i];
        int found = 0;
        for (j = 0; j < frames; j++) {
            if (frameArr[j] == page) {
                found = 1;
                timeArr[j] = i;
                break;
            }
        }
        if (!found) {
            // Miss: find LRU slot
            minTime = INT_MAX;
            pos = 0;
            for (j = 0; j < frames; j++) {
                if (frameArr[j] == -1) {
                    pos = j;
                    break;
                }
                if (timeArr[j] < minTime) {
                    minTime = timeArr[j];
                    pos = j;
                }
            }
            frameArr[pos] = page;
            timeArr[pos] = i;
            (*faults)++;
        }
    }
}

void optimal(int ref[], int n, int frames, int *faults) {
    int frameArr[MAX_FRAMES];
    int i, j, k, pos, farthest;
    for (i = 0; i < frames; i++) frameArr[i] = -1;
    *faults = 0;
    for (i = 0; i < n; i++) {
        int page = ref[i];
        int found = 0;
        for (j = 0; j < frames; j++) {
            if (frameArr[j] == page) {
                found = 1;
                break;
            }
        }
        if (found == 0) {
            // need to replace
            for (j = 0; j < frames; j++) {
                if (frameArr[j] == -1) {
                    frameArr[j] = page;
                    (*faults)++;
                    found = 1;
                    break;
                }
            }
            if (!found) {
                farthest = i + 1;
                pos = 0;
                for (j = 0; j < frames; j++) {
                    int nextUse = INT_MAX;
                    for (k = i+1; k < n; k++) {
                        if (frameArr[j] == ref[k]) {
                            nextUse = k;
                            break;
                        }
                    }
                    if (nextUse > farthest) {
                        farthest = nextUse;
                        pos = j;
                    }
                }
                frameArr[pos] = page;
                (*faults)++;
            }
        }
    }
}

int main() {
    int n, i, frames = 3;
    int ref[MAX_PAGES];
    int faultsFIFO, faultsLRU, faultsOPT;

    printf("Enter number of pages in reference string: ");
    scanf("%d", &n);
    printf("Enter the reference string of %d pages:\n", n);
    for (i = 0; i < n; i++) {
        scanf("%d", &ref[i]);
    }
    printf("Using frame size = %d frames\n", frames);

    fifo(ref, n, frames, &faultsFIFO);
    lru(ref, n, frames, &faultsLRU);
    optimal(ref, n, frames, &faultsOPT);

    printf("\nResults:\n");
    printf("FIFO page faults     = %d\n", faultsFIFO);
    printf("LRU page faults      = %d\n", faultsLRU);
    printf("Optimal page faults  = %d\n", faultsOPT);

    return 0;
}
