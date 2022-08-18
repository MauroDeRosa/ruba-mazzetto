#ifndef __ARRAY_H__
#define __ARRAY_H__

/**
 * @file array.h
 * @author Mauro De Rosa (it.mauro.derosa@gmail.com)
 * @brief generic array helper functions
 * @version 0.1
 * @date 2022-07-27
 * 
 * @copyright Copyright (c) 2022
 * @license licensed under MIT license
 * 
 */

#include <stdlib.h>
#include <stdbool.h>

typedef bool (*filter_callback)(void* element, void* value);

/**
 * @brief generic array binary search function
 * 
 * @param value pointer to the variable (primitive/struct/etc.) containing the searched value 
 * @param array array base pointer
 * @param array_size size of the array
 * @param data_size byte size of array elements and value parameter
 * @param compare_function function used for sorting the array and searching the requested value
 * @return void* pointer to the found element in the array
 * @return NULL element with the searched value not found
 */
void* binary_search(void* value, void* array, size_t array_size, size_t data_size, __compar_fn_t compare_function);

/**
 * @brief delete an element from the array, move everything after it removing the gap and decrease vector size by 1
 * 
 * @param index index of the element to delete
 * @param array base pointer of the array
 * @param array_size pointer to the variable containing the array size
 * @param data_size byte size of array elements
 */
void array_delete(size_t index, void* array, size_t *array_size, size_t data_size);

/**
 * @brief get index of the array from the pointer of the element
 * 
 * @param array base pointer of the array
 * @param element pointer to an array element
 * @param data_size byte size of array elements
 * @return size_t 
 */
size_t array_index_from_ptr(void* array, void* element, size_t data_size);

/**
 * @brief shuffle elements of the array (destructive) use srand() somewhere in the program to set the seed 
 * 
 * @param array base pointer of the array
 * @param array_size size of the array
 * @param data_size byte size of array elements
 */
void array_shuffle(void* array, size_t array_size, size_t data_size);

/**
 * @brief reverse elements in the array (destructive)
 * 
 * @param array base pointer of the array
 * @param array_size size of the array
 * @param data_size byte size of array elements
 */
void array_reverse(void* array, size_t array_size, size_t data_size);

/**
 * @brief copy an array into a new allocated one
 *
 * @param array base pointer of the array
 * @param array_size size of the array
 * @param data_size byte size of array elements
 * @return void* a copy of the passed array (must be freed)
 */
void *array_copy(void *array, size_t array_size, size_t data_size);

/**
 * @brief return an allocated filtered array using a filter_callback function;
 * 
 * @param array array pointer
 * @param array_size the array length
 * @param data_size size of each array element
 * @param filter filter function callback
 * @param filtered_size pointer to a size_t variable which will contain the length of the filtered array
 * @param filter_value the value that will be used by the filter callback
 * @return void* the filtered (must be freed)
 */
void* array_filter(void *array, size_t array_size, size_t data_size, filter_callback filter, void* filter_value, size_t *filtered_size);

#endif /* __ARRAY_H__ */
