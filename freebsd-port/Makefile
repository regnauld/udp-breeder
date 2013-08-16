PORTNAME=	udp-breeder
PORTVERSION=	1.0
CATEGORIES=	net
MASTER_SITES=	# none
DISTFILES=	# none

MAINTAINER=	tobez@tobez.org
COMMENT=	A UDP packet breeder

BUILD_DEPENDS=  ${LOCALBASE}/lib/libnet11/libnet.a:${PORTSDIR}/net/libnet
LIB_DEPENDS=    net:${PORTSDIR}/net/libnet

WRKSRC=	${WRKDIR}/src

SRC=	${.CURDIR}/../src

do-fetch:
	@${DO_NADA}

pre-patch:
	@${CP} -R ${SRC} ${WRKDIR}

do-install:
	${INSTALL_PROGRAM} ${WRKSRC}/udp-breeder ${PREFIX}/bin/udp-breeder

.include <bsd.port.mk>
