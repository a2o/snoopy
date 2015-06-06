/*
 * SNOOPY LOGGER
 *
 * File: genericregistry.c
 *
 * Copyright (c) 2015 Bostjan Skufca <bostjan@a2o.si>
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
#include "genericregistry.h"

#include "snoopy.h"

#include <string.h>



/*
 * getCount()
 *
 * Return number of available items
 */
int snoopy_genericregistry_getCount (char *regArray[])
{
    int i;

    i = 0;
    while (strcmp(regArray[i], "") != 0) {
        i++;
    }
    return i;
}



/*
 * doesIdExist()
 *
 * Return true if item exists (by id) in the registry, otherwise return false
 */
int snoopy_genericregistry_doesIdExist (char *regArray[], int itemId)
{
    if ((0 <= itemId ) && (itemId < snoopy_genericregistry_getCount(regArray))) {
        return SNOOPY_TRUE;
    } else {
        return SNOOPY_FALSE;
    }
}



/*
 * doesNameExist()
 *
 * Return true if item exists (by name) in the registry, otherwise return false
 */
int snoopy_genericregistry_doesNameExist (char *regArray[], char const * const itemName)
{
    if (snoopy_genericregistry_getIdFromName(regArray, itemName) == -1) {
        return SNOOPY_FALSE;
    } else {
        return SNOOPY_TRUE;
    }
}



/*
 * getIdFromName()
 *
 * Return id of given item, or -1 if not found
 */
int snoopy_genericregistry_getIdFromName (char *regArray[], char const * const itemName)
{
    for (int i=0 ; 0 != strcmp(regArray[i], "") ; i++) {
        if (strcmp(regArray[i], itemName) == 0) {
            return i;
        }
    }

    /* Not found */
    return -1;
}



/*
 * getName()
 *
 * Return name of given item, or NULL
 */
char* snoopy_genericregistry_getName (char *regArray[], int itemId)
{
    if (snoopy_genericregistry_doesIdExist(regArray, itemId)) {
        return regArray[itemId];
    }

    /* Not found */
    return NULL;
}
