/*
 * SNOOPY COMMAND LOGGER
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

#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>



#define   THREAD_COUNT_MAX   10000



/*
 * Thread data
 */
typedef struct {
    int seqNr;
    int verbose;
    int joined;
} tData_t;



/*
 * We do not use separate .h file here
 */
void * snoopyTestCli_action_stress_threadsexec_threadMain  (const void *args);



/*
 * Global variables
 */
char      **snoopyTestCli_action_stress_threadsexec_runCmdAndArgv;
pthread_t   snoopyTestCli_action_stress_threadsexec_tRepo[THREAD_COUNT_MAX];



tData_t    *tArgsRepo[THREAD_COUNT_MAX];



void snoopyTestCli_action_stress_threadsexec_showHelp ()
{
    char * helpContent =
        "Snoopy TEST SUITE CLI utility :: Action `stress` :: Subsystem `threadsexec`\n"
        "\n"
        "Usage:\n"
        "    snoopy-test stress threadsexec [-v] THREAD_COUNT CMD [CMD_ARGS]\n"
        "\n"
        "Description:\n"
        "    Stresses Snoopy's threading implementation by creating and destroying THREAD_COUNT\n"
        "    threads as fast as possible and executing CMD from those threads.\n"
        "\n"
        "Arguments:\n"
        "    -v                 Verbose mode (dangerous - using stdout while threading and forking can result in a deadlock)\n"
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
    int         verbose = 0;
    int         threadsToCreate;
    int         retVal = 0;


    /* Check arguments and parse them */
    if (argc < 1) {
        snoopyTestCli_action_stress_threadsexec_showHelp();
        fatalError("Missing argument: number of threads to run");
    }

    /* Check for the -v flag - this is a bad way to avoid using getopt() */
    if (strcmp(argv[0], "-v") == 0) {
        verbose = 1;
        argc--;
        argv = &argv[1];
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


    printf("M: Allocating memory for each thread's args:\n");
    for (int i=0 ; i<threadsToCreate ; i++) {
        tData_t *tData = malloc(sizeof(tData_t));
        tData->seqNr   = i;
        tData->verbose = verbose;
        tData->joined  = 0;
        tArgsRepo[i]   = tData;
    }


    // Create threads and run the function in them
    printf("M: Starting threads:\n");
    if (!verbose) {
        printf("M: From now on, the output supressed (use the dangerous -v flag to show it).\n");
    }

    for (int i=0 ; i<threadsToCreate ; i++) {
        if (verbose) printf(" M: Starting thread #%d:\n", i+1);
        retVal = pthread_create(&snoopyTestCli_action_stress_threadsexec_tRepo[i], NULL, (void *(*) (void *)) snoopyTestCli_action_stress_threadsexec_threadMain, tArgsRepo[i]);
    }
    if (verbose) printf("M: All threads started\n");

    // Wait for threads to finish
    int pMax = threadsToCreate+10;
    const struct timespec sleepTime = {
        .tv_sec = 0,
        .tv_nsec = 100000000,
    };

    // SonarCloud may complain about redundant casts here, but removing these
    // "redundant" casts causes builds to fail on non-64bit platforms.
    if (verbose) printf("M: Waiting for all threads to finish (max %d * %lld ns):\n", pMax, (long long int) sleepTime.tv_nsec);
    if (verbose) fflush(stdout);
    int p;
    for (p=1 ; p<pMax ; p++) {
        if (verbose) printf(" M: Thread join loop pass #%d start:\n", p);
        if (verbose) fflush(stdout);

        int threadsJoined = 0;
        for (int i=0 ; i<threadsToCreate ; i++) {
            if (tArgsRepo[i]->joined == 1) {
                threadsJoined++;
            } else {
                int res;

                res = pthread_tryjoin_np(snoopyTestCli_action_stress_threadsexec_tRepo[i], NULL);
                if (res == EBUSY) {
                    if (verbose) printf("  M: Thread #%d has not finished executing yet.\n", i+1);
                    if (verbose) fflush(stdout);
                } else {
                    if (verbose) printf("  M: Thread #%d joined.\n", i+1);
                    if (verbose) fflush(stdout);
                    tArgsRepo[i]->joined = 1;
                }
            }
        }
        if (verbose) printf(" M: Thread join loop pass #%d done.\n", p);

        if (threadsJoined == threadsToCreate) {
            if (verbose) printf("M: All threads have joined, exiting the wait loop.\n");
            break;
        }

        // SonarCloud may complain about redundant casts here, but removing these
        // "redundant" casts causes builds to fail on non-64bit platforms.
        if (verbose) printf(" M: Not all threads have joined yet, sleeping for %lld ns now.\n", (long long int) sleepTime.tv_nsec);
        if (verbose) fflush(stdout);
        nanosleep(&sleepTime, NULL);
    }

    if (p >= pMax) {
        // SonarCloud may complain about redundant casts here, but removing these
        // "redundant" casts causes builds to fail on non-64bit platforms.
        printf("M: ERROR - Not all threads have joined (timeout after %d * %lld ns).\n", pMax, (long long int) sleepTime.tv_nsec);
        return 1;
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
void* snoopyTestCli_action_stress_threadsexec_threadMain (const void *args)
{
    const tData_t *tArgs = args;
    char     *cmd;
    char    **argv;

    if (tArgs->verbose) printf("  t%d : Hello from thread #%d.\n", tArgs->seqNr+1, tArgs->seqNr+1);


    // Fork
    pid_t pid = fork();

    if (pid > 0) {
        // Parent
        if (tArgs->verbose) printf("  t%dp: Hello from parent proc\n", tArgs->seqNr+1);

        int * status = 0;
        waitpid(pid, status, 0);
        if (tArgs->verbose) printf("  t%dp: Child proc has finished\n", tArgs->seqNr+1);

    } else if (pid == 0) {
        // Child
        if (tArgs->verbose) printf("  t%dc: Hello from child proc\n", tArgs->seqNr+1);

        // Set variables
        cmd  = snoopyTestCli_action_stress_threadsexec_runCmdAndArgv[0];
        argv = &snoopyTestCli_action_stress_threadsexec_runCmdAndArgv[0];

        if (tArgs->verbose) printf("  t%dc: running cmd %s %s\n", tArgs->seqNr+1, cmd, argv[0]);
        execv(cmd, argv);

    } else {
        // fork failed
        if (tArgs->verbose) printf("  t%d : Fork failed!\n", tArgs->seqNr+1);
    }

    return NULL;
}
