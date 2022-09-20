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

#include "snoopy.h"
#include "entrypoint/test-cli.h"

#include "configuration.h"
#include "datasourceregistry.h"
#include "tsrm.h"

#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
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
void * snoopyTestCli_action_stress_threads_threadMain  (void *arg);
int    snoopyTestCli_action_stress_threads_randomNumberInclusive (int idMin, int idMax);



/*
 * Global variables
 */
pthread_t         snoopyTestCli_action_stress_threads_tRepo[THREAD_COUNT_MAX];
pthread_mutex_t   threadCountMutex = PTHREAD_MUTEX_INITIALIZER;
int               threadCountCreated  = 0; // Created threads, as seen by each thread
int               threadCountAliveNow = 0; // Number of threads currently alive
int               threadCountAliveMax = 0; // Maximum number of threads alive at any one time

int               verbose;



void snoopyTestCli_action_stress_threads_showHelp ()
{
    char * helpContent =
        "Snoopy TEST SUITE CLI utility :: Action `stress` :: Subsystem `threads`\n"
        "\n"
        "Usage:\n"
        "    snoopy-test stress threads THREAD_COUNT [-v]\n"
        "\n"
        "Description:\n"
        "    Stresses Snoopy's threading implementation by creating and destroying THREAD_COUNT\n"
        "    threads as fast as possible.\n"
        "\n"
        "Arguments:\n"
        "    THREAD_COUNT       Number of threads to create and destroy\n"
        "    -v                 Verbose debugging output\n"
        "\n"
        "Output:\n"
        "    Various threading-related messages and some statistics at the end.\n"
        "\n";
    printf("%s", helpContent);
}



