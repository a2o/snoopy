/**
 * SNOOPY_MAX_ARG_LENGTH
 *
 * Maximum length of arguments passed to execv(e) functions
 */
#include <unistd.h>
#define SNOOPY_MAX_ARG_LENGTH sysconf(_SC_ARG_MAX)
