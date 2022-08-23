#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <stdlib.h>

/**
 * @brief vector data structure
 */
typedef struct
{
    size_t element_size; ///< size of each element in bytes
    size_t size; ///< vector size (how many elements you can append without resizing, if count is 0)
    size_t count;  ///< vector elements count (how many elements are effectively in the array)
    void *elements; ///< elements array
} vector;

/**
 * @brief initialize a vector
 * 
 * @param element_size size of each element i.e. sizeof(int)
 * @return vector* pointer to the initialized vector (must be freed)
 */
vector* generic_vector_init(size_t element_size);

/**
 * @brief resizes a vector
 * 
 * @param vector the vector to resize
 * @param elements_count how many elements can the vector contain
 */
void generic_vector_resize(vector *vector, size_t elements_count);

/**
 * @brief gives a pointer to the first empty spot of the given vector (automatically resizes it if more space needed)
 * 
 * @param vector the vector on which append an element
 * @return void* pointer to the first empty spot of the vector
 */
void* generic_vector_append(vector *vector);

/**
 * @brief gives a pointer to the element at the given index of a vector
 * 
 * @param vector the vector from which you wanna get the element
 * @param index the index of the element
 * @return void* the pointer to the element
 */
void* generic_vector_get(vector *vector, size_t index);

/**
 * @brief delete an element at the given index from a vector
 * 
 * @param vector the vector on which delete the element
 * @param index index of the element to delete
 */
void generic_vector_delete(vector *vector, size_t index);

/**
 * @brief frees the vector allocated memory from heap
 * 
 * @param vector vector pointer
 */
void generic_vector_free(vector *vector);

#define VEC_INIT(TYPE) generic_vector_init(sizeof(TYPE)) ///< initialize a vector of the given type @see vector
#define VEC_RESIZE(VEC, ELEMENTS_COUNT) generic_vector_resize(VEC, ELEMENTS_COUNT) ///< resize a vector to contain ELEMENTS_COUNT number of elements @see vector
#define VEC_APPEND(TYPE, VEC, VALUE) (*((TYPE*)generic_vector_append(VEC)) = VALUE) ///< append an element of the given value to the given vector (type must match vector type)
#define VEC_GET(TYPE, VEC, INDEX) (*((TYPE*)generic_vector_get(VEC, INDEX))) ///< get an element at the given index of the given vector (type must match vector type)
#define VEC_DELETE(VEC, INDEX) generic_vector_delete(VEC, INDEX) ///< delete an element at the given index from the given vector
#define VEC_FREE(VEC) generic_vector_free(VEC) ///< free vector allocated memory from heap


#endif /* __VECTOR_H__ */
