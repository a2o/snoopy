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
#define   _XOPEN_SOURCE   500
#define   _GNU_SOURCE

#include <snoopy.h>

#include <configuration.h>
#include <datasourceregistry.h>
#include <misc.h>
#include <tsrm.h>

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>



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
int    main        (int argc, char **argv);
void   displayHelp ();
int    fatalError  (char *errorMsg);
void * threadMain  (void *arg);
int    randomNumberInclusive (int idMin, int idMax);



/*
 * Global variables
 */
char            **runCmdAndArgv;
pthread_t         tRepo[THREAD_COUNT_MAX];
pthread_mutex_t   threadCountMutex = PTHREAD_MUTEX_INITIALIZER;
int               threadCountCreated  = 0; // Created threads, as seen by each thread
int               threadCountAliveNow = 0; // Number of threads currently alive
int               threadCountAliveMax = 0; // Maximum number of threads alive at any one time



int main (int argc, char **argv)
{
    int         threadsToCreate;
    int         i;
    int         retVal = 0;


    /* Check arguments and parse them */
    if (argc < 2) {
        displayHelp();
        return fatalError("Missing argument: number of threads to create");
    }
    threadsToCreate = atoi(argv[1]);
    if ((threadsToCreate < 1) || (threadsToCreate > THREAD_COUNT_MAX)) {
        displayHelp();
        return fatalError("Invalid number of threads to create (min 1, max THREAD_COUNT_MAX)");
    }


    // Disable config file parsing
    snoopy_configuration_preinit_disableConfigFileParsing();


    // Do the initial Snoopy init in main thread
    // - left out intentionally, let the child threads figure it out for themselves!
//    printf("M: Threads before initial snoopy_init(): %d\n", snoopy_tsrm_get_threadCount());
//    snoopy_init();
//    printf("M: Threads after  initial snoopy_init(): %d\n", snoopy_tsrm_get_threadCount());


    // Create threads and run the function in them
    printf("M: Starting threads... ");
    for (i=0 ; i<threadsToCreate ; i++) {
        tData_t *tArgs = malloc(sizeof *tArgs);
        tArgs->seqNr   = i;
//        printf(" M: Starting thread #%d:\n", i+1);
        retVal = pthread_create(&tRepo[i], NULL, &threadMain, tArgs);
    }
    printf("done.\n");
    printf("M: Threads alive right after thread creation was completed: %d\n", threadCountAliveNow);


    // Sleep a bit, and get thread count, should be max
//    usleep(200000);
//    maxThreadsSeen = snoopy_tsrm_get_threadCount();
//    printf("M: Threads after first sleep: %d\n", maxThreadsSeen);

    // Sleep a bit more for all threads to finish
//    usleep(200000);
//    printf("M: Threads after all threads are supposedly finished: %d\n", snoopy_tsrm_get_threadCount());

    // Wait for threads to finish
    printf("M: Waiting for all threads to finish... ");
    for (i=0 ; i<threadsToCreate ; i++) {
        pthread_join(tRepo[i], NULL);
//        printf("M: Thread joined: #%d\n", i+1);
//        printf(" M: Thread #%d joined.\n", i+1);
    }
    printf("done.\n");

    printf("M: Number of threads created:        %d\n", threadCountCreated);
    printf("M: Max threads alive simultaneously: %d\n", threadCountAliveMax);

    // This should return 1 thread still active
//    printf("M: Threads after all threads, except main, have finished: %d\n", snoopy_tsrm_get_threadCount());

    // This should return 0 threads still active
//    snoopy_cleanup();
//    printf("M: Threads after all threads have finished: %d\n", snoopy_tsrm_get_threadCount());


    /* Evaluate and return */
//    if (maxThreadsSeen != threadsToCreate) {
//        printf("ERROR: Expected max Snoopy threads count was %d, but actual number was %d\n", threadsToCreate, maxThreadsSeen);
//        return 1;
//    }

//    printf("SUCCESS! Expected Snoopy threads count reached: %d\n", maxThreadsSeen);
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
void* threadMain (void *args)
{
    tData_t  *tArgs    = args;

    int       seqNr    = tArgs->seqNr;
    int       seqNrPub = seqNr + 1;

    int       dsCount;
    int       dsId;
    char     *dsName;
    char     *dsArg = "";
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
//    printf("    t%d %llu : Hello from thread #%d\n", seqNrPub, (unsigned long long)pthread_self(), seqNrPub);


    // Initialize Snoopy
//    printf("    t%d %llu : Threads before snoopy_init():    %d\n", seqNrPub, (unsigned long long)pthread_self(), snoopy_tsrm_get_threadCount());
    snoopy_init();
//    printf("    t%d %llu : Threads after  snoopy_init():    %d\n", seqNrPub, (unsigned long long)pthread_self(), snoopy_tsrm_get_threadCount());


    // Run a random snoopy datasource
    dsCount = snoopy_datasourceregistry_getCount();
    dsId    = randomNumberInclusive(0, dsCount-1);
    dsName  = snoopy_datasourceregistry_getName(dsId);
    retVal  = snoopy_datasourceregistry_callById(dsId, dsResult, dsArg);

    if (0 > retVal) {
        printf("    t%d %llu : Datasource %s returned negative result: %d\n", seqNrPub, (unsigned long long)pthread_self(), dsName, retVal);
    } else {
        printf("    t%d %llu : DS result: %30s = %s\n", seqNrPub, (unsigned long long)pthread_self(), dsName, dsResult);
    }


    // Retest at thread end
//    printf("    t%d %llu : Threads before snoopy_cleanup(): %d\n", seqNrPub, (unsigned long long)pthread_self(), snoopy_tsrm_get_threadCount());
    snoopy_cleanup();
//    printf("    t%d %llu : Threads after  snoopy_cleanup(): %d\n", seqNrPub, (unsigned long long)pthread_self(), snoopy_tsrm_get_threadCount());


    // Goodbye to user
//    printf("    t%d %llu : Thread exiting: #%d\n", seqNrPub, (unsigned long long)pthread_self(), seqNrPub);


    // Cleanup thread
    pthread_mutex_lock(&threadCountMutex);
    threadCountAliveNow--;
    pthread_mutex_unlock(&threadCountMutex);


    free (tArgs);
    return NULL;
}



/*
 * displayHelp()
 *
 * Description:
 *     Displays help
 *
 * Params:
 *     (none)
 *
 * Return:
 *     void
 */
void displayHelp ()
{
    printf("\n");
    printf("Usage: \n");
    printf("    snoopy-test-threads   THREAD_COUNT\n");
    printf("\n");
}



/*
 * fatalError()
 *
 * Description:
 *     Displays error message + help and returns non-zero exit status
 *
 * Params:
 *     errorMsg   Error message to display to user
 *
 * Return:
 *     int        Exit status to return to calling process
 */
int fatalError (char *errorMsg)
{
    printf("ERROR: %s\n", errorMsg);
    printf("\n");
    return 127;
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
int randomNumberInclusive (int nMin, int nMax)
{
    unsigned int rndSeed;
    int          randomNr;

    // Seed
    rndSeed = (unsigned int) (pthread_self() + getpid() + syscall(SYS_gettid) + time(NULL));
    srand(rndSeed);

    // Generate
    randomNr = ( rand() % (nMax - nMin + 1) ) + nMin;

    return randomNr;
}
