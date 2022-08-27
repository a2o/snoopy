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
 */
#include "systemd-snoopy.h"

//#include "snoopy.h"

#include "util/pwd-snoopy.h"

#include <stdlib.h>
#include <string.h>



/*
 * Internal function prototypes
 */
static char const * cgroupEntry_movePastInitialChaff (char const * const cgroupEntry);



/*
 * Local defines
 */
#define SYSTEMD_SLICE_TYPE__INIT   "init.scope"
#define SYSTEMD_SLICE_TYPE__SYSTEM "system.slice/"
#define SYSTEMD_SLICE_TYPE__USER   "user.slice/"



/*
 * Try to convert cgroup info text "1:name=systemd:/..." to Systemd unit name
 *
 * Supports the following formats:
 *      - 1:name=systemd:/
 *      - 1:name=systemd:/init.scope
 *      - 1:name=systemd:/system.slice/dbus.service
 *      - 1:name=systemd:/user.slice/user-1000.slice/session-3178.scope
 *
 * Params:
 *     cgroupInfoText:  The "1:name=systemd:/..." line from /proc/PID/cgroup file
 *
 * Return:
 *     char *:  Returns:
 *              - Pointer to malloc()-ed buffer containing the unit name
 *              - NULL on failure
 */
char * snoopy_util_systemd_convertCgroupEntryToUnitName (char const * const cgroupEntry)
{
    const char * matchPtr = NULL;
    const char * dotPtr = NULL;


    // Get past the chaff (the initial "1:name=systemd:/" part of the cgroup definition)
    matchPtr = cgroupEntry_movePastInitialChaff(cgroupEntry);
    if (!matchPtr) {
        return NULL;
    }


    // Which type of systemd entry is it?
    //
    // Empty content, line "1:name=systemd:/"
    if (*matchPtr == '\0') {
        return strdup("-");


    // Type: init.scope
    } else if (0 == strncmp(matchPtr, SYSTEMD_SLICE_TYPE__INIT, strlen(SYSTEMD_SLICE_TYPE__INIT))) {
        return strdup("init");


    // Type: system.slice/
    } else if (0 == strncmp(matchPtr, SYSTEMD_SLICE_TYPE__SYSTEM, strlen(SYSTEMD_SLICE_TYPE__SYSTEM))) {
        matchPtr += strlen(SYSTEMD_SLICE_TYPE__SYSTEM);

        dotPtr = strchr(matchPtr, '.');
        if ((dotPtr != NULL) && (0 == strcmp(dotPtr, ".service"))) {
            return strndup(matchPtr, dotPtr-matchPtr);
        } else {
            return strdup(matchPtr);
        }


    // Type: user.slice/
    } else if (0 == strncmp(matchPtr, SYSTEMD_SLICE_TYPE__USER, strlen(SYSTEMD_SLICE_TYPE__USER))) {
        matchPtr += strlen(SYSTEMD_SLICE_TYPE__USER);

        return snoopy_util_systemd_convertUserSliceInfoToUsername(matchPtr);
    }


    // Unknown format
    return NULL;
}



/*
 * Convert user slice info text (i.e. "user-1000.slice/session-3178.scope") to username
 *
 * The user slice info text comes from /proc/PID/cgroup file, from the follwing entry
 *      1:name=systemd:/user.slice/user-1000.slice/session-3178.scope
 *
 * Params:
 *     userSliceInfo:   The "user-1000.slice/session-3178.scope" part of the "1:name=systemd:/" line from /proc/PID/cgroup file
 *
 * Return:
 *     char *:  Returns:
 *              - Pointer to malloc()-ed buffer containing the username
 *              - Pointer to malloc()-ed buffer containig "user-UID" string when UID cannot be resolved to username
 *              - NULL on failure
 */
char * snoopy_util_systemd_convertUserSliceInfoToUsername (char const * const userSliceInfo)
{
    const char * matchPtr = userSliceInfo;
    char * dotPtr = NULL;
    uid_t uid;


    // Isolate UID string
    if (0 != strncmp(matchPtr, "user-", 5)) {
        return NULL;
    }
    matchPtr += 5;

    dotPtr = strchr(matchPtr, '.');
    if (dotPtr == NULL) {
        return NULL;
    }
    *dotPtr = '\0';


    // Convert UID string to uid_t
    uid = atoi(matchPtr);


    // Convert UID to username
    return snoopy_util_pwd_convertUidToUsername(uid);
}



/*
 * Take cgroup info text, and return a pointer to where the real content starts
 *
 * Example:
 *      1:name=systemd:/system.slice/dbus.service
 *                      ^
 *                      |
 *                      This is where the return pointer will be pointing to.
 *
 * Params:
 *     cgroupEntry:     The "1:name=systemd:/..." line from /proc/PID/cgroup file
 *
 * Return:
 *     char *:  Returns:
 *              - Pointer to the desired location in the given cgroupEntry text
 *              - NULL on failure
 */
static char const * cgroupEntry_movePastInitialChaff (char const * const cgroupEntry)
{
    char const * matchPtr = cgroupEntry;

    matchPtr = strchr(cgroupEntry, ':');
    if (!matchPtr) {
        return NULL;
    }
    matchPtr++;

    matchPtr = strchr(matchPtr, ':');
    if (!matchPtr) {
        return NULL;
    }
    matchPtr++;

    if (*matchPtr != '/') {
        return NULL;
    }
    matchPtr++;

    return matchPtr;
}
