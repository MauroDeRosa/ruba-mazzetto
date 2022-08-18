#include <types/memory.h>
#include <io/log.h>

#include <string.h>

void memory_swap(void *a, void *b, size_t data_size)
{
    check_null_pointer(a);
    check_null_pointer(b);
    void *tmp = memory_allocate(data_size);

    if (tmp == NULL)
    {
        log_error("unable to alloc temporary variable");
    }

    memcpy(tmp, a, data_size);
    memcpy(a, b, data_size);
    memcpy(b, tmp, data_size);
    free(tmp);
}

void* __memory_allocate(size_t heap_size, const char *file, const char* function, size_t line)
{
    void *p = malloc(heap_size);

    if(p==NULL)
    {
        log_error("unable to allocate memory [%s %s:%zu]", function, file, line);
    }

    return p;
}

void* __memory_allocate_typed(size_t n, size_t data_size, const char *file, const char* function, size_t line)
{
    return __memory_allocate(n * data_size, file, function, line);
}

void* __memory_resize(void* base_pointer, size_t heap_size, const char *file, const char* function, size_t line)
{
    check_null_pointer(base_pointer);
    base_pointer = realloc(base_pointer, heap_size);

    if (base_pointer == NULL)
    {
        log_error("unable to reallocate memory [%s %s:%zu]", function, file, line);
    }
    
    return base_pointer;
}

void* __memory_resize_typed(void* base_pointer, size_t n, size_t data_size, const char *file, const char* function, size_t line)
{
    return __memory_resize(base_pointer, n*data_size, file, function, line);
}

void* __memory_allocate_zero(size_t n, size_t data_size, const char *file, const char* function, size_t line)
{
    void *ptr = calloc(n, data_size);

    if (ptr == NULL)
    {
        log_error("unable to allocate zeroed memory [%s %s:%zu]", function, file, line);
    }

    return ptr;
}

void __check_null_pointer(void* pointer, const char *file, const char* function, size_t line)
{
    if(pointer == NULL)
    {
        log_error("pointer can't be NULL [%s %s:%zu]", function, file, line);
    }
}