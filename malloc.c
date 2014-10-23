#include <errno.h>
#include <limits.h>
#include <string.h>

#include "malloc.h"
#include "memreq.h"



typedef struct header {
    size_t size;

}

void *malloc(size_t size) {
    struct header* ptr = (struct header*) get_memry(size + sizeof(struct header));
    ptr->size = size;
    return (void*) (ptr + 1);
}

static size_t highest(size_t in) {
    size_t num_bits = 0;

    while (in != 0) {
        ++num_bits;
        in >>= 1;
    }

    return num_bits;
}

void* calloc(size_t number, size_t size) {
    size_t number_size = 0;

    /* This prevents an integer overflow.  A size_t is a typedef to an integer
     * large enough to index all of memory.  If we cannot fit in a size_t, then
     * we need to fail.
     */
    if (highest(number) + highest(size) > sizeof(size_t) * CHAR_BIT) {
        errno = ENOMEM;
        return NULL;
    }

    number_size = number * size;
    void* ret = malloc(number_size);

    if (ret) {
        memset(ret, 0, number_size);
    }

    return ret;
}

void* realloc(void *ptr, size_t size) {
    size_t old_size = 0; /* XXX Set this to the size of the buffer pointed to by ptr */
    void* ret = malloc(size);

    if (ret) {
        if (ptr) {
            memmove(ret, ptr, old_size < size ? old_size : size);
            free(ptr);
        }

        return ret;
    } else {
        errno = ENOMEM;
        return NULL;
    }
}

void free(void* ptr) {
}
