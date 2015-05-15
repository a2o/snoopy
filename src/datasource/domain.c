/*
 * SNOOPY LOGGER
 *
 * File: snoopy/datasource/domain.c
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



/* Define required stuff first */
// This needs to be defined before including sys/utsname.h,
// or struct utsname does not return domainname member.
#define   _GNU_SOURCE

/* Snoopy-specific includes second */
#include   "snoopy.h"

/* Generic includes third */
#include   <errno.h>
#include   <stdio.h>
#include   <string.h>
#include   <sys/utsname.h>



/*
 * SNOOPY DATA SOURCE: domain
 *
 * Description:
 *     Returns domain of this system.
 *
 * Params:
 *     result: pointer to string, to write result into
 *
 * Return:
 *     number of characters in the returned string
 */
int snoopy_datasource_domain (char *result, char *arg)
{
    struct utsname unameData;

    if (0 != uname(&unameData)) {
        return snprintf(result, SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE, "(error @ uname(): %d)", errno);
    }

    return snprintf(result, SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE, "%s", unameData.domainname);
}
