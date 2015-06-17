/*
 * SNOOPY LOGGER
 *
 * File: tsrm.c
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
#define _XOPEN_SOURCE 700   // Required by pthread_mutex_setattr

#include "tsrm.h"

#include "snoopy.h"

#include "configuration.h"
#include "inputdatastorage.h"

#include "lib/liblcthw/src/list.h"

#include <pthread.h>



/*
 * Global variables
 */
pthread_once_t       snoopy_tsrm_init_onceControl = PTHREAD_ONCE_INIT;
pthread_mutex_t      snoopy_tsrm_threadRepo_mutex;
pthread_mutexattr_t  snoopy_tsrm_threadRepo_mutexAttr;
List                 snoopy_tsrm_threadRepo_data = {
    .first = NULL,
    .last  = NULL,
    .count = 0,
};
List                *snoopy_tsrm_threadRepo = &snoopy_tsrm_threadRepo_data;



/*
 * Non-exported function prototypes
 */
void                        snoopy_tsrm_init                      ();
int                         snoopy_tsrm_doesThreadRepoEntryExist  (snoopy_tsrm_threadId_t threadId);
snoopy_tsrm_threadId_t      snoopy_tsrm_getCurrentThreadId        ();
ListNode*                   snoopy_tsrm_getCurrentThreadRepoEntry ();
snoopy_tsrm_threadData_t*   snoopy_tsrm_getCurrentThreadData      ();
snoopy_tsrm_threadData_t*   snoopy_tsrm_createNewThreadData       (snoopy_tsrm_threadId_t threadId);



/*
 * snoopy_tsrm_ctor
 *
 * Description:
 *     Initialize threadRepo list.
 *
 * Params:
 *     (none)
 *
 * Return:
 *     void
 */
void snoopy_tsrm_ctor ()
{
    snoopy_tsrm_threadId_t      curTid;
    snoopy_tsrm_threadData_t   *tData;

    // Initialize threading support
    pthread_once(&snoopy_tsrm_init_onceControl, snoopy_tsrm_init);

    // Get my thread id - before mutex, no need for mutex here
    curTid = snoopy_tsrm_getCurrentThreadId();

    // Mutex START
    pthread_mutex_lock(&snoopy_tsrm_threadRepo_mutex);

    // Create my entry if it does not exist
    if (SNOOPY_FALSE == snoopy_tsrm_doesThreadRepoEntryExist(curTid)) {
        tData = snoopy_tsrm_createNewThreadData(curTid);
        List_push(snoopy_tsrm_threadRepo, tData);
    }

    // Mutex END
    pthread_mutex_unlock(&snoopy_tsrm_threadRepo_mutex);
}



/*
 * snoopy_tsrm_dtor
 *
 * Description:
 *     Removes own entry from threadRepo, or removes threadRepo altogether.
 *
 * Params:
 *     (none)
 *
 * Return:
 *     void
 */
void snoopy_tsrm_dtor ()
{
    ListNode                   *tRepoEntry;
    snoopy_tsrm_threadData_t   *tData;

    // Get my thread data structure
    tRepoEntry = snoopy_tsrm_getCurrentThreadRepoEntry();
    if (NULL == tRepoEntry) {
        // This is an error condition, but let's not free NULLs below
        return;
    }

    // Mutex START
    pthread_mutex_lock(&snoopy_tsrm_threadRepo_mutex);

    // Remove from repo
    tData = List_remove(snoopy_tsrm_threadRepo, tRepoEntry);

    // Mutex END
    pthread_mutex_unlock(&snoopy_tsrm_threadRepo_mutex);

    // Free the results AFTER my node has been removed from repo
    free(tData->inputdatastorage);
    free(tData->configuration);
    free(tData);

    return;
}



/*
 * snoopy_tsrm_init
 *
 * Description:
 *     Initialize threading subsystem. This function should be run
 *     only once, by only one thread.
 *
 * Params:
 *     (none)
 *
 * Return:
 *     void
 */
