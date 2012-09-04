/*
 * error.c
 *
 *  Created on: Sep 4, 2012
 *      Author: luan
 */

#include <stdio.h>		/* for convenience */
#include <errno.h>		/* for definition of errno */
#include <stdarg.h>		/* ISO C variable arguments */
#include <stdlib.h>		/* for convenience */
#include <string.h>		/* for convenience */
#include <stddef.h>		/* for offsetof */

#include "log.h"

#define	MAXLINE	4096

/*
 * Error routines for programs that can run as a daemon.
 */

static void	log_doit(int, const char *, va_list ap);

/*
 * Caller must define and set this: nonzero if
 * interactive, zero if daemon
 */
static FILE * g_file = NULL;

/*
 * Initialize syslog(), if running as daemon.
 */
void
log_open(const char *file)
{
	g_file = fopen(file,"a");
}

/*
 * Nonfatal error related to a system call.
 * Print a message with the system's errno value and return.
 */
void
log_ret(const char *fmt, ...)
{
	va_list		ap;

	va_start(ap, fmt);
	log_doit(1, fmt, ap);
	va_end(ap);
}

/*
 * Fatal error related to a system call.
 * Print a message and terminate.
 */
void
log_sys(const char *fmt, ...)
{
	va_list		ap;

	va_start(ap, fmt);
	log_doit(1, fmt, ap);
	va_end(ap);
	exit(2);
}

/*
 * Nonfatal error unrelated to a system call.
 * Print a message and return.
 */
void
log_msg(const char *fmt, ...)
{
	va_list		ap;

	va_start(ap, fmt);
	log_doit(0, fmt, ap);
	va_end(ap);
}

/*
 * Fatal error unrelated to a system call.
 * Print a message and terminate.
 */
void
log_quit(const char *fmt, ...)
{
	va_list		ap;

	va_start(ap, fmt);
	log_doit(0, fmt, ap);
	va_end(ap);
	exit(2);
}

/*
 * Print a message and return to caller.
 * Caller specifies "errnoflag" and "priority".
 */
static void
log_doit(int errnoflag, const char *fmt, va_list ap)
{
	int		errno_save;
	char	buf[MAXLINE];

	errno_save = errno;		/* value caller might want printed */
	vsnprintf(buf, MAXLINE, fmt, ap);
	if (errnoflag){
		snprintf(buf+strlen(buf), MAXLINE-strlen(buf), ": %s",
		  strerror(errno_save));
	}
	strcat(buf, "\n");
	if (g_file == NULL) {
		fflush(stdout);
		fputs(buf, stderr);
		fflush(stderr);
	} else {
		fputs(buf, g_file);
		fflush(g_file);
	}
}
