/*
 * SNOOPY COMMAND LOGGER
 *
 * File: snoopy/datasource/tty_username.c
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
#include "tty_username.h"
#include "tty__common.h"
#include "tty.h"

#include "snoopy.h"

#include "util/pwd-snoopy.h"

#include <stdio.h>
#include <stdlib.h>



/*
 * SNOOPY DATA SOURCE: tty_username
 *
 * Description:
 *     Returns litaral username of current controlling terminal
 *
 * Params:
 *     result: pointer to string, to write result into
 *     arg:    (ignored)
 *
 * Return:
 *     number of characters in the returned string, or SNOOPY_DATASOURCE_FAILURE
 */
int snoopy_datasource_tty_username (char * const result, __attribute__((unused)) char const * const arg)
{
    int     retVal;
    uid_t   ttyUid;
    char   *username = NULL;
    int     retMsgLen = 0;


    /* Get tty UID */
    retVal = snoopy_datasource_tty__get_tty_uid(&ttyUid, result);
    if (retVal > 0) {
        return retVal;   // Error occurred, and the message about it is already in the result buffer
    }


    username = snoopy_util_pwd_convertUidToUsername(ttyUid);
    if (username == NULL) {
        return snprintf(result, SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE, "Unable to convert UID to username");
    }


    retMsgLen = snprintf(result, SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE, "%s", username);
    free(username);
    return retMsgLen;
}
