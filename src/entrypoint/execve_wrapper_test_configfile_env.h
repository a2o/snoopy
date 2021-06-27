/*
 * SNOOPY LOGGER
 *
 * File: snoopy/entrypoint/execve_wrapper_test_configfile_env.h
 *
 * Copyright (c) 2020 Bostjan Skufca <bostjan@skufca.si>
 *
 * Part hacked on flight KL 0617, 30,000 ft or so over the Atlantic :)
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
 * Function prototypes
 */
int execv  (const char *filename, char *const argv[]);
int execve (const char *filename, char *const argv[], char *const envp[]);
