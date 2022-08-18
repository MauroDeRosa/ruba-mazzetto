#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <stdlib.h>

typedef struct
{
    size_t element_size;
    size_t size;
    size_t count;
    void *elements;
} vector;

vector* generic_vector_init(size_t element_size);
void generic_vector_resize(vector *vector, size_t elements_count);
void* generic_vector_append(vector *vector);
void* generic_vector_get(vector *vector, size_t index);
void generic_vector_delete(vector *vector, size_t index);
void generic_vector_free(vector *vector);

#define VEC_INIT(TYPE) generic_vector_init(sizeof(TYPE))
#define VEC_RESIZE(VEC, ELEMENTS_COUNT) generic_vector_resize(VEC, ELEMENTS_COUNT)
#define VEC_APPEND(TYPE, VEC, VALUE) (*((TYPE*)generic_vector_append(VEC)) = VALUE)
#define VEC_GET(TYPE, VEC, INDEX) (*((TYPE*)generic_vector_get(VEC, INDEX)))
#define VEC_DELETE(VEC, INDEX) generic_vector_delete(VEC, INDEX)
#define VEC_FREE(VEC) generic_vector_free(VEC)


#endif /* __VECTOR_H__ */
