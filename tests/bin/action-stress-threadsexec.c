/*
 * SNOOPY LOGGER
 *
 * File: snoopy-test-datasource.c
 *
 * Copyright (c) 2015 Bostjan Skufca <bostjan@a2o.si>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */



/*
 * Includes order: from local to global
 */
#include "action-common.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>



#define   THREAD_COUNT_MAX   10000



/*
 * Thread data
 */
typedef struct {
    int seqNr;
} tData_t;



/*
 * We do not use separate .h file here
 */
void * snoopyTestCli_action_stress_threadsexec_threadMain  (void *arg);



/*
 * Global variables
 */
char      **snoopyTestCli_action_stress_threadsexec_runCmdAndArgv;
pthread_t   snoopyTestCli_action_stress_threadsexec_tRepo[THREAD_COUNT_MAX];






void snoopyTestCli_action_stress_threadsexec_showHelp ()
{
    char * helpContent =
        "Snoopy TEST SUITE CLI utility :: Action `stress` :: Subsystem `threadsexec`\n"
        "\n"
        "Usage:\n"
        "    snoopy-test stress threadsexec THREAD_COUNT CMD [CMD_ARGS]\n"
        "\n"
        "Description:\n"
        "    Stresses Snoopy's threading implementation by creating and destroying THREAD_COUNT\n"
        "    threads as fast as possible and executing CMD from those threads.\n"
        "\n"
        "Arguments:\n"
        "    THREAD_COUNT       Number of threads to create and destroy\n"
        "    CMD                External command to execute from each newly created thread\n"
        "    [CMD_ARGS]         Optional argument(s) for the external command\n"
        "\n"
        "Output:\n"
        "    Various threading-related messages are shown, followed by a word \"SUCCESS!\".\n"
        "\n";
    printf("%s", helpContent);
}



int snoopyTestCli_action_stress_threadsexec (int argc, char ** argv)
{
    int         threadsToCreate;
    int         i;
    int         retVal = 0;


    /* Check arguments and parse them */
    if (argc < 1) {
        snoopyTestCli_action_stress_threadsexec_showHelp();
        fatalError("Missing argument: number of threads to run");
    }
    threadsToCreate = atoi(argv[0]);
    if ((threadsToCreate < 1) || (threadsToCreate > THREAD_COUNT_MAX)) {
        snoopyTestCli_action_stress_threadsexec_showHelp();
        fatalErrorValue("Invalid number of threads to create (min 1, max THREAD_COUNT_MAX)", argv[0]);
    }

    if (argc < 2) {
        snoopyTestCli_action_stress_threadsexec_showHelp();
        fatalError("Missing argument: external command to run");
    }
    snoopyTestCli_action_stress_threadsexec_runCmdAndArgv = &argv[1];


    // Create threads and run the function in them
    printf("M: Starting threads:\n");
    for (i=0 ; i<threadsToCreate ; i++) {
        tData_t *tArgs = malloc(sizeof *tArgs);
        tArgs->seqNr   = i;
        printf(" M: Starting thread #%d:\n", i+1);
        retVal = pthread_create(&snoopyTestCli_action_stress_threadsexec_tRepo[i], NULL, &snoopyTestCli_action_stress_threadsexec_threadMain, tArgs);
    }
    printf("M: All threads started\n");

    // Wait for threads to finish
    printf("M: Waiting for all threads to finish:\n");
    for (i=0 ; i<threadsToCreate ; i++) {
        pthread_join(snoopyTestCli_action_stress_threadsexec_tRepo[i], NULL);
        printf(" M: Thread #%d joined.\n", i+1);
    }
    printf("M: All threads have finished.\n");

    /* Housekeeping and return */
    printf("SUCCESS!\n");
    return retVal;
}



/*
 * threadMain()
 *
 * Description:
 *     Main thread routine
 *
 * Params:
 *     errorMsg   Error message to display to user
 *
 * Return:
 *     int        Exit status to return to calling process
 */
void* snoopyTestCli_action_stress_threadsexec_threadMain (void *args)
{
    tData_t  *tArgs = args;
    char     *cmd;
    char    **argv;

    printf("  t%d : Hello from thread #%d.\n", tArgs->seqNr+1, tArgs->seqNr+1);


    // Fork
    pid_t pid = fork();

    if (pid > 0) {
        // Parent
        printf("  t%dp: Hello from parent proc\n", tArgs->seqNr+1);

        int * status = 0;
        waitpid(pid, status, 0);
        printf("  t%dp: Child proc has finished\n", tArgs->seqNr+1);

    } else if (pid == 0) {
        // Child
        printf("  t%dc: Hello from child proc\n", tArgs->seqNr+1);

        // Set variables
        cmd  = snoopyTestCli_action_stress_threadsexec_runCmdAndArgv[0];
        argv = &snoopyTestCli_action_stress_threadsexec_runCmdAndArgv[0];

        printf("  t%dc: running cmd %s %s\n", tArgs->seqNr+1, cmd, argv[0]);
        execv(cmd, argv);

    } else {
        // fork failed
        printf("  t%d : Fork failed!\n", tArgs->seqNr+1);
    }

    free (tArgs);
    return NULL;
}
