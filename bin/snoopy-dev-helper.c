/*
 * SNOOPY LOGGER
 *
 * TODO
 */
#include "snoopy.h"
#include <stdio.h>
#include <stdlib.h>



/*
 * Include all input-related assets
 */
#include "inputregistry.h"
#include "inputdatastorage.h"




int main (int argc, char **argv)
{
    char *logMessage = NULL;

    /* Fake init */
    snoopy_inputdatastorage_store_filename(argv[0]);
    snoopy_inputdatastorage_store_argv(argv);

    // Init log message as empty string
    logMessage    = malloc(SNOOPY_LOG_MESSAGE_MAX_SIZE);
    logMessage[0] = '\0';

    printf("Snoopy: testing custom message formatting:\n\n");
    printf("Message format used: \n%s\n\n", SNOOPY_LOG_MESSAGE_FORMAT);
    snoopy_log_message_generate(logMessage, SNOOPY_LOG_MESSAGE_FORMAT);
    printf("Produced output:\n%s\n\n", logMessage);


    /* Housekeeping */
    free(logMessage);

    return 0;
}
