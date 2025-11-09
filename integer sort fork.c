#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void bubble_sort(int arr[], int n) {
    int i, j, temp;
    for (i = 0; i < n-1; i++) {
        for (j = 0; j < n-1-i; j++) {
            if (arr[j] > arr[j+1]) {
                temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}

void insertion_sort(int arr[], int n) {
    int i, key, j;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i-1;
        while (j >= 0 && arr[j] > key) {
            arr[j+1] = arr[j];
            j = j-1;
        }
        arr[j+1] = key;
    }
}

void print_array(const char *label, int arr[], int n) {
    printf("%s: ", label);
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main() {
    int n;
    printf("Enter number of integers: ");
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    int *arr = malloc(n * sizeof(int));
    if (!arr) {
        perror("malloc");
        return 1;
    }
    printf("Enter %d integers:\n", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        free(arr);
        return 1;
    }
    else if (pid == 0) {
        // Child process
        printf("[Child %d] Sorting in child using insertion sort\n", getpid());
        insertion_sort(arr, n);
        print_array("[Child] Sorted array", arr, n);

        // To demonstrate **orphan**, we make the child sleep
        // after parent ends. So we pause a bit.
        printf("[Child %d] Sleeping for 5 seconds to possibly become orphan...\n", getpid());
        sleep(5);
        printf("[Child %d] Child ends now.\n", getpid());

        free(arr);
        exit(0);
    }
    else {
        // Parent process
        printf("[Parent %d] Sorting in parent using bubble sort\n", getpid());
        bubble_sort(arr, n);
        print_array("[Parent] Sorted array", arr, n);

        // Demonstrate **zombie**: child exits quickly, we delay before wait() so child becomes zombie briefly
        printf("[Parent %d] Sleeping for 5 seconds before wait() to let child become zombie...\n", getpid());
        sleep(5);

        int status;
        pid_t w = wait(&status);
        if (w > 0) {
            printf("[Parent %d] wait() returned child pid %d, status %d\n", getpid(), w, status);
        }

        printf("[Parent %d] Parent ends now.\n", getpid());

        free(arr);
        // Parent ends, if child were still running, child becomes orphan (but in this case child sleeps then ends)
        return 0;
    }
}
