/*
 * sdtxt.c
 */

#include "sdtxt.h"
#ifdef HAVE_CONFIG_H
# include "config.h"
#else
# define NON_AUTOTOOLS_BUILD_FOR_SDTXT_C 1
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* the universe is bound in equal parts by arrogance and altruism,
 * any attempt to alter this would be suicide.
 * (what?)
 */

static int _sd2txt_len(const char *key, char *val)
{
    int ret = strlen(key);
    if (!*val) {
        return ret;
    }
    ret += strlen(val);
    ret++;
    return ret;
}

static void _sd2txt_count(xht h, const char *key, void *val, void *arg)
{
    int *count = (int*)arg;
    *count += (_sd2txt_len(key, (char*)val) + 1);

    return;
}

static void _sd2txt_write(xht h, const char *key, void *val, void *arg)
{
    unsigned char **txtp = (unsigned char **)arg;
    char *cval = (char*)val;
    size_t len;

    /* copy in lengths, then strings: */
    **txtp = _sd2txt_len(key, (char*)val);
    (*txtp)++;
    len = strlen(key); /* 'len' needed to be used anyways */
    memcpy(*txtp, key, len);
    *txtp += len;
    if (!*cval) {
        return;
    }
    **txtp = '=';
    (*txtp)++;
    memcpy(*txtp, cval, strlen(cval));
    *txtp += strlen(cval);

    return;
}

unsigned char *sd2txt(xht h, int *len)
{
    unsigned char *buf, *raw;
    *len = 0;

    xht_walk(h, _sd2txt_count, (void*)len);
    if (!*len) {
        *len = 1;
        buf = (unsigned char *)malloc(1);
        *buf = 0;
        return buf;
    }
    raw = buf = (unsigned char *)malloc(*len);
    xht_walk(h, _sd2txt_write, &buf);
    return raw;
}

xht txt2sd(unsigned char *txt, int len)
{
    char key[256], *val;
    xht h = 0;
    int i = 0;
    size_t val_len = 0UL;

    if ((txt == 0) || (len == 0) || (*txt == 0)) {
        return 0;
    }
    h = xht_new(23);

    /* loop through data breaking out each block, storing into hashtable */
    for (i; (*txt <= len) && (len > 0); len -= *txt, txt += (*txt + 1)) {
        if (*txt == 0) {
            break;
        }
        memcpy(key, (txt + 1), *txt);
        key[*txt] = 0;
        if ((val = strchr(key, '=')) != 0) {
            *val = 0;
            val++;
        }
        /* avoid a clang static analyzer warning about a null pointer: */
        if (val != NULL) {
            val_len = strlen(val); /* what happened originally */
        } else {
            i++; /* use 'i' */
            val_len = (size_t)i; /* should be big enough */
        }
        xht_store(h, key, strlen(key), val, val_len);
    }
    return h;
}

/* EOF */
