/*
 * SNOOPY LOGGER
 *
 * Copyright (c) 2015 Bostjan Skufca Jese <bostjan@a2o.si>
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
#include "action-unit-misc.h"
#include "action-common.h"

#include "snoopy.h"
#include "misc.h"

#include <stdio.h>
#include <string.h>
#include <syslog.h>



void snoopyTestCli_action_unit_misc_showHelp ()
{
    char * helpContent =
        "Snoopy TEST SUITE CLI utility :: Action `unit` :: Unit `misc`\n"
        "\n"
        "Description:\n"
        "    Mocks src/misc.c implementation code paths (mainly for the coverage of code parts/paths not covered by the test suite).\n"
        "\n"
        "Usage:\n"
        "    snoopy-test unit misc\n"
        "    snoopy-test unit misc --help\n"
        "\n";
    printf("%s", helpContent);
}



int snoopyTestCli_action_unit_misc (int argc, char ** argv)
{
    const char *arg1;

    if (argc > 0) {
        arg1 = argv[0];
    } else {
        arg1 = "";
    }

    if (0 == strcmp(arg1, "--help")) {
        snoopyTestCli_action_unit_misc_showHelp();
        return 0;
    }


    // Do the mocking
    if (snoopy_syslog_convert_facilityToInt("AUTH")     != LOG_AUTH)     fatalErrorValue("Error converting syslog facility str to int", "AUTH");
    if (snoopy_syslog_convert_facilityToInt("AUTHPRIV") != LOG_AUTHPRIV) fatalErrorValue("Error converting syslog facility str to int", "AUTHPRIV");
    if (snoopy_syslog_convert_facilityToInt("CRON")     != LOG_CRON)     fatalErrorValue("Error converting syslog facility str to int", "CRON");
    if (snoopy_syslog_convert_facilityToInt("DAEMON")   != LOG_DAEMON)   fatalErrorValue("Error converting syslog facility str to int", "DAEMON");
    if (snoopy_syslog_convert_facilityToInt("FTP")      != LOG_FTP)      fatalErrorValue("Error converting syslog facility str to int", "FTP");
    if (snoopy_syslog_convert_facilityToInt("KERN")     != LOG_KERN)     fatalErrorValue("Error converting syslog facility str to int", "KERN");
    if (snoopy_syslog_convert_facilityToInt("LOCAL0")   != LOG_LOCAL0)   fatalErrorValue("Error converting syslog facility str to int", "LOCAL0");
    if (snoopy_syslog_convert_facilityToInt("LOCAL1")   != LOG_LOCAL1)   fatalErrorValue("Error converting syslog facility str to int", "LOCAL1");
    if (snoopy_syslog_convert_facilityToInt("LOCAL2")   != LOG_LOCAL2)   fatalErrorValue("Error converting syslog facility str to int", "LOCAL2");
    if (snoopy_syslog_convert_facilityToInt("LOCAL3")   != LOG_LOCAL3)   fatalErrorValue("Error converting syslog facility str to int", "LOCAL3");
    if (snoopy_syslog_convert_facilityToInt("LOCAL4")   != LOG_LOCAL4)   fatalErrorValue("Error converting syslog facility str to int", "LOCAL4");
    if (snoopy_syslog_convert_facilityToInt("LOCAL5")   != LOG_LOCAL5)   fatalErrorValue("Error converting syslog facility str to int", "LOCAL5");
    if (snoopy_syslog_convert_facilityToInt("LOCAL6")   != LOG_LOCAL6)   fatalErrorValue("Error converting syslog facility str to int", "LOCAL6");
    if (snoopy_syslog_convert_facilityToInt("LOCAL7")   != LOG_LOCAL7)   fatalErrorValue("Error converting syslog facility str to int", "LOCAL7");
    if (snoopy_syslog_convert_facilityToInt("LPR")      != LOG_LPR)      fatalErrorValue("Error converting syslog facility str to int", "LPR");
    if (snoopy_syslog_convert_facilityToInt("MAIL")     != LOG_MAIL)     fatalErrorValue("Error converting syslog facility str to int", "MAIL");
    if (snoopy_syslog_convert_facilityToInt("NEWS")     != LOG_NEWS)     fatalErrorValue("Error converting syslog facility str to int", "NEWS");
    if (snoopy_syslog_convert_facilityToInt("SYSLOG")   != LOG_SYSLOG)   fatalErrorValue("Error converting syslog facility str to int", "SYSLOG");
    if (snoopy_syslog_convert_facilityToInt("USER")     != LOG_USER)     fatalErrorValue("Error converting syslog facility str to int", "USER");
    if (snoopy_syslog_convert_facilityToInt("UUCP")     != LOG_UUCP)     fatalErrorValue("Error converting syslog facility str to int", "UUCP");

    if (snoopy_syslog_convert_facilityToInt("LOG_AUTH")     != LOG_AUTH)     fatalErrorValue("Error converting syslog facility str to int", "LOG_AUTH");
    if (snoopy_syslog_convert_facilityToInt("LOG_AUTHPRIV") != LOG_AUTHPRIV) fatalErrorValue("Error converting syslog facility str to int", "LOG_AUTHPRIV");
    if (snoopy_syslog_convert_facilityToInt("LOG_CRON")     != LOG_CRON)     fatalErrorValue("Error converting syslog facility str to int", "LOG_CRON");
    if (snoopy_syslog_convert_facilityToInt("LOG_DAEMON")   != LOG_DAEMON)   fatalErrorValue("Error converting syslog facility str to int", "LOG_DAEMON");
    if (snoopy_syslog_convert_facilityToInt("LOG_FTP")      != LOG_FTP)      fatalErrorValue("Error converting syslog facility str to int", "LOG_FTP");
    if (snoopy_syslog_convert_facilityToInt("LOG_KERN")     != LOG_KERN)     fatalErrorValue("Error converting syslog facility str to int", "LOG_KERN");
    if (snoopy_syslog_convert_facilityToInt("LOG_LOCAL0")   != LOG_LOCAL0)   fatalErrorValue("Error converting syslog facility str to int", "LOG_LOCAL0");
    if (snoopy_syslog_convert_facilityToInt("LOG_LOCAL1")   != LOG_LOCAL1)   fatalErrorValue("Error converting syslog facility str to int", "LOG_LOCAL1");
    if (snoopy_syslog_convert_facilityToInt("LOG_LOCAL2")   != LOG_LOCAL2)   fatalErrorValue("Error converting syslog facility str to int", "LOG_LOCAL2");
    if (snoopy_syslog_convert_facilityToInt("LOG_LOCAL3")   != LOG_LOCAL3)   fatalErrorValue("Error converting syslog facility str to int", "LOG_LOCAL3");
    if (snoopy_syslog_convert_facilityToInt("LOG_LOCAL4")   != LOG_LOCAL4)   fatalErrorValue("Error converting syslog facility str to int", "LOG_LOCAL4");
    if (snoopy_syslog_convert_facilityToInt("LOG_LOCAL5")   != LOG_LOCAL5)   fatalErrorValue("Error converting syslog facility str to int", "LOG_LOCAL5");
    if (snoopy_syslog_convert_facilityToInt("LOG_LOCAL6")   != LOG_LOCAL6)   fatalErrorValue("Error converting syslog facility str to int", "LOG_LOCAL6");
    if (snoopy_syslog_convert_facilityToInt("LOG_LOCAL7")   != LOG_LOCAL7)   fatalErrorValue("Error converting syslog facility str to int", "LOG_LOCAL7");
    if (snoopy_syslog_convert_facilityToInt("LOG_LPR")      != LOG_LPR)      fatalErrorValue("Error converting syslog facility str to int", "LOG_LPR");
    if (snoopy_syslog_convert_facilityToInt("LOG_MAIL")     != LOG_MAIL)     fatalErrorValue("Error converting syslog facility str to int", "LOG_MAIL");
    if (snoopy_syslog_convert_facilityToInt("LOG_NEWS")     != LOG_NEWS)     fatalErrorValue("Error converting syslog facility str to int", "LOG_NEWS");
    if (snoopy_syslog_convert_facilityToInt("LOG_SYSLOG")   != LOG_SYSLOG)   fatalErrorValue("Error converting syslog facility str to int", "LOG_SYSLOG");
    if (snoopy_syslog_convert_facilityToInt("LOG_USER")     != LOG_USER)     fatalErrorValue("Error converting syslog facility str to int", "LOG_USER");
    if (snoopy_syslog_convert_facilityToInt("LOG_UUCP")     != LOG_UUCP)     fatalErrorValue("Error converting syslog facility str to int", "LOG_UUCP");

    if (0 != strcmp(snoopy_syslog_convert_facilityToStr(LOG_AUTH)    , "AUTH")    ) fatalErrorValue("Error converting syslog facility int to str", "AUTH");
    if (0 != strcmp(snoopy_syslog_convert_facilityToStr(LOG_AUTHPRIV), "AUTHPRIV")) fatalErrorValue("Error converting syslog facility int to str", "AUTHPRIV");
    if (0 != strcmp(snoopy_syslog_convert_facilityToStr(LOG_CRON)    , "CRON")    ) fatalErrorValue("Error converting syslog facility int to str", "CRON");
    if (0 != strcmp(snoopy_syslog_convert_facilityToStr(LOG_DAEMON)  , "DAEMON")  ) fatalErrorValue("Error converting syslog facility int to str", "DAEMON");
    if (0 != strcmp(snoopy_syslog_convert_facilityToStr(LOG_FTP)     , "FTP")     ) fatalErrorValue("Error converting syslog facility int to str", "FTP");
    if (0 != strcmp(snoopy_syslog_convert_facilityToStr(LOG_KERN)    , "KERN")    ) fatalErrorValue("Error converting syslog facility int to str", "KERN");
    if (0 != strcmp(snoopy_syslog_convert_facilityToStr(LOG_LOCAL0)  , "LOCAL0")  ) fatalErrorValue("Error converting syslog facility int to str", "LOCAL0");
    if (0 != strcmp(snoopy_syslog_convert_facilityToStr(LOG_LOCAL1)  , "LOCAL1")  ) fatalErrorValue("Error converting syslog facility int to str", "LOCAL1");
    if (0 != strcmp(snoopy_syslog_convert_facilityToStr(LOG_LOCAL2)  , "LOCAL2")  ) fatalErrorValue("Error converting syslog facility int to str", "LOCAL2");
    if (0 != strcmp(snoopy_syslog_convert_facilityToStr(LOG_LOCAL3)  , "LOCAL3")  ) fatalErrorValue("Error converting syslog facility int to str", "LOCAL3");
    if (0 != strcmp(snoopy_syslog_convert_facilityToStr(LOG_LOCAL4)  , "LOCAL4")  ) fatalErrorValue("Error converting syslog facility int to str", "LOCAL4");
    if (0 != strcmp(snoopy_syslog_convert_facilityToStr(LOG_LOCAL5)  , "LOCAL5")  ) fatalErrorValue("Error converting syslog facility int to str", "LOCAL5");
    if (0 != strcmp(snoopy_syslog_convert_facilityToStr(LOG_LOCAL6)  , "LOCAL6")  ) fatalErrorValue("Error converting syslog facility int to str", "LOCAL6");
    if (0 != strcmp(snoopy_syslog_convert_facilityToStr(LOG_LOCAL7)  , "LOCAL7")  ) fatalErrorValue("Error converting syslog facility int to str", "LOCAL7");
    if (0 != strcmp(snoopy_syslog_convert_facilityToStr(LOG_LPR)     , "LPR")     ) fatalErrorValue("Error converting syslog facility int to str", "LPR");
    if (0 != strcmp(snoopy_syslog_convert_facilityToStr(LOG_MAIL)    , "MAIL")    ) fatalErrorValue("Error converting syslog facility int to str", "MAIL");
    if (0 != strcmp(snoopy_syslog_convert_facilityToStr(LOG_NEWS)    , "NEWS")    ) fatalErrorValue("Error converting syslog facility int to str", "NEWS");
    if (0 != strcmp(snoopy_syslog_convert_facilityToStr(LOG_SYSLOG)  , "SYSLOG")  ) fatalErrorValue("Error converting syslog facility int to str", "SYSLOG");
    if (0 != strcmp(snoopy_syslog_convert_facilityToStr(LOG_USER)    , "USER")    ) fatalErrorValue("Error converting syslog facility int to str", "USER");
    if (0 != strcmp(snoopy_syslog_convert_facilityToStr(LOG_UUCP)    , "UUCP")    ) fatalErrorValue("Error converting syslog facility int to str", "UUCP");


    if (snoopy_syslog_convert_levelToInt("EMERG")   != LOG_EMERG)   fatalErrorValue("Error converting syslog level str to int", "EMERG");
    if (snoopy_syslog_convert_levelToInt("ALERT")   != LOG_ALERT)   fatalErrorValue("Error converting syslog level str to int", "ALERT");
    if (snoopy_syslog_convert_levelToInt("CRIT")    != LOG_CRIT)    fatalErrorValue("Error converting syslog level str to int", "CRIT");
    if (snoopy_syslog_convert_levelToInt("ERR")     != LOG_ERR)     fatalErrorValue("Error converting syslog level str to int", "ERR");
    if (snoopy_syslog_convert_levelToInt("WARNING") != LOG_WARNING) fatalErrorValue("Error converting syslog level str to int", "WARNING");
    if (snoopy_syslog_convert_levelToInt("NOTICE")  != LOG_NOTICE)  fatalErrorValue("Error converting syslog level str to int", "NOTICE");
    if (snoopy_syslog_convert_levelToInt("INFO")    != LOG_INFO)    fatalErrorValue("Error converting syslog level str to int", "INFO");
    if (snoopy_syslog_convert_levelToInt("DEBUG")   != LOG_DEBUG)   fatalErrorValue("Error converting syslog level str to int", "DEBUG");

    if (snoopy_syslog_convert_levelToInt("LOG_EMERG")   != LOG_EMERG)   fatalErrorValue("Error converting syslog level str to int", "EMERG");
    if (snoopy_syslog_convert_levelToInt("LOG_ALERT")   != LOG_ALERT)   fatalErrorValue("Error converting syslog level str to int", "ALERT");
    if (snoopy_syslog_convert_levelToInt("LOG_CRIT")    != LOG_CRIT)    fatalErrorValue("Error converting syslog level str to int", "CRIT");
    if (snoopy_syslog_convert_levelToInt("LOG_ERR")     != LOG_ERR)     fatalErrorValue("Error converting syslog level str to int", "ERR");
    if (snoopy_syslog_convert_levelToInt("LOG_WARNING") != LOG_WARNING) fatalErrorValue("Error converting syslog level str to int", "WARNING");
    if (snoopy_syslog_convert_levelToInt("LOG_NOTICE")  != LOG_NOTICE)  fatalErrorValue("Error converting syslog level str to int", "NOTICE");
    if (snoopy_syslog_convert_levelToInt("LOG_INFO")    != LOG_INFO)    fatalErrorValue("Error converting syslog level str to int", "INFO");
    if (snoopy_syslog_convert_levelToInt("LOG_DEBUG")   != LOG_DEBUG)   fatalErrorValue("Error converting syslog level str to int", "DEBUG");

    if (0 != strcmp(snoopy_syslog_convert_levelToStr(LOG_EMERG)  , "EMERG")  ) fatalErrorValue("Error converting syslog level int to str", "EMERG");
    if (0 != strcmp(snoopy_syslog_convert_levelToStr(LOG_ALERT)  , "ALERT")  ) fatalErrorValue("Error converting syslog level int to str", "ALERT");
    if (0 != strcmp(snoopy_syslog_convert_levelToStr(LOG_CRIT)   , "CRIT")   ) fatalErrorValue("Error converting syslog level int to str", "CRIT");
    if (0 != strcmp(snoopy_syslog_convert_levelToStr(LOG_ERR)    , "ERR")    ) fatalErrorValue("Error converting syslog level int to str", "ERR");
    if (0 != strcmp(snoopy_syslog_convert_levelToStr(LOG_WARNING), "WARNING")) fatalErrorValue("Error converting syslog level int to str", "WARNING");
    if (0 != strcmp(snoopy_syslog_convert_levelToStr(LOG_NOTICE) , "NOTICE") ) fatalErrorValue("Error converting syslog level int to str", "NOTICE");
    if (0 != strcmp(snoopy_syslog_convert_levelToStr(LOG_INFO)   , "INFO")   ) fatalErrorValue("Error converting syslog level int to str", "INFO");
    if (0 != strcmp(snoopy_syslog_convert_levelToStr(LOG_DEBUG)  , "DEBUG")  ) fatalErrorValue("Error converting syslog level int to str", "DEBUG");


    printSuccess("Mocking src/misc.c complete.");
    return 0;
}
