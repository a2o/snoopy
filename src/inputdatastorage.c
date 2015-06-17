/*
 * SNOOPY LOGGER
 *
 * File: inputdatastorage.c
 *
 * Copyright (c) 2014-2015 Bostjan Skufca <bostjan@a2o.si>
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
#include "inputdatastorage.h"

#include "snoopy.h"
#ifdef SNOOPY_CONF_THREAD_SAFETY_ENABLED
#include "tsrm.h"
#endif



/*
 * Common definitions
 */
#define   EMPTY_FILENAME      ""
#define   EMPTY_STRINGARRAY   (char *[]){'\0'}



/*
 * Storage of Snoopy's input data for non-thread-safe builds
 */
#ifndef SNOOPY_CONF_THREAD_SAFETY_ENABLED
snoopy_inputdatastorage_t   snoopy_inputdatastorage_data = {
    .initialized = SNOOPY_FALSE,
};
#endif



/*
 * snoopy_inputdatastorage_ctor
 *
 * Description:
 *     Populates snoopy_inputdatastorage struct with default empty
 *     data, in order to prevent data leaks between execv(e) calls.
 *
 * Params:
 *     (none)
 *
 * Return:
 *     void
 */
void snoopy_inputdatastorage_ctor ()
{
    /* Get IDS pointer */
    snoopy_inputdatastorage_t *IDS = snoopy_inputdatastorage_get();

    snoopy_inputdatastorage_setDefaults(IDS);
}



/*
 * snoopy_inputdatastorage_dtor
 *
 * Description:
 *     Populates snoopy_inputdatastorage struct with default empty
 *     data, in order to prevent data leaks between execv(e) calls.
 *
 *     NOTE: This is intentional behaviour. Reset in ctor and in
 *           dtor too.
 *
 * Params:
 *     (none)
 *
 * Return:
 *     void
 */
void snoopy_inputdatastorage_dtor ()
{
    /* Get IDS pointer */
    snoopy_inputdatastorage_t *IDS = snoopy_inputdatastorage_get();

    snoopy_inputdatastorage_setDefaults(IDS);
}



/*
 * snoopy_inputdatastorage_setUninitialized()
 *
 * Description:
 *     Sets the state of IDS to uninitialized.
 *
 * Params:
 *     (none)
 *
 * Return:
 *     void
 */
void snoopy_inputdatastorage_setUninitialized
(
    snoopy_inputdatastorage_t *IDS
) {
    IDS->initialized = SNOOPY_FALSE;
}



/*
 * snoopy_inputdatastorage_setDefaults()
 *
 * Description:
 *     Resets the input data storage to default values
 *
 * Params:
 *     (none)
 *
 * Return:
 *     void
 */
void snoopy_inputdatastorage_setDefaults
(
    snoopy_inputdatastorage_t *IDS
) {
    IDS->initialized = SNOOPY_TRUE;
    IDS->filename    = EMPTY_FILENAME;
    IDS->argv        = EMPTY_STRINGARRAY;
    IDS->envp        = EMPTY_STRINGARRAY;
}



/*
 * snoopy_inputdatastorage_store_filename()
 *
 * Description:
 *     Store filename of execv()/execve() syscall
 *
 * Params:
 *     filename:   filename to store
 *
 * Return:
 *     void
 */
void snoopy_inputdatastorage_store_filename (
    const char *filename
) {
    /* Get IDS pointer */
    snoopy_inputdatastorage_t *IDS = snoopy_inputdatastorage_get();

    /* Store value */
    IDS->filename = filename;
}



/*
 * snoopy_inputdatastorage_store_argv()
 *
 * Description:
 *     Store argv[] of execv()/execve() syscall
 *
 * Params:
 *     argv:   argv pointer to store
 *
 * Return:
 *     void
 */
void snoopy_inputdatastorage_store_argv (
    char *argv[]
) {
    /* Get IDS pointer */
    snoopy_inputdatastorage_t *IDS = snoopy_inputdatastorage_get();

    /* Store value */
    IDS->argv = argv;
}



/*
 * snoopy_inputdatastorage_store_envp()
 *
 * Description:
 *     Store envp[] of execve() syscall
 *
 * Params:
 *     envp:   environment array pointer to store
 *
 * Return:
 *     void
 */
void snoopy_inputdatastorage_store_envp (
    char *envp[]
) {
    /* Get IDS pointer */
    snoopy_inputdatastorage_t *IDS = snoopy_inputdatastorage_get();

    /* Store value */
    IDS->envp = envp;
}



/*
 * snoopy_inputdatastorage_get()
 *
 * Description:
 *     Retrieve inpudatastorage struct pointer
 *
 * Params:
 *     (none)
 *
 * Return:
 *     snoopy_inputdatastorage_t*:   Pointer to inputdatastorage struct
 */
snoopy_inputdatastorage_t* snoopy_inputdatastorage_get ()
{
    snoopy_inputdatastorage_t *IDS;

#ifdef SNOOPY_CONF_THREAD_SAFETY_ENABLED
    IDS = snoopy_tsrm_get_inputdatastorage();
#else
    IDS = &snoopy_inputdatastorage_data;
#endif

    if (SNOOPY_TRUE != IDS->initialized) {
        snoopy_inputdatastorage_setDefaults(IDS);
    }

    return IDS;
}
