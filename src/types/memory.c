#include <types/memory.h>
#include <io/log.h>

#include <string.h>

void memory_swap(void *a, void *b, size_t data_size)
{
    void *tmp = malloc(data_size);

    if (tmp == NULL)
    {
        log_error("unable to alloc temporary variable");
    }

    memcpy(tmp, a, data_size);
    memcpy(a, b, data_size);
    memcpy(b, tmp, data_size);
    free(tmp);
}

void* memory_allocate(size_t heap_size)
{
    void *p = malloc(heap_size);

    if(p==NULL)
    {
        log_error("unable to allocate memory");
    }

    return p;
}

void* memory_allocate_typed(size_t n, size_t data_size)
{
    return memory_allocate(n * data_size);
}

void* memory_resize(void* base_pointer, size_t heap_size)
{
    base_pointer = realloc(base_pointer, heap_size);

    if (base_pointer == NULL)
    {
        log_error("unable to reallocate memory");
    }
    
    return base_pointer;
}

void* memory_resize_typed(void* base_pointer, size_t n, size_t data_size)
{
    return memory_resize(base_pointer, n*data_size);
}

void __check_null_pointer(void* pointer, const char *file, const char* function, size_t line)
{
    if(pointer == NULL)
    {
        log_error("pointer can't be NULL [%s %s:%zu]", function, file, line);
    }
}