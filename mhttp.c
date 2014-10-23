/*
 * mhttp.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h> /* for inet_addr(), inet_makeaddr(), and inet_ntoa() */
#include <netinet/in.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#ifdef HAVE_CONFIG_H
# include "config.h"
#else
# define NON_AUTOTOOLS_BUILD_FOR_MHTTP_C 1
#endif /* HAVE_CONFIG_H */

#include "mdnsd.h"
#include "mhttp.h" /* for functions from this file */
#include "msock.h" /* for msock() */
#include "sdtxt.h"

/* conflict! */
void con(char *name, int type, void *arg)
{
    printf("conflicting name detected %s for type %d\n",name,type);
    exit(1);
}

/* quit */
/* (why are these variables global?) */
int _shutdown = 0;
mdnsd _d;
int _zzz[2];
void done(int sig)
{
    _shutdown = 1;
    mdnsd_shutdown(_d);
    write(_zzz[1]," ",1);
}

/* msock() function moved to msock.c */

/* main function gets a separate file, too */

/* EOF */
