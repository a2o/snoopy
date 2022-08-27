/*
 * SNOOPY COMMAND LOGGER
 *
 * Copyright (c) 2022 Bostjan Skufca Jese (bostjan _A_T_ a2o.si)
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
 * Includes order: from local to global (local, this-subsystem, snoopy.h, other-subsystems, global)
 *
 * Header file must not be named "syslog.h" in order to not conflict with the global <syslog.h>.
 * Therefore we're using the "-snoopy" suffix - "syslog-snoopy.h".
 */
#include "syslog-snoopy.h"

#include "snoopy.h"

#include <string.h>
#include <syslog.h>



/*
 * Return corresponding integer for each syslog facility
 *
 * Params:
 *     facilityStr   Syslog facility string to convert
 *
 * Return:
 *     int           Corresponding syslog facility id, or -1 if not found
 */
int snoopy_util_syslog_convertFacilityToInt (const char *facilityStr)
{
    const char *facilityStrAdj;
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
 * Convert syslog facility from integer code to corresponding string
 *
 * Params:
 *     facilityInt   Syslog facility to convert
 *
 * Return:
 *     const char *  Corresponding syslog facility string
 */
const char* snoopy_util_syslog_convertFacilityToStr (int facilityInt)
{
    const char *facilityStr;

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
 * Return corresponding integer for each syslog level
 *
 * Params:
 *     levelStr   Syslog level string to convert
 *
 * Return:
 *     int        Corresponding syslog level id, or -1 if not found
 */
int snoopy_util_syslog_convertLevelToInt (const char *levelStr)
{
    const char *levelStrAdj;
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
 * Convert syslog level from integer code to corresponding string
 *
 * Params:
 *     levelInt      Syslog level to convert
 *
 * Return:
 *     const char *  Corresponding syslog facility string
 */
const char * snoopy_util_syslog_convertLevelToStr (int levelInt)
{
    const char *levelStr;

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