void snoopy_tsrm_init ()
{
    // Initialize threadRepo mutex
    pthread_mutexattr_init   (&snoopy_tsrm_threadRepo_mutexAttr);
    pthread_mutexattr_settype(&snoopy_tsrm_threadRepo_mutexAttr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init       (&snoopy_tsrm_threadRepo_mutex, &snoopy_tsrm_threadRepo_mutexAttr);
}



/*
 * snoopy_tsrm_doesThreadRepoEntryExist
 *
 * Description:
 *     Check if thread with id threadId already has an entry in Snoopy's thread
 *     repository.
 *
 * Params:
 *     snoopy_tsrm_threadId_t:   Thread ID to look for
 *
 * Return:
 *     int:   SNOOPY_TRUE if yes, SNOOPY_FALSE if not
 */
int   snoopy_tsrm_doesThreadRepoEntryExist (snoopy_tsrm_threadId_t threadId)
{
    snoopy_tsrm_threadData_t   *tData;
    int                         retVal = SNOOPY_FALSE;

    // Mutex START
    pthread_mutex_lock(&snoopy_tsrm_threadRepo_mutex);

    LIST_FOREACH(snoopy_tsrm_threadRepo, first, next, cur) {
        if (NULL == cur->value) {
            continue;
        }
        tData = cur->value;
        if (0 != pthread_equal(threadId, tData->threadId)) {

            /* Thread ID matches */
            retVal = SNOOPY_TRUE;
            goto FOUND;
        }
    }

FOUND:
    // Mutex END
    pthread_mutex_unlock(&snoopy_tsrm_threadRepo_mutex);

    return retVal;
}



/*
 * snoopy_tsrm_createNewThreadData
 *
 * Description:
 *     Mallocs space and creates new thread data structure, for given thread ID.
 *
 * Params:
 *     snoopy_tsrm_threadId_t:   Thread ID to create entry for
 *
 * Return:
 *     snoopy_tsrm_threadData_t*:   Pointer to newly-created threadData structure
 */
snoopy_tsrm_threadData_t*   snoopy_tsrm_createNewThreadData (snoopy_tsrm_threadId_t threadId)
{
    snoopy_tsrm_threadData_t    *tData;

    // Allocte storage memory for new threadData structure
    tData                   = malloc(sizeof(snoopy_tsrm_threadData_t));
    tData->configuration    = malloc(sizeof(snoopy_configuration_t));
    tData->inputdatastorage = malloc(sizeof(snoopy_inputdatastorage_t));

    // Store thread ID
    tData->threadId         = threadId;

    // Initialize empty values
    snoopy_configuration_setUninitialized   (tData->configuration);
    snoopy_inputdatastorage_setUninitialized(tData->inputdatastorage);

    return tData;
}



/*
 * snoopy_tsrm_getCurrentThreadId
 *
 * Description:
 *     Retrieve id of current thread
 *
 * Params:
 *     (none)
 *
 * Return:
 *     snoopy_tsrm_threadId_t:   Current thread id
 */
snoopy_tsrm_threadId_t   snoopy_tsrm_getCurrentThreadId ()
{
    return pthread_self();
}



/*
 * snoopy_tsrm_getCurrentThreadRepoEntry
 *
 * Description:
 *     Retrieve pointer to threadRepo list node structure of current thread.
 *
 * Params:
 *     (none)
 *
 * Return:
 *     ListNode*:   Current threadRepo entry, or null if it does not exist
 */
ListNode*   snoopy_tsrm_getCurrentThreadRepoEntry ()
{
    snoopy_tsrm_threadId_t      myThreadId;
    ListNode                   *tRepoEntry = NULL;
    snoopy_tsrm_threadData_t   *tData;

    // This is the thread ID we are looking for
    myThreadId = snoopy_tsrm_getCurrentThreadId();

    // Mutex START
    pthread_mutex_lock(&snoopy_tsrm_threadRepo_mutex);

    LIST_FOREACH(snoopy_tsrm_threadRepo, first, next, cur) {

        // This should not happen, but maybe, just maybe, there is another thread
        // that is just creating new entry.
        if (NULL == cur->value) {
            continue;
        }
        tData = cur->value;

        if (0 != pthread_equal(myThreadId, tData->threadId)) {
            tRepoEntry = cur;
            goto FOUND;
        }
    }

FOUND:
    // Mutex END
    pthread_mutex_unlock(&snoopy_tsrm_threadRepo_mutex);

    // Return
    return tRepoEntry;
}



/*
 * snoopy_tsrm_getCurrentThreadData
 *
 * Description:
 *     Retrieve pointer to thread data structure of current thread. Creates
 *     a new repo entry if it does not exist.
 *
 * Params:
 *     (none)
 *
 * Return:
 *     snoopy_tsrm_threadData_t:   Current threadRepo list entry, or NULL
 */
snoopy_tsrm_threadData_t*   snoopy_tsrm_getCurrentThreadData ()
{
    ListNode   *tRepoEntry;

    tRepoEntry = snoopy_tsrm_getCurrentThreadRepoEntry();
    if (NULL == tRepoEntry) {
        return NULL;
    }

    return tRepoEntry->value;
}



/*
 * snoopy_tsrm_get_configuration()
 *
 * Description:
 *     Retrieve configuration array pointer for current thread.
 *     This function is to be consumed by the rest of Snoopy.
 *
 * Params:
 *     (none)
 *
 * Return:
 *     snoopy_configuration_t*:   Pointer to thread-specific Snoopy configuration data structure
 */
snoopy_configuration_t*   snoopy_tsrm_get_configuration ()
{
    snoopy_tsrm_threadData_t   *tData;

    tData = snoopy_tsrm_getCurrentThreadData();

    return tData->configuration;
}



/*
 * snoopy_tsrm_get_inputdatastorage()
 *
 * Description:
 *     Retrieve input data storage array pointer for current thread.
 *     This function is to be consumed by the rest of Snoopy.
 *
 * Params:
 *     (none)
 *
 * Return:
 *     snoopy_inputdatastorage_t*:   Pointer to thread-specific input data storage structure
 */
snoopy_inputdatastorage_t*   snoopy_tsrm_get_inputdatastorage ()
{
    snoopy_tsrm_threadData_t   *tData;

    tData = snoopy_tsrm_getCurrentThreadData();

    return tData->inputdatastorage;
}



/*
 * snoopy_tsrm_get_threadCount()
 *
 * Description:
 *     Retrieves number of currently configured threads.
 *
 * Params:
 *     (none)
 *
 * Return:
 *     snoopy_inputdatastorage_t*:   Pointer to thread-specific input data storage structure
 */
int   snoopy_tsrm_get_threadCount ()
{
    int   threadCount;

    // Mutex START
    pthread_mutex_lock(&snoopy_tsrm_threadRepo_mutex);

    // Get count
    threadCount = snoopy_tsrm_threadRepo->count;

    // Mutex END
    pthread_mutex_unlock(&snoopy_tsrm_threadRepo_mutex);

    return threadCount;
}
