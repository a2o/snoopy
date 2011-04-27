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
	char   *logString           = NULL;
	size_t  logStringSize     = 0;
	char    cwd[PATH_MAX+1];
	char   *getCwdRet           = NULL;
	char   *logMessage          = NULL;
	int     logMessageMaxSize   = 0;

	char   *ttyPath         = NULL; 
	char    ttyPathEmpty[]  = ""; 

	#if defined(SNOOPY_EXTERNAL_FILTER)
		FILE   *fp;
		int     status;
		char   *filterCommand          = NULL;
		int     filterCommandMaxSize   = 0;
		char   *filterOutput           = NULL;
		char   *filterOutputPtr        = NULL;
		int     filterOutputMaxSize    = 0;
		char    buffer[PATH_MAX];
		int     bufferSize             = PATH_MAX;
	#endif

	int     i               = 0;
	int     argc            = 0;
	size_t  argLength       = 0;
	int n;


	/* If this stuff should be excluded, don't log it */
	if (strncmp(filename, SNOOPY_EXCLUDE_PREFIX,
		min(strlen(filename), strlen(SNOOPY_EXCLUDE_PREFIX))
	) == 0 ) {
		return;
	}


	/* Also exclude if root-only mode is enabled and this is non-root command */
	#if defined(SNOOPY_ROOT_ONLY)
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
	logStringSize = 0;
	for (i=0 ; i<argc ; i++) {
		/* Argument length + space */
		logStringSize += sizeof(logString[0]) * (strlen(argv[i]) + 1);
	}
	/* +1 for last \0 */
	logStringSize     = min(SNOOPY_MAX_ARG_LENGTH, logStringSize+1);
	logMessageMaxSize = logStringSize + (PATH_MAX * 3);
	logString         = malloc(sizeof *logString * logStringSize);
	logMessage        = malloc(logMessageMaxSize);


	/* Create logString, and protect against overflows */
	logString[0] = '\0';
	for (i = n = 0 ; i<argc ; i++) {
		n += snprintf(logString+n, logStringSize-n, "%s", argv[i]);
		if (n >= logStringSize)
			break;
		logString[n++] = ' ';
	}
	logString[logStringSize-1] = '\0';


	/* Create logMessage */
	#if defined(SNOOPY_CWD_LOGGING)
		getCwdRet = getcwd(cwd, PATH_MAX+1);
		sprintf(logMessage, "[uid:%d sid:%d tty:%s cwd:%s filename:%s]: %s", getuid(), getsid(0), ttyPath, cwd, filename, logString);
	#else
		sprintf(logMessage, "[uid:%d sid:%d tty:%s filename:%s]: %s",        getuid(), getsid(0), ttyPath, filename, logString);
	#endif


	/* Prepare logging stuff */
	openlog("snoopy", LOG_PID, SNOOPY_SYSLOG_FACILITY);


	/* Filter it */
	#if defined(SNOOPY_EXTERNAL_FILTER)
		filterCommandMaxSize = logMessageMaxSize;
		filterOutputMaxSize  = logMessageMaxSize;
		filterCommand = malloc(filterCommandMaxSize);
		filterOutput  = malloc(filterOutputMaxSize);

		/* Check filter command - existance */
		fp = fopen(SNOOPY_EXTERNAL_FILTER_COMMAND, "r");
		if (fp) {
			fclose(fp);
		} else {
			syslog(LOG_ALERT, "External snoopy filter does not exist: %s", SNOOPY_EXTERNAL_FILTER_COMMAND);
			goto extFilterEnd;
		}

		/* Check filter command - executable */
		if (access(SNOOPY_EXTERNAL_FILTER_COMMAND, X_OK)) {
			syslog(LOG_ALERT, "External snoopy filter is not executable: %s", SNOOPY_EXTERNAL_FILTER_COMMAND);
			goto extFilterEnd;
		}

		/* Construct filter command */
		sprintf(filterCommand, "%s '%s'", SNOOPY_EXTERNAL_FILTER_COMMAND, logMessage);

		/* Execute it */
		fp = popen(filterCommand, "r");
		if (fp == NULL) {
			syslog(LOG_ALERT, "Unable to execute external snoopy filter: %s", SNOOPY_EXTERNAL_FILTER_COMMAND);
			goto extFilterEnd;
		}

		/* Read output and close file descriptor */
		filterOutputPtr = filterOutput;
		*filterOutput = '\0';
		while (!feof(fp)) {
			fgets(buffer, bufferSize, fp);

			if ((strlen(filterOutput) + strlen(buffer)) > filterOutputMaxSize) {
				syslog(LOG_ALERT, "Snoopy external filter output is too large");
				goto extFilterEnd;
			}

			strcpy(filterOutputPtr, buffer);
			filterOutputPtr += strlen(buffer);
		}

		/* Finish execution and check exit status */
		status = pclose(fp);
		if (status != 0) {
			syslog(LOG_ALERT, "External snoopy filter returned non-zero status: %d", status);
		}

		/* Move contents to logMessage */
		strcpy(logMessage, filterOutput);
		extFilterEnd:
	#endif   /* defined(SNOOPY_EXTERNAL_FILTER) */


	/* Log it */
	if (strlen(logMessage) > 0) {
		syslog(SNOOPY_SYSLOG_LEVEL, "%s", logMessage);
	}


	/* Free the logString memory */
	free(logString);
	free(logMessage);

	#if defined(SNOOPY_EXTERNAL_FILTER)
		free(filterCommand);
		free(filterOutput);
	#endif
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
