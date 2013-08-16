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
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>

#ifdef __FreeBSD__
    #include <libnet11/libnet.h>
#elif __linux__
    #include <libnet.h>
#endif

#include "udp-breeder.h"

void*
prepare(int argc, char **argv)
{
	struct control *c;
	struct forward *f;
	char *s_port;
	int port;
	char libnet_errbuf[LIBNET_ERRBUF_SIZE];
	struct sockaddr_in servaddr;

	c = Malloc(sizeof(*c));
	c->lsock = Socket(AF_INET, SOCK_DGRAM, 0);
	c->l = libnet_init(LIBNET_RAW4, NULL, libnet_errbuf);
	if (!c->l)
		errx(1, "%s: libnet_init() failed: %s", prog, libnet_errbuf);

	while (argc--) {
		c->nf++;
		c->f = Realloc(c->f, sizeof(*f)*c->nf);
		f = &c->f[c->nf-1];
		bzero(f, sizeof(*f));
		f->ip_port = Strdup(argv[0]);
		s_port = strchr(argv[0], ':');
		if (!s_port)
			errx(1, "%s: bad address:port %s", prog, argv[0]);
		*s_port = '\0';
		s_port++;
		f->ip = libnet_name2addr4(c->l, argv[0], LIBNET_DONT_RESOLVE);
		if (f->ip == 0xffffffff)
			errx(1, "%s: bad address %s", prog, argv[0]);
		if ((port = atoi(s_port)) == 0)
			errx(1, "%s: bad port number %s", prog, s_port);
		f->port = port;

		argv++;
	}

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(listen_port);

	Bind(c->lsock, (struct sockaddr*)&servaddr, sizeof(servaddr));

	return (void *)c;
}
