#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

void sort_array(int *arr, int n) {
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-1-i; j++) {
            if (arr[j] > arr[j+1]) {
                int tmp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = tmp;
            }
        }
    }
}

void print_reverse(int *arr, int n) {
    printf("Child: Received sorted array. Now printing in reverse order:\n");
    for (int i = n-1; i >= 0; i--) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    if (argc > 1) {
        // We are in the exec-invoked “child” mode: arguments are the sorted ints
        int n = argc - 1;
        int *arr = malloc(n * sizeof(int));
        if (!arr) { perror("malloc"); return EXIT_FAILURE; }
        for (int i = 0; i < n; i++) {
            arr[i] = atoi(argv[i+1]);
        }
        print_reverse(arr, n);
        free(arr);
        return EXIT_SUCCESS;
    }

    // Otherwise, initial run by parent
    int n;
    printf("Enter number of integers: ");
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    int *arr = malloc(n * sizeof(int));
    if (!arr) { perror("malloc"); return EXIT_FAILURE; }
    printf("Enter %d integers:\n", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    sort_array(arr, n);
    printf("Parent: sorted array:\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        free(arr);
        return EXIT_FAILURE;
    }
    else if (pid == 0) {
        // Child process: exec self with sorted array as args
        char **args = malloc((n + 2) * sizeof(char *));
        if (!args) { perror("malloc"); exit(EXIT_FAILURE); }
        args[0] = argv[0];  // same program name
        for (int i = 0; i < n; i++) {
            char *buf = malloc(20);
            if (!buf) { perror("malloc"); exit(EXIT_FAILURE); }
            sprintf(buf, "%d", arr[i]);
            args[i+1] = buf;
        }
        args[n+1] = NULL;

        execve(argv[0], args, NULL);
        perror("execve");
        exit(EXIT_FAILURE);
    }
    else {
        int status;
        wait(&status);
        free(arr);
        printf("Parent: child finished. Exiting.\n");
    }

    return EXIT_SUCCESS;
}
