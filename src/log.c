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
 * Include all required C resources
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



/*
 * Include all snoopy-related resources
 */
#include "snoopy.h"
#include "log.h"

#include "configuration.h"
#if defined(SNOOPY_FILTERING_ENABLED)
#include "filtering.h"
#endif
#include "inputdatastorage.h"
#include "message.h"
#include "misc.h"
#include "outputregistry.h"



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
    char *envp[] = { NULL };
    snoopy_inputdatastorage_store_filename(filename);
    snoopy_inputdatastorage_store_argv((char**)argv);
    snoopy_inputdatastorage_store_envp(envp);
    snoopy_log_syscall("execv");
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
    snoopy_inputdatastorage_store_filename(filename);
    snoopy_inputdatastorage_store_argv((char**)argv);
    snoopy_inputdatastorage_store_envp((char**)envp);
    snoopy_log_syscall("execve");
}



/*
 * snoopy_log_syscall
 *
 * Description:
 *     Log the call to one of the syscalls
 *
 * Params:
 *     syscallName:   system call name to log
 *
 * Return:
 *     void
 */
void snoopy_log_syscall (
    const char *syscallName
) {
    char *logMessage = NULL;

    /* Do not log non-root commands if this is requested */
#if defined(SNOOPY_CONF_ROOT_ONLY)
    if ((geteuid() != 0) && (getuid() != 0)) {
        return;
    }
#endif

    /* Initialize snoopy */
    snoopy_init();

    /* Initialize empty log message */
    logMessage    = malloc(SNOOPY_LOG_MESSAGE_MAX_SIZE);
    logMessage[0] = '\0';

    /* Generate log message in specified format */
    snoopy_message_generateFromFormat(logMessage, snoopy_configuration.message_format);

#if defined(SNOOPY_FILTERING_ENABLED)
    /* Should message be passed to syslog or not? */
    if (
        (SNOOPY_FALSE == snoopy_configuration.filtering_enabled)
        ||
        (
            (SNOOPY_TRUE == snoopy_configuration.filtering_enabled)
            &&
            (SNOOPY_FILTER_PASS == snoopy_filtering_check_chain(logMessage, snoopy_configuration.filter_chain))
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
 *     void
 */
void snoopy_log_dispatch (
    char *logMessage,
    int   errorOrMessage
) {
    /* Dispatch only if non-zero size */
    if (0 == strlen(logMessage)) {
        return;
    }

    // Dispatch to configured output
    snoopy_outputregistry_dispatch(logMessage, errorOrMessage);
}
