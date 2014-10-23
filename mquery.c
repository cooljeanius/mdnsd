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
#   if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#    warning "mquery.c expects a time-related header to be included."
#   endif /* __GNUC__ && !__STRICT_ANSI__ */
#  endif /* HAVE_TIME_H */
# endif /* HAVE_SYS_TIME_H */
#endif /* TIME_WITH_SYS_TIME */

#include "mdnsd.h" /* for mdnsda struct */
#include "mquery.h" /* for functions from this file */
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

/* main function gets a separate file, too */

/* EOF */
