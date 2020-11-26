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

    // Copy the argv
    char** new_argv = malloc((argc) * sizeof *new_argv);
    for (int i=1; i < argc; ++i)
    {
        size_t length = strlen(argv[i])+1;
        new_argv[i-1] = malloc(length);
        memcpy(new_argv[i-1], argv[i], length);
    }
    new_argv[argc-1] = NULL;

    /* Spawn a child to run the program. */
    pid_t pid = fork();
    if (pid == -1) { /* fork failed */
        printf("fork() failed\n");
        exit(2);
    }

    if (pid == 0) { /* child process */
        //static char *argv[] = {
        //    "echo",
        //    "Foo is my name.",
        //    NULL
        //};
        //execv("/bin/echo", argv);
        execv(argv[1], new_argv);

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
