// combined_shm_sync.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define SHM_SIZE       1024
#define SHM_KEY_FILE   "/tmp"
#define SHM_PROJ_ID    'S'

// Structure in shared memory: message + flag
struct shm_struct {
    int ready;           // flag: 0 = data not ready; 1 = data ready
    char message[SHM_SIZE - sizeof(int)];
};

int main(void) {
    key_t key;
    int shmid;
    struct shm_struct *shmptr;

    // Generate key
    key = ftok(SHM_KEY_FILE, SHM_PROJ_ID);
    if (key == (key_t)-1) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    // Create shared memory segment
    shmid = shmget(key, sizeof(struct shm_struct), IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    // Fork: parent is server, child is client
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        shmctl(shmid, IPC_RMID, NULL);
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Child process – client
        shmptr = (struct shm_struct *) shmat(shmid, NULL, 0);
        if (shmptr == (void *)-1) {
            perror("shmat (child)");
            exit(EXIT_FAILURE);
        }

        // Wait until server has written data
        while (shmptr->ready == 0) {
            usleep(10000);  // sleep 10 ms
        }

        printf("Client: read \"%s\" from shared memory.\n", shmptr->message);

        if (shmdt(shmptr) == -1) {
            perror("shmdt (child)");
            exit(EXIT_FAILURE);
        }

        exit(EXIT_SUCCESS);
    }
    else {
        // Parent process – server
        shmptr = (struct shm_struct *) shmat(shmid, NULL, 0);
        if (shmptr == (void *)-1) {
            perror("shmat (parent)");
            shmctl(shmid, IPC_RMID, NULL);
            exit(EXIT_FAILURE);
        }

        // Initialize flag
        shmptr->ready = 0;

        printf("Server: Enter a message: ");
        if (fgets(shmptr->message, sizeof(shmptr->message), stdin) == NULL) {
            perror("fgets");
        }
        // Remove newline if present
        shmptr->message[strcspn(shmptr->message, "\n")] = '\0';

        // Signal client
        shmptr->ready = 1;

        printf("Server: wrote \"%s\" into shared memory.\n", shmptr->message);

        // Wait for child to finish
        wait(NULL);

        if (shmdt(shmptr) == -1) {
            perror("shmdt (parent)");
        }
        if (shmctl(shmid, IPC_RMID, NULL) == -1) {
            perror("shmctl(IPC_RMID)");
        }

        printf("Server: shared memory removed, exiting.\n");
        exit(EXIT_SUCCESS);
    }
}
