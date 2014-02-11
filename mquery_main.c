/*
 * mquery_main.c
 */

/* Made into a separate file so that the rest of mquery.c can be put into a
 * shared library. This way the mquery program can just compile this file and
 * link it against the shared library, and then there will be no conflict
 * between "main()" functions
 */

#include <errno.h> /* for errno() */
#include <netinet/in.h> /* for struct sockaddr_in */
#include <stdio.h> /* for printf(), and also for strerror() on Darwin */
#include <stdlib.h> /* for atoi() */
#include <string.h> /* for strerror() on Linux */
#include <strings.h> /* for bzero() */
#include <sys/socket.h> /* for recvfrom(), and sendto() */
#ifdef HAVE_CONFIG_H
# include "config.h"
#else
# define NON_AUTOTOOLS_BUILD_FOR_MQUERY_MAIN_C 1
#endif /* HAVE_CONFIG_H */

#include "mdnsd.h" /* for all sorts of mdnsd stuff */
#include "mquery.h" /* for ans() */
#include "msock.h" /* for msock() */

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
