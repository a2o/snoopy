/*
 * SNOOPY LOGGER
 *
 * File: misc.c
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
#include "misc.h"

#include "snoopy.h"
#include "configuration.h"
#include "error.h"
#include "inputdatastorage.h"
#ifdef SNOOPY_CONF_THREAD_SAFETY_ENABLED
#include "tsrm.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>



/*
 * snoopy_init
 *
 * Description:
 *     Handles Snoopy initialization/startup specifics.
 *     This method must be called when initializing anything that is
 *     Snoopy-based. This is especially true for thread-safe Snoopy builds.
 *
 * Params:
 *     (none)
 *
 * Return:
 *     void
 */
void snoopy_init ()
{
#ifdef SNOOPY_CONF_THREAD_SAFETY_ENABLED
    snoopy_tsrm_ctor();
#endif
    snoopy_configuration_ctor();
    snoopy_inputdatastorage_ctor();
}



/*
 * snoopy_cleanup
 *
 * Description:
 *     Handles all Snoopy deinitialization/shutdown specifics
 *
 * Params:
 *     (none)
 *
 * Return:
 *     void
 */
void snoopy_cleanup ()
{
    /* Reverse order from ctor */
    snoopy_inputdatastorage_dtor();
    snoopy_configuration_dtor();
#ifdef SNOOPY_CONF_THREAD_SAFETY_ENABLED
    snoopy_tsrm_dtor();
#endif
}



/*
 * snoopy_string_append
 *
 * Description:
 *     Appends content to the end of string, watching for
 *     buffer overrun.
 *
 * Params:
 *     destString:            string container to append to
 *     appendThis:            content to append to destString
 *     destStringMaxLength:   maximum length of dest string, including \0
 *
 * Return:
 *     void
 */
void snoopy_string_append (
    char *destString,
    char *appendThis,
    int   destStringMaxLength
) {
    int   destStringSize          = -1;
    int   destStringSizeRemaining = -1;
    int   appendThisSize          = -1;

    /* Verify the limits */
    destStringSize          = strlen(destString);
    appendThisSize          = strlen(appendThis);
    destStringSizeRemaining = destStringMaxLength - destStringSize;
    if (destStringSizeRemaining < appendThisSize) {
        snoopy_error_handler("Maximum destination string size exceeded");
    }

    /* Copy to the destination string */
    strcat(&destString[destStringSize], appendThis);
}



/*
 * snoopy_string_countChars
 *
 * Description:
 *     Counts number of occurences of specified character in a given string.
 *
 * Params:
 *     stringToSearch:   string to look into
 *     characterToCount: search for this character
 *
 * Return:
 *     int   Number of occurences
 */
int  snoopy_string_countChars (const char *stringToSearch, char characterToCount)
{
    const char *strPtr = stringToSearch;
    int charCount      = 0;

    while ('\0' != *strPtr) {
        if (*strPtr == characterToCount) {
            charCount++;
        }
        strPtr++;
    }

    return charCount;
}



/*
 * snoopy_syslog_convert_facilityToInt
 *
 * Description:
 *     Returns corresponding integer for each syslog facility, or -1 on failure.
 *
 * Params:
 *     facilityStr   Syslog facility string to convert
 *
 * Return:
 *     int           Corresponding syslog facility id, or -1 if not found
 */
int snoopy_syslog_convert_facilityToInt (
    char *facilityStr
) {
    char *facilityStrAdj;
    int   facilityInt;

    facilityStrAdj = facilityStr;

    // If there is LOG_ prefix, loose it.
    if ('_' == facilityStr[3]) {
        facilityStrAdj = &facilityStr[4];
    }

    // Evaluate
    if      (strcmp(facilityStrAdj, "AUTH")     == 0) { facilityInt = LOG_AUTH;     }
    else if (strcmp(facilityStrAdj, "AUTHPRIV") == 0) { facilityInt = LOG_AUTHPRIV; }
    else if (strcmp(facilityStrAdj, "CRON")     == 0) { facilityInt = LOG_CRON;     }
    else if (strcmp(facilityStrAdj, "DAEMON")   == 0) { facilityInt = LOG_DAEMON;   }
    else if (strcmp(facilityStrAdj, "FTP")      == 0) { facilityInt = LOG_FTP;      }
    else if (strcmp(facilityStrAdj, "KERN")     == 0) { facilityInt = LOG_KERN;     }
    else if (strcmp(facilityStrAdj, "LOCAL0")   == 0) { facilityInt = LOG_LOCAL0;   }
    else if (strcmp(facilityStrAdj, "LOCAL1")   == 0) { facilityInt = LOG_LOCAL1;   }
    else if (strcmp(facilityStrAdj, "LOCAL2")   == 0) { facilityInt = LOG_LOCAL2;   }
    else if (strcmp(facilityStrAdj, "LOCAL3")   == 0) { facilityInt = LOG_LOCAL3;   }
    else if (strcmp(facilityStrAdj, "LOCAL4")   == 0) { facilityInt = LOG_LOCAL4;   }
    else if (strcmp(facilityStrAdj, "LOCAL5")   == 0) { facilityInt = LOG_LOCAL5;   }
    else if (strcmp(facilityStrAdj, "LOCAL6")   == 0) { facilityInt = LOG_LOCAL6;   }
    else if (strcmp(facilityStrAdj, "LOCAL7")   == 0) { facilityInt = LOG_LOCAL7;   }
    else if (strcmp(facilityStrAdj, "LPR")      == 0) { facilityInt = LOG_LPR;      }
    else if (strcmp(facilityStrAdj, "MAIL")     == 0) { facilityInt = LOG_MAIL;     }
    else if (strcmp(facilityStrAdj, "NEWS")     == 0) { facilityInt = LOG_NEWS;     }
    else if (strcmp(facilityStrAdj, "SYSLOG")   == 0) { facilityInt = LOG_SYSLOG;   }
    else if (strcmp(facilityStrAdj, "USER")     == 0) { facilityInt = LOG_USER;     }
    else if (strcmp(facilityStrAdj, "UUCP")     == 0) { facilityInt = LOG_UUCP;     }
    else {
        facilityInt = -1;
    }

    return facilityInt;
}



