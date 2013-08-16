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

#ifdef __FreeBSD__
    #include <libnet11/libnet.h>
#elif __linux__
    #include <libnet.h>
#endif

#include "udp-breeder.h"

int forward(struct control *c, struct forward *f, u_int32_t ip, int port, char *payload, ssize_t sz);

int
work(void *cnt)
{
	struct control *c = (struct control *)cnt;
	struct sockaddr_in claddr;
	socklen_t len;
	char dgram[8192];
	char *from;
	int from_port;
	u_int32_t from_ip;
	ssize_t sz;
	int i;

	plog("listening on port %d", listen_port);
	for (;;) {
		len = sizeof(claddr);
		sz = Recvfrom(c->lsock, dgram, 8192, 0, (struct sockaddr *)&claddr, &len);

		from = inet_ntoa(claddr.sin_addr);
		from_port = ntohs(claddr.sin_port);
		from_ip = libnet_name2addr4(c->l, from, LIBNET_DONT_RESOLVE);
		plog("got datagram of length %d from %s:%d", sz, from, from_port);

		for (i = 0; i < c->nf; i++) {
			forward(c, &(c->f[i]), from_ip, from_port, dgram, sz);
		}
	}

	return 0;
}

int
forward(struct control *c, struct forward *f, u_int32_t ip, int port, char *payload, ssize_t sz)
{
	libnet_ptag_t p_ip, p_udp;
	p_ip = p_udp = 0;

	libnet_clear_packet(c->l);
	p_udp = libnet_build_udp(
		port, f->port,
		LIBNET_UDP_H + sz,
		0,
		payload,
		sz,
		c->l,
		p_udp);
	if (p_udp < 0) {
		plog("error bulding UDP header: %s", libnet_geterror(c->l));
		return -1;
	}

	p_ip = libnet_build_ipv4(
		LIBNET_IPV4_H + LIBNET_UDP_H + sz,
		0,
		242,
		0,
		64,
		IPPROTO_UDP,
		0,
		ip, f->ip,
		NULL, 0,
		c->l, p_ip);
	if (p_ip < 0) {
		plog("error bulding IP header: %s", libnet_geterror(c->l));
		return -1;
	}

	plog("forwarding packet of size %d to %s",
		libnet_getpacket_size(c->l), f->ip_port);
	if (libnet_write(c->l) < 0)
		plog("error forwarding to %s: %s", f->ip_port, libnet_geterror(c->l));
	return 0;
}
