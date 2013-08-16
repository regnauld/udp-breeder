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

#ifndef _UDP_BREEDER_H_
#define _UDP_BREEDER_H_

struct sockaddr;

extern char *prog;
extern int   listen_port;

extern void* Malloc(size_t);
extern void *Realloc(void*, size_t);
extern char *Strdup(const char*);

extern int Socket(int,int,int);
extern int Bind(int,const struct sockaddr*, u_int32_t);

extern ssize_t Recvfrom(int, void *, size_t, int, struct sockaddr*, u_int32_t*);

extern int Inet_pton(int, const char*, void*);

extern void plog(const char *, ...);
extern void* prepare(int, char **);
extern int work(void *);

struct forward {
	char *ip_port;
	u_int32_t ip;
	int port;
};

struct control {
	int nf;
	int lsock;
	struct forward *f;
	struct libnet_context *l;
};

#endif