/*
 * snoopy_syslog_convert_facilityToStr
 *
 * Description:
 *     Convert syslog facility from integer code to corresponding string.
 *
 * Params:
 *     facilityInt   Syslog facility to convert
 *
 * Return:
 *     const char*   Corresponding syslog facility string
 */
const char* snoopy_syslog_convert_facilityToStr (
    int   facilityInt
) {
    char *facilityStr;

    // Evaluate and set return value
    if      (LOG_AUTH     == facilityInt) { facilityStr = "AUTH";     }
    else if (LOG_AUTHPRIV == facilityInt) { facilityStr = "AUTHPRIV"; }
    else if (LOG_CRON     == facilityInt) { facilityStr = "CRON";     }
    else if (LOG_DAEMON   == facilityInt) { facilityStr = "DAEMON";   }
    else if (LOG_FTP      == facilityInt) { facilityStr = "FTP";      }
    else if (LOG_KERN     == facilityInt) { facilityStr = "KERN";     }
    else if (LOG_LOCAL0   == facilityInt) { facilityStr = "LOCAL0";   }
    else if (LOG_LOCAL1   == facilityInt) { facilityStr = "LOCAL1";   }
    else if (LOG_LOCAL2   == facilityInt) { facilityStr = "LOCAL2";   }
    else if (LOG_LOCAL3   == facilityInt) { facilityStr = "LOCAL3";   }
    else if (LOG_LOCAL4   == facilityInt) { facilityStr = "LOCAL4";   }
    else if (LOG_LOCAL5   == facilityInt) { facilityStr = "LOCAL5";   }
    else if (LOG_LOCAL6   == facilityInt) { facilityStr = "LOCAL6";   }
    else if (LOG_LOCAL7   == facilityInt) { facilityStr = "LOCAL7";   }
    else if (LOG_LPR      == facilityInt) { facilityStr = "LPR";      }
    else if (LOG_MAIL     == facilityInt) { facilityStr = "MAIL";     }
    else if (LOG_NEWS     == facilityInt) { facilityStr = "NEWS";     }
    else if (LOG_SYSLOG   == facilityInt) { facilityStr = "SYSLOG";   }
    else if (LOG_USER     == facilityInt) { facilityStr = "USER";     }
    else if (LOG_UUCP     == facilityInt) { facilityStr = "UUCP";     }
    else {
        facilityStr = "(invalid)";
    }

    return facilityStr;
}



/*
 * snoopy_syslog_convert_levelToInt
 *
 * Description:
 *     Returns corresponding integer for each syslog level, or -1 on failure.
 *
 * Params:
 *     levelStr   Syslog level string to convert
 *
 * Return:
 *     int        Corresponding syslog level id, or -1 if not found
 */
int snoopy_syslog_convert_levelToInt (
    char *levelStr
) {
    char *levelStrAdj;
    int   levelInt;

    levelStrAdj = levelStr;

    // If there is LOG_ prefix, loose it.
    if ('_' == levelStr[3]) {
        levelStrAdj = &levelStr[4];
    }

    // Evaluate and set return value
    if      (strcmp(levelStrAdj, "EMERG")   == 0) { levelInt = LOG_EMERG;   }
    else if (strcmp(levelStrAdj, "ALERT")   == 0) { levelInt = LOG_ALERT;   }
    else if (strcmp(levelStrAdj, "CRIT")    == 0) { levelInt = LOG_CRIT;    }
    else if (strcmp(levelStrAdj, "ERR")     == 0) { levelInt = LOG_ERR;     }
    else if (strcmp(levelStrAdj, "WARNING") == 0) { levelInt = LOG_WARNING; }
    else if (strcmp(levelStrAdj, "NOTICE")  == 0) { levelInt = LOG_NOTICE;  }
    else if (strcmp(levelStrAdj, "INFO")    == 0) { levelInt = LOG_INFO;    }
    else if (strcmp(levelStrAdj, "DEBUG")   == 0) { levelInt = LOG_DEBUG;   }
    else {
        levelInt = SNOOPY_SYSLOG_LEVEL;
    }

    return levelInt;
}



/*
 * snoopy_syslog_convert_levelToStr
 *
 * Description:
 *     Convert syslog level from integer code to corresponding string.
 *
 * Params:
 *     levelInt      Syslog level to convert
 *
 * Return:
 *     const char*   Corresponding syslog facility string
 */
const char* snoopy_syslog_convert_levelToStr (
    int   levelInt
) {
    char *levelStr;

    // Evaluate and set return value
    if      (LOG_EMERG   == levelInt) { levelStr = "EMERG";   }
    else if (LOG_ALERT   == levelInt) { levelStr = "ALERT";   }
    else if (LOG_CRIT    == levelInt) { levelStr = "CRIT";    }
    else if (LOG_ERR     == levelInt) { levelStr = "ERR";     }
    else if (LOG_WARNING == levelInt) { levelStr = "WARNING"; }
    else if (LOG_NOTICE  == levelInt) { levelStr = "NOTICE";  }
    else if (LOG_INFO    == levelInt) { levelStr = "INFO";    }
    else if (LOG_DEBUG   == levelInt) { levelStr = "DEBUG";   }
    else {
        levelStr = "(invalid)";
    }

    return levelStr;
}
