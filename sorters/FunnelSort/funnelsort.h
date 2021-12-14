typedef int (*cmp_t)(const void *, const void *);

typedef struct funnel {
    struct funnel *sub_funnels[2];
    void *in;
    void *out;
    size_t size;
    size_t nmemb;
    size_t index;
    cmp_t cmp;
} funnel;

int comparator(const void *a, const void *b);

funnel * create_funnel(void *in, void *out, size_t nmemb, size_t size, cmp_t cmp);

void clean_funnel(funnel *f);

void * get_head_funnel(funnel *f);

void fill_funnel(funnel *f);

void funnel_sort(void *array, size_t nmemb, size_t size, cmp_t cmp);
