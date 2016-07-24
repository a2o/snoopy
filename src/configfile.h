/*
 * SNOOPY LOGGER
 *
 * File: configfile.h
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
 * Functions to load/parse config file
 */
int   snoopy_configfile_load (char *iniFilePath);
int   snoopy_configfile_parser_callback (
    void* sth,
    const char* section,
    const char* name,
    const char* confValString
);
void  snoopy_configfile_parse_output               (const char *confVal);
void  snoopy_configfile_parse_syslog_facility      (const char *confVal);
void  snoopy_configfile_parse_syslog_level         (const char *confVal);
char *snoopy_configfile_syslog_value_cleanup       (char *confVal);
char *snoopy_configfile_syslog_value_remove_prefix (char *confVal);
void  snoopy_configfile_strtoupper (char *s);
int   snoopy_configfile_getboolean (const char *c, int notfound);
