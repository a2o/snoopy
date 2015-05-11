/*
 * SNOOPY LOGGER
 *
 * File: filterregistry.c
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
 * Include relevant header files
 */
#include "snoopy.h"
#include "filterregistry.h"
#include <string.h>



/*
 * Include headers of all filter functions
 */
#ifdef SNOOPY_CONF_FILTER_ENABLED_exclude_spawns_of
#include "filter/exclude_spawns_of.h"
#endif
#ifdef SNOOPY_CONF_FILTER_ENABLED_exclude_uid
#include "filter/exclude_uid.h"
#endif
#ifdef SNOOPY_CONF_FILTER_ENABLED_only_root
#include "filter/only_root.h"
#endif
#ifdef SNOOPY_CONF_FILTER_ENABLED_only_uid
#include "filter/only_uid.h"
#endif



/*
 * Two arrays holding data about filter functions
 */
char *snoopy_filterregistry_names[] = {
#ifdef SNOOPY_CONF_FILTER_ENABLED_exclude_spawns_of
    "exclude_spawns_of",
#endif
#ifdef SNOOPY_CONF_FILTER_ENABLED_exclude_uid
    "exclude_uid",
#endif
#ifdef SNOOPY_CONF_FILTER_ENABLED_only_root
    "only_root",
#endif
#ifdef SNOOPY_CONF_FILTER_ENABLED_only_uid
    "only_uid",
#endif
    "",
};

int (*snoopy_filterregistry_ptrs []) (char *logMessage, char *arg) = {
#ifdef SNOOPY_CONF_FILTER_ENABLED_exclude_spawns_of
    snoopy_filter_exclude_spawns_of,
#endif
#ifdef SNOOPY_CONF_FILTER_ENABLED_exclude_uid
    snoopy_filter_exclude_uid,
#endif
#ifdef SNOOPY_CONF_FILTER_ENABLED_only_root
    snoopy_filter_only_root,
#endif
#ifdef SNOOPY_CONF_FILTER_ENABLED_only_uid
    snoopy_filter_only_uid,
#endif
};



/*
 * isRegistered()
 *
 * Return true if filter filter exists, otherwise return false
 */
int snoopy_filterregistry_isRegistered (char *filterName)
{
    if (snoopy_filterregistry_getIndex(filterName) == -1) {
        return SNOOPY_FALSE;
    } else {
        return SNOOPY_TRUE;
    }
}



/*
 * getIndex()
 *
 * Return index of given filter, or -1 if not found 
 */
int snoopy_filterregistry_getIndex (char *filterName)
{
    int i;

    i = 0;
    while (strcmp(snoopy_filterregistry_names[i], "") != 0) {
        if (strcmp(snoopy_filterregistry_names[i], filterName) == 0) {
            return i;
        }
        i++;
    }
    return -1;
}



/*
 * call()
 *
 * Call the given filter and return its output
 */
int snoopy_filterregistry_call (char *filterName, char *logMessage, char *filterArg)
{
    int idx;

    idx = snoopy_filterregistry_getIndex(filterName);
    if (idx == -1) {
        return -1;
    }

    return snoopy_filterregistry_ptrs[idx](logMessage, filterArg);
}
