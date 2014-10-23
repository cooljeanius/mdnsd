/*
 * xht.c
 */

#include "xht.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef HAVE_CONFIG_H
# include "config.h"
#else
# define NON_AUTOTOOLS_BUILD_FOR_XHT_C 1
#endif /* HAVE_CONFIG_H */

typedef struct xhn_struct
{
    char flag;
    struct xhn_struct *next;
    const char *key;
    void *val;
} *xhn;

struct xht_struct
{
    int prime;
    xhn zen;
};

/* Generates a hash code for a string.
 * This function uses the ELF hashing algorithm as reprinted in
 * Andrew Binstock, "Hashing Rehashed," Dr. Dobb's Journal, April 1996.
 */
static int _xhter(const char *s)
{
    /* ELF hash uses unsigned chars and unsigned arithmetic for portability */
    const unsigned char *name = (const unsigned char *)s;
    unsigned long h = 0UL, g;

    while (*name) {
        /* do some fancy bitwanking on the string: */
        h = ((h << 4) + (unsigned long)(*name++));
        if ((g = (h & 0xF0000000UL))!=0) {
            h ^= (g >> 24);
        }
        h &= ~g;

    }

    return (int)h;
}


static xhn _xht_node_find(xhn n, const char *key)
{
    int i;
    for (i = 0; n != 0; n = n->next) {
        if ((n->key != 0) && (strcmp(key, n->key) == 0)) {
            return n;
        }
        if (i >= 0) {
            i++;
        }
    }
    return 0;
}


xht xht_new(int prime)
{
    xht xnew;

    xnew = (xht)malloc(sizeof(struct xht_struct));
    xnew->prime = prime;
    /* array of xhn size of prime: */
    xnew->zen = (xhn)malloc(sizeof(struct xhn_struct) * prime);
    bzero(xnew->zen, (sizeof(struct xhn_struct) * prime));
    return xnew;
}

/* does the set work, used by the xht_set() and xht_store() functions: */
static xhn _xht_set(xht h, const char *key, void *val, char flag)
{
    int i;
    xhn n;

    /* get our index for this key: */
    i = (_xhter(key) % h->prime);

    /* check for existing key first, or find an empty one: */
    if ((n = _xht_node_find(&h->zen[i], key)) == 0) {
        for (n = &h->zen[i]; n != 0; n = n->next) {
            if (n->val == 0) {
                break;
            }
        }
    }

    /* if none, make a new one, link into this index: */
    if (n == 0) {
        n = (xhn)malloc(sizeof(struct xhn_struct));
        /* Try to avoid a clang static analyzer warning about a null
         * pointer dereference (FIXME): */
        if (h->zen[i].next != NULL) {
            n->next = h->zen[i].next; /* what we had originally */
        } else {
            n->next = NULL; /* pretty much the same thing */
        }
        h->zen[i].next = n;
    }

    /* when flag is set, we manage their mem and free them first: */
    if (n->flag) {
        free((void *)n->key);
        free(n->val);
    }

    n->flag = flag;
    n->key = key;
    n->val = val;

    return n; /* hope that this is correct... */
}

void xht_set(xht h, const char *key, void *val)
{
    if ((h == 0) || (key == 0)) {
        return;
    }
    _xht_set(h, key, val, 0);
}

void xht_store(xht h, const char *key, int klen, void *val, int vlen)
{
    char *ckey, *cval;

    if ((h == 0) || (key == 0) || (klen == 0)) {
        return;
    }

    ckey = (char*)malloc(klen + 1);
    memcpy(ckey, key, klen);
    ckey[klen] = '\0';
    cval = (void*)malloc(vlen + 1);
    memcpy(cval, val, vlen);
    cval[vlen] = '\0'; /* convenience, in case it was a string too */
    _xht_set(h, ckey, cval, 1);
    /* avoid a clang static analyzer warning about a memory leak: */
    if (ckey) {
        free((void *)ckey);
    }
    return;
}


void *xht_get(xht h, const char *key)
{
    xhn n;

    if ((h == 0) || (key == 0) ||
        (n = _xht_node_find(&h->zen[_xhter(key) % h->prime], key)) == 0) {
        return 0;
    }

    return n->val;
}


void xht_free(xht h)
{
    xhn n, f;
    int i;

    if (h == 0) {
        return;
    }

    for (i = 0; i < h->prime; i++) {
        for (n = (&h->zen[i])->next; n != 0;) {
            f = n->next;
            if (n->flag) {
                free((void *)n->key);
                free(n->val);
            }
            free(n);
            n = f;
        }
    }

    free(h->zen);
    free(h);
}

void xht_walk(xht h, xht_walker w, void *arg)
{
    int i;
    xhn n;

    if ((h == 0) || (w == 0)) {
        return;
    }

    for (i = 0; i < h->prime; i++) {
        for (n = &h->zen[i]; n != 0; n = n->next) {
            if ((n->key != 0) && (n->val != 0)) {
                (*w)(h, n->key, n->val, arg);
            }
        }
    }

    return;
}

/* EOF */
