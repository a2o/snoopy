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
    logMessage[0] = '\n';

    /* Iterate through all inputs */
    snoopy_log_message_generate_testLoopAllInputs(logMessage);
    printf("Snoopy loop through all input providers:\n%s\n", logMessage);

    /* Housekeeping */
    free(logMessage);

    return 0;
}
