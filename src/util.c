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
#include <string.h>
#include <strings.h>
#include <err.h>

#include "udp-breeder.h"

void *
Malloc(size_t size)
{
	void *r = malloc(size);
	if (!r)
		err(1, "%s: malloc", prog);
	bzero(r, size);
	return r;
}

int
Socket(int domain, int type, int protocol)
{
	int r = socket(domain, type, protocol);
	if (r < 0)
		err(1, "%s: socket", prog);
	return r;
}

int
Bind(int s, const struct sockaddr *addr, socklen_t addrlen)
{
	int r = bind(s, addr, addrlen);
	if (r != 0)
		err(1, "%s: bind", prog);
	return r;
}

ssize_t
Recvfrom(int s, void *buf, size_t len, int flags,
		 struct sockaddr *from, socklen_t *fromlen)
{
	ssize_t r;

	r = recvfrom(s, buf, len, flags, from, fromlen);
	if (r < 0)
		err(1, "%s: recvfrom", prog);
	return r;
}

char *
Strdup(const char *str)
{
	char *r = strdup(str);
	if (!r)
		err(1, "%s: strdup", prog);
	return r;
}

void *
Realloc(void* ptr, size_t size)
{
	void *r = realloc(ptr, size);
	if (!r)
		err(1, "%s: realloc", prog);
	return r;
}

int
Inet_pton(int af, const char *src, void *dst)
{
	int r;
	r = inet_pton(af, src, dst);
	if (r == 1)
		return r;
	if (r < 0)
		err(1, "%s: inet_pton", prog);
	errx(1, "%s: inet_pton: bad address", prog);
	return 0;
}
