/*
 * SNOOPY LOGGER
 *
 * File: log.c
 *
 * Copyright (c) 2014 bostjan@a2o.si
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
#include "snoopy.h"
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <syslog.h>



/*
 * Include all input-related resources
 */
#include "log.h"
#include "inputdatastorage.h"
#include "inputregistry.h"



/*
 * snoopy_log_message_generate
 *
 * Description:
 *     Generates log message from inputs available,
 *     for testing/debugging purposes
 *
 * Params:
 *     logMessage:         destination string to return message in
 *     logMessageFormat:   log message format to use
 *
 * Return:
 *     void
 */
void snoopy_log_message_generate (
    char       *logMessage,
    char       *logMessageFormat
) {
}



/*
 * snoopy_log_message_generate_origFormat
 *
 * Description:
 *     Generates log message from inputs available, formatted
 *     according to logFormat specification - TODO
 *
 * Params:
 *     logMessage:   destination string to return message in
 *
 * Return:
 *     void
 */
void snoopy_log_message_generate_origFormat (
    char       *logMessage
) {
    char   *inputMessage = NULL;

    /* Initialize empty strings */
    logMessage[0]    = '\0';
    inputMessage     = malloc(SNOOPY_INPUT_MESSAGE_MAX_SIZE + 1);

    /* Format like original snoopy format did */
    snoopy_log_message_append(logMessage, "[uid:");
    inputMessage[0]  = '\0';
    snoopy_input_uid(inputMessage);
    snoopy_log_message_append(logMessage, inputMessage);
    snoopy_log_message_append(logMessage, " ");

    snoopy_log_message_append(logMessage, "sid:");
    inputMessage[0]  = '\0';
    snoopy_input_sid(inputMessage);
    snoopy_log_message_append(logMessage, inputMessage);
    snoopy_log_message_append(logMessage, " ");

    snoopy_log_message_append(logMessage, "tty:");
    inputMessage[0]  = '\0';
    snoopy_input_tty(inputMessage);
    snoopy_log_message_append(logMessage, inputMessage);
    snoopy_log_message_append(logMessage, " ");

    #if defined(SNOOPY_CWD_LOGGING)
        snoopy_log_message_append(logMessage, "cwd:");
        inputMessage[0]  = '\0';
        snoopy_input_cwd(inputMessage);
        snoopy_log_message_append(logMessage, inputMessage);
        snoopy_log_message_append(logMessage, " ");
    #endif

    snoopy_log_message_append(logMessage, "filename:");
    inputMessage[0]  = '\0';
    snoopy_input_filename(inputMessage);
    snoopy_log_message_append(logMessage, inputMessage);
    snoopy_log_message_append(logMessage, "]: ");

    inputMessage[0]  = '\0';
    snoopy_input_cmdline(inputMessage);
    snoopy_log_message_append(logMessage, inputMessage);

    /* Housekeeping */
    free(inputMessage);
}



/*
 * snoopy_log_message_generate_testLoopAllInputs
 *
 * Description:
 *     Generates log message from inputs available,
 *     for testing/debugging purposes
 *
 * Params:
 *     logMessage:   destination string to return message in
 *
 * Return:
 *     void
 */
void snoopy_log_message_generate_testLoopAllInputs (
    char       *logMessage
) {
    char   *inputMessage = NULL;
    int     i;

    /* Initialize empty strings */
    inputMessage   = malloc(SNOOPY_INPUT_MESSAGE_MAX_SIZE + 1);
    logMessage[0]  = '\0';

    /* Loop throught all inputs and just append the output */
    i = 0;
    while (strcmp(snoopy_inputregistry_names[i], "") != 0) {
        inputMessage[0]  = '\0';
        int inputMessageSize = -1;

        /* Execute the input function */
        inputMessageSize = snoopy_inputregistry_ptrs[i](inputMessage);
        if (inputMessageSize > SNOOPY_INPUT_MESSAGE_MAX_SIZE) {
            // ERROR, TODO
            printf("SNOOPY ERROR: Maximum input message size exceeded (%s)", snoopy_inputregistry_names[i]);
            exit(1);
        }

        /* Copy content, append */
        snoopy_log_message_append(logMessage, snoopy_inputregistry_names[i]);
        snoopy_log_message_append(logMessage, ":\t");
        snoopy_log_message_append(logMessage, inputMessage);
        snoopy_log_message_append(logMessage, "\n");

        /* Go to next input provider */
        i++;
    }

    /* Remove latest space */
    logMessage[strlen(logMessage)-1] = '\0';

    /* Memory housekeeping */
    free(inputMessage);
}



/*
 * snoopy_log_message_append
 *
 * Description:
 *     Appends content to the end of log message, watching for
 *     buffer overruns
 *
 * Params:
 *     logMessage:   message container to append to
 *     appendThis:   content to append to logMessage
 *
 * Return:
 *     void
 */
void snoopy_log_message_append (
    char *logMessage,
    char *appendThis
) {
    int   logMessageSize          = -1;
    int   logMessageSizeRemaining = -1;
    int   appendThisSize          = -1;

    /* Verify the limits */
    logMessageSize          = strlen(logMessage);
    appendThisSize          = strlen(appendThis);
    logMessageSizeRemaining = SNOOPY_LOG_MESSAGE_MAX_SIZE - logMessageSize;
    if (logMessageSizeRemaining < appendThisSize) {
        // ERROR TODO
        printf("SNOOPY ERROR: Maximum log message size exceeded");
        exit(1);
    }

    /* Copy to the destination string */
    strcat(&logMessage[logMessageSize], appendThis);
}



/*
 * snoopy_log_syslog
 *
 * Description:
 *     Send given message to syslog
 *
 * Params:
 *     logMessage:   message to send to syslog
 *
 * Return:
 *     void
 */
void snoopy_log_send_to_syslog (
    char *logMessage
) {
    /* Prepare logging stuff */
    openlog("snoopy", LOG_PID, SNOOPY_SYSLOG_FACILITY);

    /* Log it, but only if non-zero size */
    if (strlen(logMessage) > 0) {
        syslog(SNOOPY_SYSLOG_LEVEL, "%s", logMessage);
    }

    /* Close the syslog file descriptor */
    closelog();
}



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
    snoopy_inputdatastorage_store_filename(filename);
    snoopy_inputdatastorage_store_argv((char**)argv);
//    snoopy_inputdatastorage_store_envp((char**)envp); // TODO, must create null array
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
//    snoopy_inputdatastorage_store_envp((char**)envp);
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
#if defined(SNOOPY_ROOT_ONLY)
    if ((geteuid() != 0) && (getuid() != 0)) {
        return;
    }
#endif

    /* Initialize empty log message */
    logMessage    = malloc(SNOOPY_LOG_MESSAGE_MAX_SIZE);
    logMessage[0] = '\n';

    /* Generate log message in original format, for now */
    snoopy_log_message_generate_origFormat(logMessage);

    /* Send it to syslog */
    snoopy_log_send_to_syslog(logMessage);

    /* Housekeeping */
    free(logMessage);
}
