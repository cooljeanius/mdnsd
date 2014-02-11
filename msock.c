/*
 * msock.c
 */

#include <arpa/inet.h> /* for htonl(), htons(), and inet_addr() */
#include <fcntl.h> /* for fcntl() */
#include <strings.h> /* for bzero() */
#include <sys/socket.h> /* for setsockopt() */
#include <unistd.h> /* for close() */
#ifdef HAVE_CONFIG_H
# include "config.h"
#else
# define NON_AUTOTOOLS_BUILD_FOR_SOCK_C 1
#endif /* HAVE_CONFIG_H */

#include "msock.h"

/* this function was the same in both mhttp.c and mquery.c */
/* split into separate file to reduce code duplication */

/* create multicast 224.0.0.251:5353 socket */
int msock()
{
    int s, flag = 1, ittl = 255;
    struct sockaddr_in in;
    struct ip_mreq mc;
    char ttl = 255;

    bzero(&in, sizeof(in));
    in.sin_family = AF_INET;
    in.sin_port = htons(5353);
    in.sin_addr.s_addr = 0;

    if((s = socket(AF_INET,SOCK_DGRAM,0)) < 0) return 0;
#ifdef SO_REUSEPORT
    setsockopt(s, SOL_SOCKET, SO_REUSEPORT, (char*)&flag, sizeof(flag));
#endif /* SO_REUSEPORT */
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char*)&flag, sizeof(flag));
    if(bind(s,(struct sockaddr*)&in,sizeof(in))) { close(s); return 0; }

    mc.imr_multiaddr.s_addr = inet_addr("224.0.0.251");
    mc.imr_interface.s_addr = htonl(INADDR_ANY);
    setsockopt(s, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mc, sizeof(mc));
    setsockopt(s, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof(ttl));
    setsockopt(s, IPPROTO_IP, IP_MULTICAST_TTL, &ittl, sizeof(ittl));

    flag =  fcntl(s, F_GETFL, 0);
    flag |= O_NONBLOCK;
    fcntl(s, F_SETFL, flag);

    return s;
}

/* EOF */
