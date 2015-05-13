/* snoopy.c -- execve() logging wrapper 
 * Copyright (c) 2000 marius@linux.com,mbm@linux.com
 *
 * $Id: snoopy.c 19 2010-02-10 01:45:48Z bostjanskufca $
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
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dlfcn.h>
#include <syslog.h>
#include <string.h>
#include "snoopy.h"

#define min(a,b) a<b ? a : b

#if defined(RTLD_NEXT)
#  define REAL_LIBC RTLD_NEXT
#else
#  define REAL_LIBC ((void *) -1L)
#endif

#define FN(ptr,type,name,args)  ptr = (type (*)args)dlsym (REAL_LIBC, name)



static inline void snoopy_log(const char *filename, char *const argv[])
{
	char *logString       = NULL; 
	int   logStringLength = 0;
	int   i               = 0;
	int   argc            = 0;
	int   argLength       = 0;

	#if SNOOPY_ROOT_ONLY
	if ((geteuid() != 0) && (getuid() != 0)) {
		return;
	}
	#endif

	// Count number of arguments
	for (argc=0 ; *(argv+argc) != '\0' ; argc++);

	// Allocate space
	logStringLength = 0;
	for (i=0 ; i<argc ; i++) {
		// Argument length + space
		logStringLength += sizeof(char) * (min(SNOOPY_MAX_ARG_LENGTH, strlen(argv[i])) + 1);
	}
	logStringLength += 1; // for last \0
	logString        = (char *) malloc(logStringLength);

	// Create logString
	strcpy(logString, "");
	for (i=0 ; i<argc ; i++) {
		argLength = strlen(argv[i]);
		strncat(logString, argv[i], min(SNOOPY_MAX_ARG_LENGTH, argLength));
		strcat(logString, " ");
	}
	strcat(logString, "\0");

	// Log it
	openlog("snoopy", LOG_PID, LOG_AUTHPRIV);
	syslog(LOG_INFO, "[uid:%d sid:%d]: %s", getuid(), getsid(0), logString);

	// Free the logString memory
	free(logString);
}



int execve(const char *filename, char *const argv[], char *const envp[])
{
	static int (*func)(const char *, char **, char **);

	FN(func,int,"execve",(const char *, char **const, char **const));
	snoopy_log(filename, argv);

	return (*func) (filename, (char**) argv, (char **) envp);
}



int execv(const char *filename, char *const argv[]) {
	static int (*func)(const char *, char **);

	FN(func,int,"execv",(const char *, char **const));
	snoopy_log(filename, argv);

	return (*func) (filename, (char **) argv);
}