int snoopyTestCli_action_stress_threads (int argc, char ** argv)
{
    int         threadsToCreate;
    int         maxConcurrentThreadsObserved = 0;
    int         retVal = 0;


    /* Check arguments and parse them */
    if (argc < 1) {
        snoopyTestCli_action_stress_threads_showHelp();
        fatalError("Missing argument: number of threads to create");
    }
    threadsToCreate = atoi(argv[0]);
    if ((threadsToCreate < 1) || (threadsToCreate > THREAD_COUNT_MAX)) {
        snoopyTestCli_action_stress_threads_showHelp();
        fatalError("Invalid number of threads to create (min 1, max THREAD_COUNT_MAX)");
    }

    if ((argc >= 2) && (0 == strcmp(argv[1], "-v"))) {
        verbose = 1;
    } else {
        verbose = 0;
    }


    // Disable config file parsing
    snoopy_configuration_preinit_disableConfigFileParsing();


    // Do not call snoopy_init() in the main thread here, as we're letting the non-main threads
    // figure it out for themselves.


    // Create threads and run the function in them
    printf("M: Starting threads... ");
    for (int i=0 ; i<threadsToCreate ; i++) {
        tData_t *tArgs = malloc(sizeof *tArgs);
        tArgs->seqNr   = i;
        if (verbose) printf(" M: Starting thread #%d:\n", i+1);
        retVal = pthread_create(&snoopyTestCli_action_stress_threads_tRepo[i], NULL, &snoopyTestCli_action_stress_threads_threadMain, tArgs);
    }
    printf("done.\n");
    printf("M: Threads alive right after thread creation was completed: %d\n", threadCountAliveNow);


    // Sleep a bit, and get thread count, should be max
    if (verbose) {
        struct timespec ts_sleep;
        ts_sleep.tv_sec = 0;
        ts_sleep.tv_nsec = 200000000;
        nanosleep(&ts_sleep, NULL);
        maxConcurrentThreadsObserved = snoopy_tsrm_get_threadCount();
        printf("M: Threads after first sleep: %d\n", maxConcurrentThreadsObserved);

        // Sleep a bit more for all threads to finish
        nanosleep(&ts_sleep, NULL);
        printf("M: Threads after all threads are supposedly finished: %d\n", snoopy_tsrm_get_threadCount());
    }

    // Wait for threads to finish
    printf("M: Waiting for all threads to finish... ");
    for (int i=0 ; i<threadsToCreate ; i++) {
        pthread_join(snoopyTestCli_action_stress_threads_tRepo[i], NULL);
        if (verbose) {
            printf("M: Thread joined: #%d\n", i+1);
            printf(" M: Thread #%d joined.\n", i+1);
        }
    }
    printf("done.\n");

    printf("M: Number of threads created:        %d\n", threadCountCreated);
    printf("M: Max threads alive simultaneously: %d\n", threadCountAliveMax);

    // This should return 1 thread still active
    if (verbose) printf("M: Threads after all threads, except main, have finished: %d\n", snoopy_tsrm_get_threadCount());
    // Why did we have snoopy_cleanup() commented out here?
    if (verbose) printf("M: Threads after all threads have finished: %d\n", snoopy_tsrm_get_threadCount());

    if (verbose) printf("SUCCESS! Expected Snoopy threads count reached: %d\n", maxConcurrentThreadsObserved);
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
void* snoopyTestCli_action_stress_threads_threadMain (void *args)
{
    tData_t  *tArgs    = args;

    int       seqNr    = tArgs->seqNr;
    int       seqNrPub = seqNr + 1;

    int       dsCount;
    int       dsId;
    char     *dsName;
    const char *dsArg = "";
    char      dsResult[SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE];
    int       retVal;


    // Initialize thread
    pthread_mutex_lock(&threadCountMutex);
    threadCountCreated++;
    threadCountAliveNow++;
    if (threadCountAliveNow > threadCountAliveMax) {
        threadCountAliveMax = threadCountAliveNow;
    }
    pthread_mutex_unlock(&threadCountMutex);


    // Hello the user
    if (verbose) printf("    t%d %llu : Hello from thread #%d\n", seqNrPub, (unsigned long long)pthread_self(), seqNrPub);


    // Initialize Snoopy
    if (verbose) printf("    t%d %llu : Threads before snoopy_init():    %d\n", seqNrPub, (unsigned long long)pthread_self(), snoopy_tsrm_get_threadCount());
    snoopy_entrypoint_test_cli_threads_init();
    if (verbose) printf("    t%d %llu : Threads after  snoopy_init():    %d\n", seqNrPub, (unsigned long long)pthread_self(), snoopy_tsrm_get_threadCount());


    // Run a random snoopy datasource
    dsCount = snoopy_datasourceregistry_getCount();
    dsId    = snoopyTestCli_action_stress_threads_randomNumberInclusive(0, dsCount-1);
    dsName  = snoopy_datasourceregistry_getName(dsId);
    retVal  = snoopy_datasourceregistry_callById(dsId, dsResult, dsArg);

    if (0 > retVal) {
        printf("    t%d %llu : Datasource %s returned negative result: %d\n", seqNrPub, (unsigned long long)pthread_self(), dsName, retVal);
    } else {
        printf("    t%d %llu : DS result: %30s = %s\n", seqNrPub, (unsigned long long)pthread_self(), dsName, dsResult);
    }


    // Retest at thread end
    if (verbose) printf("    t%d %llu : Threads before snoopy_cleanup(): %d\n", seqNrPub, (unsigned long long)pthread_self(), snoopy_tsrm_get_threadCount());
    snoopy_entrypoint_test_cli_exit();
    if (verbose) printf("    t%d %llu : Threads after  snoopy_cleanup(): %d\n", seqNrPub, (unsigned long long)pthread_self(), snoopy_tsrm_get_threadCount());


    // Goodbye to user
    if (verbose) printf("    t%d %llu : Thread exiting: #%d\n", seqNrPub, (unsigned long long)pthread_self(), seqNrPub);


    // Cleanup thread
    pthread_mutex_lock(&threadCountMutex);
    threadCountAliveNow--;
    pthread_mutex_unlock(&threadCountMutex);


    free (tArgs);
    return NULL;
}



/*
 * randomNumberInclusive()
 *
 * Description:
 *     Return random number between idMin and idMax inclusive
 *
 * Params:
 *
 * Return:
 *     int        Random number
 */
int snoopyTestCli_action_stress_threads_randomNumberInclusive (int nMin, int nMax)
{
    int           randomNrRaw = 0;
    int           randomNr;
    ssize_t       bytesRead = 0;
    unsigned char buffer[sizeof(randomNrRaw)];

    // Read the random content
    int fd = open("/dev/urandom", O_RDONLY);
    if (-1 == fd) {
        printf("ERROR: Unable to open /dev/urandom.\n");
        return -1; // Yeah, not the best error handling
    }
    bytesRead = read(fd, buffer, sizeof(randomNrRaw));
    close(fd);
    if (bytesRead != sizeof(randomNrRaw)) {
        // SonarCloud may complain about redundant casts here, but removing these
        // "redundant" casts causes builds to fail on non-64bit platforms.
        printf("ERROR: Unable to read %lu bytes from /dev/urandom, only got %li bytes.\n", (long unsigned) sizeof(randomNrRaw), (long int) bytesRead);
        return -1;
    }

    // Convert the read bytes to random positive number
    for (unsigned int i=0 ; i<sizeof(randomNrRaw) ; ++i) {
        randomNrRaw += (int) buffer[i] << i*8;
    }
    if (randomNrRaw < 0) {
        randomNrRaw = -randomNrRaw;
    }

    // Generate
    randomNr = ( randomNrRaw % (nMax - nMin + 1) ) + nMin;

    return randomNr;
}
