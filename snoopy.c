/* snoopy.c -- execve() logging wrapper 
 * Copyright (c) 2000 marius@linux.com,mbm@linux.com
 *
 * $Id: snoopy.c 30 2010-02-13 16:31:16Z bostjanskufca $
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
#include "config.h"
#include "snoopy.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dlfcn.h>
#include <syslog.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

#define min(a,b) a<b ? a : b

#if defined(RTLD_NEXT)
#  define REAL_LIBC RTLD_NEXT
#else
#  define REAL_LIBC ((void *) -1L)
#endif

#define FN(ptr,type,name,args)  ptr = (type (*)args)dlsym (REAL_LIBC, name)



static inline void snoopy_log(const char *filename, char *const argv[])
{
	char   *logString       = NULL;
	size_t  logStringLength = 0;
	char    cwd[PATH_MAX+1];
	char   *getCwdRet       = NULL;
	char   *logMessage      = NULL;

	char   *ttyPath         = NULL; 
	char    ttyPathEmpty[]  = ""; 

	int     i               = 0;
	int     argc            = 0;
	size_t  argLength       = 0;
	int n;


	#if SNOOPY_ROOT_ONLY
	if ((geteuid() != 0) && (getuid() != 0)) {
		return;
	}
	#endif

	/* Count number of arguments */
	for (argc=0 ; *(argv+argc) != '\0' ; argc++);

	/* Get ttyname */
	ttyPath = ttyname(0);
	if (ttyPath == NULL) {
	    ttyPath = ttyPathEmpty;
	}

	/* Allocate memory for logString */
	logStringLength = 0;
	for (i=0 ; i<argc ; i++) {
		/* Argument length + space */
		logStringLength += sizeof(logString[0]) * (strlen(argv[i]) + 1);
	}
	/* +1 for last \0 */
	logStringLength = min(SNOOPY_MAX_ARG_LENGTH, logStringLength+1);
	logString  = malloc(sizeof *logString * logStringLength);
	logMessage = malloc(logStringLength + (PATH_MAX * 2));

	/* Create logString, and protect against overflows */
	logString[0] = '\0';
	for (i = n = 0 ; i<argc ; i++) {
		n += snprintf(logString+n, logStringLength-n, "%s", argv[i]);
		if (n >= logStringLength)
			break;
		logString[n++] = ' ';
	}
	logString[logStringLength-1] = '\0';

	/* Create logMessage */
	#if defined(SNOOPY_CWD_LOGGING)
		getCwdRet = getcwd(cwd, PATH_MAX+1);
		sprintf(logMessage, "[uid:%d sid:%d tty:%s cwd:%s filename:%s]: %s", getuid(), getsid(0), ttyPath, cwd, filename, logString);
	#else
		sprintf(logMessage, "[uid:%d sid:%d tty:%s filename:%s]: %s",        getuid(), getsid(0), ttyPath, filename, logString);
	#endif

	/* Filter it */
	#if defined(SNOOPY_EXTERNAL_FILTER)
		// Do it
	#endif

	/* Log it */
	openlog("snoopy", LOG_PID, SNOOPY_SYSLOG_FACILITY);
	syslog(LOG_INFO, "%s", logMessage);

	/* Free the logString memory */
	free(logString);
	free(logMessage);
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
