#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> /* for fork */
#include <sys/types.h> /* for pid_t */
#include <sys/wait.h> /* for wait */

int main (int argc, char **argv)
{
    if (argc < 2) {
        printf("Pass command to run as an argument: \"./space parent\" /cmd/to/run [args]\n");
        exit(1);
    }

    /* Spawn a child to run the program. */
    pid_t pid = fork();
    if (pid == -1) { /* fork failed */
        printf("fork() failed\n");
        exit(2);
    }

    if (pid == 0) { /* child process */
        execv(argv[1], &argv[1]);

        /* This only gets executed if execv fails */
        printf("execv() failed\n");
        exit(3);

    } else { /* pid!=0; parent process */
        int status;
        waitpid(pid, &status, 0); /* wait for child to exit */
        if (status != 0) {
            printf("Child regurned with a non-zero status: %d\n", status);
            exit(4);
        }
    }
}
