/*
 * SNOOPY LOGGER
 *
 * File: log.c
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
#include "log.h"

#include "snoopy.h"
#include "configuration.h"
#if defined(SNOOPY_FILTERING_ENABLED)
#include "filtering.h"
#endif
#include "inputdatastorage.h"
#include "message.h"
#include "misc.h"
#include "outputregistry.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



/*
 * snoopy_log_syscall_execv
 *
 * Description:
 *     Log the call to syscall execv()
 *
 * Params:
 *     filename:   filename of program being executed
 *     argv:       arguments being passed to execv()
 *
 * Return:
 *     void
 */
void snoopy_log_syscall_execv (
    const char *filename,
    char *const argv[]
) {
    // Syscall was done without new environmental variables, so let's create
    // a fake empty array to simulate it.
    char *envp[] = { NULL };

    snoopy_log_syscall_exec("execv", filename, argv, envp);
}



/*
 * snoopy_log_syscall_execve
 *
 * Description:
 *     Log the call to syscall execve()
 *
 * Params:
 *     filename:   filename of program being executed
 *     argv:       arguments being passed to execve()
 *     envp:       environment being passed to execve()
 *
 * Return:
 *     void
 */
void snoopy_log_syscall_execve (
    const char *filename,
    char *const argv[],
    char *const envp[]
) {
    snoopy_log_syscall_exec("execve", filename, argv, envp);
}



/*
 * snoopy_log_syscall_exec
 *
 * Description:
 *     Common routine that does execv(e)() logging
 *
 * Params:
 *     syscallName:   system call name to log
 *     filename:      filename of program being executed
 *     argv:          arguments being passed to execv(e)()
 *     envp:          environment being passed to execve()
 *
 * Return:
 *     void
 */
void snoopy_log_syscall_exec (
    const char *syscallName,
    const char *filename,
    char *const argv[],
    char *const envp[]
) {
    char *logMessage = NULL;
    snoopy_configuration_t *CFG;


    /* Initialize Snoopy */
    snoopy_init();

    /* Get config pointer */
    CFG = snoopy_configuration_get();

    // Store arguments passed to execv(e)()
    snoopy_inputdatastorage_store_filename(filename);
    snoopy_inputdatastorage_store_argv((char**)argv);
    snoopy_inputdatastorage_store_envp((char**)envp);

    /* Initialize empty log message */
    logMessage    = malloc(SNOOPY_LOG_MESSAGE_MAX_SIZE);
    logMessage[0] = '\0';

    /* Generate log message in specified format */
    snoopy_message_generateFromFormat(logMessage, CFG->message_format);

#if defined(SNOOPY_FILTERING_ENABLED)
    /* Should message be passed to syslog or not? */
    if (
        (SNOOPY_FALSE == CFG->filtering_enabled)
        ||
        (
            (SNOOPY_TRUE == CFG->filtering_enabled)
            &&
            (SNOOPY_FILTER_PASS == snoopy_filtering_check_chain(logMessage, CFG->filter_chain))
        )
    ) {
#endif
        snoopy_log_dispatch(logMessage, SNOOPY_LOG_MESSAGE);
#if defined(SNOOPY_FILTERING_ENABLED)
    }
#endif

    /* Housekeeping */
    free(logMessage);
    snoopy_cleanup();
}



/*
 * snoopy_log_dispatch
 *
 * Description:
 *     Dispatch given message to configured output
 *
 * Params:
 *     logMessage:       message to dispatch
 *     errorOrMessage:   is this a message or an error?
 *
 * Return:
 *     int:              See snoopy.h (SNOOPY_OUTPUT_*) for details.
 */
int snoopy_log_dispatch (
    char *logMessage,
    int   errorOrMessage
) {
    /* Dispatch only if non-zero size */
    if (0 == strlen(logMessage)) {
        return SNOOPY_OUTPUT_GRACEFUL_DISCARD;
    }

    // Dispatch to configured output
    return snoopy_outputregistry_dispatch(logMessage, errorOrMessage);
}
