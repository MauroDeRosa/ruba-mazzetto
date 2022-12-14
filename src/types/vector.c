/**
 * @file vector.c
 * @author Mauro De Rosa (it.mauro.derosa@gmail.com)
 * @brief vector module implementation @see vector.h
 * @version 1.0.0 alpha
 * @date 2022-08-24
 * 
 * @copyright Copyright (c) 2022
 * @license licensed under MIT license
 * 
 */

#include <types/vector.h>
#include <types/array.h>
#include <types/memory.h>
#include <io/log.h>

#define DEFAULT_VECTOR_SIZE 20 ///< how big is a vector when initialized

vector* generic_vector_init(size_t element_size)
{
    vector* v = memory_allocate_typed(1, sizeof(vector));
    v->element_size = element_size;
    v->count = 0;
    v->size = DEFAULT_VECTOR_SIZE;
    v->elements = memory_allocate_typed(v->size, v->element_size);
    return v;
}

void generic_vector_resize(vector *v, size_t elements_count)
{
    check_null_pointer(v);
    v->size = elements_count;
    v->elements = memory_resize_typed(v->elements, v->size, v->element_size);

    if (v->count > v->size)
    {
        // if elements are disposed by resizing update elements count
        v->count = v->size;
    }
}

void* generic_vector_append(vector *v)
{
    check_null_pointer(v);

    if(v->count == v->size)
    {
        // resize if no more space available
        generic_vector_resize(v, v->size + DEFAULT_VECTOR_SIZE);
    }

    // return last first empty vector slot
    void *out = v->elements + (v->count * v->element_size);
    v->count++;
    return out;
}

void* generic_vector_get(vector *v, size_t index)
{
    check_null_pointer(v);

    if(index >= v->count)
    {
        log_error("index out of bound");
    }

    return v->elements + (index * v->element_size);
}

void generic_vector_delete(vector *v, size_t index)
{
    check_null_pointer(v);
    
    if (index >= v->count)
    {
        log_error("index out of bound");
    }

    array_delete(index, v->elements, &v->count, v->element_size);
}

void generic_vector_free(vector *v)
{
    check_null_pointer(v);
    free(v->elements);
    free(v);
}
