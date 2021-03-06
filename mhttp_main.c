/*
 * mhttp_main.c
 */

/* Made into a separate file so that the rest of mhttp.c can be put into a
 * shared library. This way the mhttp program can just compile this file and
 * link it against the shared library, and then there will be no conflict
 * between "main()" functions
 */

#include <arpa/inet.h> /* for inet_addr(), inet_makeaddr(), and inet_ntoa() */
#include <errno.h> /* for errno() */
#include <netinet/in.h> /* for struct sockaddr_in */
#include <signal.h> /* for signal stuff */
#include <stdio.h> /* for printf(), sprintf(), and strerror() */
#include <stdlib.h> /* for atoi(), and free() */
#include <string.h> /* for strlen() */
#include <strings.h> /* for bzero() */
#include <sys/socket.h> /* for recvfrom() */
#include <unistd.h> /* for pipe() */
#ifdef HAVE_CONFIG_H
# include "config.h"
#else
# define NON_AUTOTOOLS_BUILD_FOR_MHTTP_MAIN_C 1
#endif /* HAVE_CONFIG_H */

#include "mdnsd.h" /* for all sorts of mdnsd stuff */
#include "mhttp.h" /* for con() and done() */
#include "msock.h" /* for msock() */
#include "sdtxt.h" /* for sd2txt() */
#include "xht.h" /* for xht_new(), xht_free() */

/* declared as global in mhttp.c
 * not sure if safe to put in the header (mhttp.h)...
 */
int _shutdown = 0;
mdnsd _d;
int _zzz[2];

int main(int argc, char *argv[])
{
    mdnsd d;
    mdnsdr r;
    struct message m;
#ifdef HAVE_IN_ADDR_T
    in_addr_t ip;
#else
    unsigned long int ip;
#endif /* HAVE_IN_ADDR_T */
#ifdef HAVE_IN_ADDR_T
    in_addr_t local_ip;
#else
    unsigned long int local_ip;
#endif /* HAVE_IN_ADDR_T */
    unsigned short int port;
    struct in_addr my_in_addr_struct;
    struct timeval *tv;
    int bsize, ssize = sizeof(struct sockaddr_in);
    unsigned char buf[MAX_PACKET_LEN];
    struct sockaddr_in from, to;
    fd_set fds;
    int s;
    unsigned char *packet, hlocal[256], nlocal[256];
    int len = 0;
    xht h;

    if (argc < 4) {
        printf("usage: mhttp 'unique name' 12.34.56.78 80 '/optionalpath'\n");
        return 1;
    }

    ip = inet_addr(argv[2]);
    local_ip = inet_addr("127.0.0.1");
    port = atoi(argv[3]);
    /* not sure if this will work, as the second argument is made up: */
    my_in_addr_struct = inet_makeaddr(ip, local_ip);
    printf("Announcing .local site named '%s' to %s:%d and extra path '%s'\n",
           argv[1], inet_ntoa(my_in_addr_struct), port, argv[4]);

    signal(SIGINT, done);
    signal(SIGHUP, done);
    signal(SIGQUIT, done);
    signal(SIGTERM, done);
    pipe(_zzz);
    _d = d = mdnsd_new(1,1000);
    if ((s = msock()) == 0) {
        printf("cannot create socket: %s\n", strerror(errno));
        return 1;
    }

    sprintf(hlocal, "%s._http._tcp.local.", argv[1]);
    sprintf(nlocal, "http-%s.local.", argv[1]);
    r = mdnsd_shared(d, "_http._tcp.local.", QTYPE_PTR, 120);
    mdnsd_set_host(d, r, hlocal);
    r = mdnsd_unique(d, hlocal, QTYPE_SRV, 600, con, 0);
    mdnsd_set_srv(d, r, 0, 0, port, nlocal);
    r = mdnsd_unique(d, nlocal, QTYPE_A, 600, con, 0);
    mdnsd_set_raw(d, r, (unsigned char *)&ip, 4);
    r = mdnsd_unique(d, hlocal, 16, 600, con, 0);
    h = xht_new(11);
    if ((argc == 5) && argv[4] && (strlen(argv[4]) > 0)) {
        xht_set(h, "path", argv[4]);
    }
    packet = sd2txt(h, &len);
    xht_free(h);
    mdnsd_set_raw(d, r, packet, len);
    free(packet);

    while (1) {
        tv = mdnsd_sleep(d);
        FD_ZERO(&fds);
        FD_SET(_zzz[0], &fds);
        FD_SET(s, &fds);
        select((s + 1), &fds, 0, 0, tv);

        /* only used when we wake-up from a signal, shutting down: */
        if (FD_ISSET(_zzz[0], &fds)) {
            read(_zzz[0], buf, MAX_PACKET_LEN);
        }

        if (FD_ISSET(s, &fds)) {
            while ((bsize = recvfrom(s, buf, MAX_PACKET_LEN, 0,
                                     (struct sockaddr*)&from,
                                     &ssize)) > 0) {
                bzero(&m, sizeof(struct message));
                message_parse(&m, buf);
                mdnsd_in(d, &m, (unsigned long int)from.sin_addr.s_addr,
                         from.sin_port);
            }
            if ((bsize < 0) && (errno != EAGAIN)) {
                printf("cannot read from socket %d: %s\n", errno,
                       strerror(errno));
                return 1;
            }
        }
        /* in case 'ip' is of type 'in_addr_t', cast it: */
        while (mdnsd_out(d, &m, (unsigned long int *)&ip, &port)) {
            bzero(&to, sizeof(to));
            to.sin_family = AF_INET;
            to.sin_port = port;
            to.sin_addr.s_addr = ip;
            if (sendto(s, message_packet(&m), message_packet_len(&m), 0,
                       (struct sockaddr *)&to,
                       sizeof(struct sockaddr_in)) != message_packet_len(&m)) {
                printf("cannot write to socket: %s\n", strerror(errno));
                return 1;
            }
        }
        if (_shutdown) {
            break;
        }
    }

    mdnsd_shutdown(d);
    mdnsd_free(d);
    return 0;
}

/* EOF */
