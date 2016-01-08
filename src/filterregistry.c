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
 * Includes order: from local to global
 */
#include "filterregistry.h"

#include "snoopy.h"
#include "genericregistry.h"



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
#ifdef SNOOPY_CONF_FILTER_ENABLED_only_tty
#include "filter/only_tty.h"
#endif
#ifdef SNOOPY_CONF_FILTER_ENABLED_only_uid
#include "filter/only_uid.h"
#endif

/* This prevents "ISO C forbids empty initializer braces" error */
#include "filter/noop.h"



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
#ifdef SNOOPY_CONF_FILTER_ENABLED_only_tty
    "only_tty",
#endif
#ifdef SNOOPY_CONF_FILTER_ENABLED_only_uid
    "only_uid",
#endif

    /* This prevents "ISO C forbids empty initializer braces" error */
    "noop",
    "",
};

int (*snoopy_filterregistry_ptrs []) (char *logMessage, char const * const arg) = {
#ifdef SNOOPY_CONF_FILTER_ENABLED_exclude_spawns_of
    snoopy_filter_exclude_spawns_of,
#endif
#ifdef SNOOPY_CONF_FILTER_ENABLED_exclude_uid
    snoopy_filter_exclude_uid,
#endif
#ifdef SNOOPY_CONF_FILTER_ENABLED_only_root
    snoopy_filter_only_root,
#endif
#ifdef SNOOPY_CONF_FILTER_ENABLED_only_tty
    snoopy_filter_only_tty,
#endif
#ifdef SNOOPY_CONF_FILTER_ENABLED_only_uid
    snoopy_filter_only_uid,
#endif

    /* This prevents "ISO C forbids empty initializer braces" error */
    snoopy_filter_noop,
};



/*
 * getCount()
 *
 * Return number of available filters
 */
int snoopy_filterregistry_getCount ()
{
    return snoopy_genericregistry_getCount(snoopy_filterregistry_names);
}



/*
 * doesIdExist()
 *
 * Return true if filter exists (by id), otherwise return false
 */
int snoopy_filterregistry_doesIdExist (int filterId)
{
    return snoopy_genericregistry_doesIdExist(snoopy_filterregistry_names, filterId);
}



/*
 * doesNameExist()
 *
 * Return true if filter exists (by name), otherwise return false
 */
int snoopy_filterregistry_doesNameExist (char const * const filterName)
{
    return snoopy_genericregistry_doesNameExist(snoopy_filterregistry_names, filterName);
}



/*
 * getIdFromName()
 *
 * Return index of given filter, or -1 if not found
 */
int snoopy_filterregistry_getIdFromName (char const * const filterName)
{
    return snoopy_genericregistry_getIdFromName(snoopy_filterregistry_names, filterName);
}



/*
 * getName()
 *
 * Return name of given filter, or NULL
 */
char* snoopy_filterregistry_getName (int filterId)
{
    return snoopy_genericregistry_getName(snoopy_filterregistry_names, filterId);
}



/*
 * callById()
 *
 * Call the given filter by id and return its output
 */
int snoopy_filterregistry_callById (int filterId, char *logMessage, char const * const filterArg)
{
    if (SNOOPY_FALSE == snoopy_filterregistry_doesIdExist(filterId)) {
        return -1;
    }

    return snoopy_filterregistry_ptrs[filterId](logMessage, filterArg);
}



/*
 * callByName()
 *
 * Call the given filter by name and return its output
 */
int snoopy_filterregistry_callByName (char const * const filterName, char * logMessage, char const * const filterArg)
{
    int filterId;

    filterId = snoopy_filterregistry_getIdFromName(filterName);
    if (filterId == -1) {
        return -1;
    }

    return snoopy_filterregistry_ptrs[filterId](logMessage, filterArg);
}
