/* Adopted from https://github.com/prokaktus/funnelsort and adjusted accordingly */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define L 64
#include "funnelsort.h"

int comparator_int(const void *a, const void *b) {
    return (*(int *)a < *(int *)b ? -1 : 1);
}

funnel * funnel_create(void *in, void *out, size_t nmemb, size_t size, cmp_t cmp) {
    funnel *f = (funnel *)malloc(sizeof(funnel));

    f->out = out;
    f->nmemb = nmemb;
    f->size = size; 
    f->cmp = cmp;
    f->in = in;
    f->index = 0;

    if ((f->nmemb) * size > L) {
        size_t nmemb_left = nmemb / 2;
        size_t nmemb_right = nmemb - nmemb_left;
        void *out_left = malloc(nmemb_left * f->size);
        void *out_right = malloc(nmemb_right * f->size);
        f->lr[0] = funnel_create(in, out_left, nmemb_left, f->size, cmp);
        f->lr[1] = funnel_create((char *)in + nmemb_left * size, out_right,
                                 nmemb_right, f->size, cmp);
    } else {
        f->lr[0] = NULL;
        f->lr[1] = NULL;
    }
    return f;
}

void funnel_clean(funnel *f) {
    if (f) {
        if (f->out) {
            free(f->out);
        }
        if (f->lr[0]) {
            funnel_clean(f->lr[0]);
        } 
        if (f->lr[1]) {
            funnel_clean(f->lr[1]);
        }
        free(f);
    }
}

void * funnel_first(funnel *f) {
    if (f->index < f->nmemb) {
        return (void *)((char *)f->out + f->size * f->index);
    }
    return NULL;
}

void funnel_fill(funnel *f) {
    if (f->lr[0] == NULL || f->lr[1] == NULL) {
        qsort(f->in, f->nmemb, f->size, f->cmp);
        memcpy(f->out, f->in, f->nmemb * f->size);
        return;
    }
    funnel_fill(f->lr[0]);
    funnel_fill(f->lr[1]);
    // perform merge
    size_t i1 = f->lr[0]->index, i2 = f->lr[1]->index;
    size_t n1 = f->lr[0]->nmemb, n2 = f->lr[1]->nmemb;
    void *e1, *e2;
    size_t index = 0;

    e1 = funnel_first(f->lr[0]);
    e2 = funnel_first(f->lr[1]);
    while (i1 < n1 && i2 < n2) {
        if (f->cmp(e1, e2) < 0) {
            memcpy((char *)f->out + index * f->size, e1, f->size);
            e1 = funnel_first(f->lr[0]);
            i1++;
        }
        else {
            memcpy((char *)f->out + index * f->size, e2, f->size);
            e2 = funnel_first(f->lr[1]);
            i2++;
        }
        index++;
    }
    while (i1 < n1) {
        e1 = funnel_first(f->lr[0]);
        memcpy((char *)f->out + index * f->size, e1, f->size);
        i1++;
        index++;
    }
    while (i2 < n2) {
        e2 = funnel_first(f->lr[1]);
        memcpy((char *)f->out + index * f->size, e2, f->size);
        i2++;
        index++;
    }

    return;
}

void funnel_sort(void *array, size_t nmemb, size_t size, cmp_t cmp) {
    void *out = malloc(size * nmemb);
    funnel *funn = funnel_create(array, out, nmemb, size, cmp);
    if (!funn) {
        fprintf(stdout, "Error while creation funnel\n");
        return;
    }
    funnel_fill(funn);
    memcpy(array, out, size * nmemb);
    funnel_clean(funn);
}
