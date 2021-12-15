/* Adopted from https://github.com/prokaktus/funnelsort and adjusted accordingly */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define L 64
#include "funnelsort.h"

int comparator(const void *a, const void *b) {
    return (*(int *)a < *(int *)b ? -1 : 1);
}

funnel * create_funnel(void *in, void *out, size_t nmemb, size_t size, cmp_t cmp) {
    funnel *f = (funnel *)malloc(sizeof(funnel));

    f->out = out;
    f->nmemb = nmemb;
    f->size = size; 
    f->cmp = cmp;
    f->in = in;
    f->index = 0;

    if ((f->nmemb) * size <= L) { // base case
        f->sub_funnels[0] = NULL;
        f->sub_funnels[1] = NULL;
    } else { // recursive step
        size_t nmemb_left = nmemb / 2;
        size_t nmemb_right = nmemb - nmemb_left;
        void *out_left = malloc(nmemb_left * f->size);
        void *out_right = malloc(nmemb_right * f->size);
        // create sub-funnels
        f->sub_funnels[0] = create_funnel(in, out_left, nmemb_left, f->size, cmp);
        f->sub_funnels[1] = create_funnel((char *)in + nmemb_left * size, out_right,
                                 nmemb_right, f->size, cmp);
    }
    return f;
}

void clean_funnel(funnel *f) {
    // free funnels memory
    if (f) {
        if (f->out) {
            free(f->out);
        }
        if (f->sub_funnels[0]) {
            clean_funnel(f->sub_funnels[0]);
        } 
        if (f->sub_funnels[1]) {
            clean_funnel(f->sub_funnels[1]);
        }
        free(f);
    }
}

void * get_funnel_head(funnel *f) {
    // get the head of the buffer
    if (f->index < f->nmemb) {
        return (void *)((char *)f->out + f->size * f->index);
    }
    return NULL;
}

void fill_funnel(funnel *f) {
    // base case, sort the elements in the funnel
    if (f->sub_funnels[0] == NULL || f->sub_funnels[1] == NULL) { 
        qsort(f->in, f->nmemb, f->size, f->cmp);
        memcpy(f->out, f->in, f->nmemb * f->size);
        return;
    }
    // recursive calls to fill the buffers 
    fill_funnel(f->sub_funnels[0]);
    fill_funnel(f->sub_funnels[1]);
    
    size_t index1 = f->sub_funnels[0]->index; 
    size_t nmemb1 = f->sub_funnels[0]->nmemb;
    size_t index2 = f->sub_funnels[1]->index;
    size_t nmemb2 = f->sub_funnels[1]->nmemb;
    void *head1, *head2;
    size_t index = 0;

    head1 = get_funnel_head(f->sub_funnels[0]);
    head2 = get_funnel_head(f->sub_funnels[1]);

    // merge step
    while (index1 < nmemb1 && index2 < nmemb2) {
        if (f->cmp(head1, head2) < 0) {
            memcpy((char *)f->out + index * f->size, head1, f->size);
            head1 = get_funnel_head(f->sub_funnels[0]);
            index1++;
        }
        else {
            memcpy((char *)f->out + index * f->size, head2, f->size);
            head2 = get_funnel_head(f->sub_funnels[1]);
            index2++;
        }
        index++;
    }
    while (index1 < nmemb1) {
        head1 = get_funnel_head(f->sub_funnels[0]);
        memcpy((char *)f->out + index * f->size, head1, f->size);
        index1++;
        index++;
    }
    while (index2 < nmemb2) {
        head2 = get_funnel_head(f->sub_funnels[1]);
        memcpy((char *)f->out + index * f->size, head2, f->size);
        index2++;
        index++;
    }
    return;
}

void funnel_sort(void *array, size_t nmemb, size_t size, cmp_t cmp) {
    void *sorted = malloc(size * nmemb);
    funnel *f = create_funnel(array, sorted, nmemb, size, cmp);
    if (!f) {
        printf("Error in create_funnel!\n");
        return;
    }
    fill_funnel(f);
    memcpy(array, sorted, size * nmemb);
    clean_funnel(f);
}
