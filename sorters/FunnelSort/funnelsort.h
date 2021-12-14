typedef int (*cmp_t)(const void *, const void *);

typedef struct funnel {
    struct funnel *lr[2];
    void *in;
    void *out;
    size_t size;
    size_t nmemb;
    size_t index;
    cmp_t cmp;
} funnel;

int comparator_int(const void *a, const void *b);

funnel * funnel_create(void *in, void *out, size_t nmemb, size_t size, cmp_t cmp);

void funnel_clean(funnel *f);

void * funnel_first(funnel *f);

void funnel_fill(funnel *f);

void funnel_sort(void *array, size_t nmemb, size_t size, cmp_t cmp);
