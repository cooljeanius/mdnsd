/*
 * mquery.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> /* for inet_addr(), inet_makeaddr(), and inet_ntoa() */
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#ifdef HAVE_CONFIG_H
# include "config.h"
#else
# define NON_AUTOTOOLS_BUILD_FOR_MQUERY_C 1
#endif /* HAVE_CONFIG_H */

#ifdef TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# ifdef HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  ifdef HAVE_TIME_H
#   include <time.h>
#  else
#   warning mquery.c expects a time-related header to be included.
#  endif /* HAVE_TIME_H */
# endif /* HAVE_SYS_TIME_H */
#endif /* TIME_WITH_SYS_TIME */

#include "mdnsd.h" /* for mdnsda struct */
#include "msock.h" /* for msock() */

/* print an answer */
int ans(mdnsda a, void *arg)
{
    int now;
#ifdef HAVE_IN_ADDR_T
    in_addr_t local_ip;
#else
    unsigned long int local_ip;
#endif /* HAVE_IN_ADDR_T */
    local_ip = inet_addr("127.0.0.1");
    struct in_addr my_in_addr_struct;
    if(a->ttl == 0) now = 0;
    else now = a->ttl - time(0);
    /* not sure if this will work, as the second argument is made up: */
    my_in_addr_struct = inet_makeaddr(a->ip, local_ip);
    switch(a->type) {
    case QTYPE_A:
        printf("A %s for %d seconds to ip %s\n",a->name,now,inet_ntoa(my_in_addr_struct));
        break;
    case QTYPE_PTR:
        printf("PTR %s for %d seconds to %s\n",a->name,now,a->rdname);
        break;
    case QTYPE_SRV:
        printf("SRV %s for %d seconds to %s:%d\n",a->name,now,a->rdname,a->srv.port);
        break;
    default:
        printf("%d %s for %d seconds with %d data\n",a->type,a->name,now,a->rdlen);
    }
	return 0;
}

/* msock() function moved to msock.c */

/* main function */
int main(int argc, char *argv[])
{
    mdnsd d;
    struct message m;
    unsigned long int ip;
    unsigned short int port;
    struct timeval *tv;
    int bsize, ssize = sizeof(struct sockaddr_in);
    unsigned char buf[MAX_PACKET_LEN];
    struct sockaddr_in from, to;
    fd_set fds;
    int s;

    if(argc != 3) { printf("usage: mquery 12 _http._tcp.local.\n"); return 1; }

    d = mdnsd_new(1,1000);
    if((s = msock()) == 0) { printf("cannot create socket: %s\n",strerror(errno)); return 1; }

    mdnsd_query(d,argv[2],atoi(argv[1]),ans,0);

    while(1) {
        tv = mdnsd_sleep(d);
        FD_ZERO(&fds);
        FD_SET(s,&fds);
        select(s+1,&fds,0,0,tv);

        if(FD_ISSET(s,&fds)) {
            while((bsize = recvfrom(s,buf,MAX_PACKET_LEN,0,(struct sockaddr*)&from,&ssize)) > 0)
            {
                bzero(&m,sizeof(struct message));
                message_parse(&m,buf);
                mdnsd_in(d,&m,(unsigned long int)from.sin_addr.s_addr,from.sin_port);
            }
            if(bsize < 0 && errno != EAGAIN) { printf("cannot read from socket %d: %s\n",errno,strerror(errno)); return 1; }
        }
        while(mdnsd_out(d,&m,&ip,&port)) {
            bzero(&to, sizeof(to));
            to.sin_family = AF_INET;
            to.sin_port = port;
            to.sin_addr.s_addr = ip;
            if(sendto(s,message_packet(&m),message_packet_len(&m),0,(struct sockaddr *)&to,sizeof(struct sockaddr_in)) != message_packet_len(&m))  { printf("cannot write to socket: %s\n",strerror(errno)); return 1; }
        }
    }

    mdnsd_shutdown(d);
    mdnsd_free(d);
    return 0;
}

/* EOF */
