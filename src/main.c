/*
 * Copyright (c) 2007-2013 Anton Berezin for bluepipe a/s
 *
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in
 * the documentation and/or other materials provided with the
 * distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <err.h>

#include "udp-breeder.h"

char *prog;
int   listen_port;

static int   nodaemon;
static char *logfile;
static int   quiet;
static FILE *stdlog;

static void usage(int);

int
main(int argc, char **argv)
{
	int opt;
	void *control;

	prog = argv[0];

	while ((opt = getopt(argc, argv, "fhl:p:q")) != EOF) {
		switch (opt) {
		case 'f':
			nodaemon = 1;
			break;
		case 'h':
			usage(0);
			break;
		case 'l':
			logfile = optarg;
			break;
		case 'p':
			listen_port = atoi(optarg);
			break;
		case 'q':
			quiet = 1;
			break;
		default:
			usage(1);
		}
	}
	if (!listen_port)	usage(1);
	if (!nodaemon && !logfile)	quiet = 1;
	if (argc-optind <= 0)	usage(1);

	if (logfile) {
		stdlog = fopen(logfile, "a");
		if (!stdlog)
			err(1, "%s: %s", prog, logfile);
		setvbuf(stdlog, NULL, _IONBF, 0);
	} else {
		stdlog = stderr;
	}

	control = prepare(argc-optind, argv+optind);

	if (!nodaemon)
		if (daemon(0, 0) != 0)
			err(1, "%s: daemonization attempt", prog);

	return work(control);
}

static void
usage(int exitcode)
{
	fprintf(stderr, "usage:\n\t%s -h\n"
		"\t%s -p port -f ip:port ...\n"
		"\t%s -p port -l logfile ip:port ...\n"
		"\t%s -p port -q ip:port ...\n",
		prog, prog, prog, prog);
	exit(exitcode);
}

void
plog(const char *format, ...)
{
	va_list ap;
	char ptime[256];
	time_t t;

	if (quiet)	return;
	va_start(ap, format);
	t = time(NULL);
	strftime(ptime, 255, "%b %d %H:%M:%S", localtime(&t));
	fprintf(stdlog, "%s %s[%d]: ", ptime, prog, getpid());
	vfprintf(stdlog, format, ap);
	fprintf(stdlog, "\n");
	va_end(ap);
}
