
#ifndef __MEMORY_H__
#define __MEMORY_H__

/**
 * @file memory.h
 * @author Mauro De Rosa (it.mauro.derosa@gmail.com)
 * @brief memory management functions
 * @version 1.0.0 alpha
 * @date 2022-08-23
 * 
 * @copyright Copyright (c) 2022
 * @license licensed under MIT license
 * 
 */

#include <stdlib.h>

#define check_null_pointer(pointer) __check_null_pointer(pointer, __FILE__, __FUNCTION__, __LINE__) ///< @see __check_null_pointer
#define memory_allocate(heap_size) __memory_allocate(heap_size, __FILE__, __FUNCTION__, __LINE__) ///< @see __memory_allocate
#define memory_allocate_typed(n, data_size) __memory_allocate_typed(n, data_size, __FILE__, __FUNCTION__, __LINE__) ///< @see __memory_allocate_typed
#define memory_resize(pointer, heap_size) __memory_resize(pointer, heap_size, __FILE__, __FUNCTION__, __LINE__) ///< @see __memory_resize
#define memory_resize_typed(pointer, n, data_size) __memory_resize_typed(pointer, n, data_size, __FILE__, __FUNCTION__, __LINE__) ///< @see __memory_resize_typed
#define memory_allocate_zero(n, data_size) __memory_allocate_zero(n, data_size, __FILE__, __FUNCTION__, __LINE__) ///< @see __memory_allocate_zero

/**
 * @brief swap memory sections
 * 
 * @param a first section
 * @param b second section
 * @param data_size section size
 */
void memory_swap(void *a, void *b, size_t data_size);

/**
 * @brief allocate heap memory and check for allocation errors
 * 
 * @param heap_size how many bites to allocate
 * @return void* pointer to the allocated memory
 */
void* __memory_allocate(size_t heap_size, const char *file, const char* function, size_t line);

/**
 * @brief allocate heap memory and check for allocation errors
 * 
 * @param n number of sections to allocate
 * @param data_size size of sections
 * @return void* pointer to the allocated memory
 */
void* __memory_allocate_typed(size_t n, size_t data_size, const char *file, const char* function, size_t line);

/**
 * @brief resize allocated memory and check for reallocation errors
 * 
 * @param base_pointer pointer to the memory area to resize
 * @param heap_size how many bytes will the memory area become
 * @return void* pointer to the resized memory
 */
void* __memory_resize(void* base_pointer, size_t heap_size, const char *file, const char* function, size_t line);

/**
 * @brief resize allocated memory and check for reallocation errors
 * 
 * @param base_pointer pointer to the memory area to resize
 * @param n number of sections of the resized area
 * @param data_size size of sections
 * @return void* pointer to the resized memory
 */
void* __memory_resize_typed(void* base_pointer, size_t n, size_t data_size, const char *file, const char* function, size_t line);


/**
 * @brief allocate heap memory setted to zero and check for allocation errors
 * 
 * @param n number of sections of the resized area
 * @param data_size size of sections
 * @return void* pointer to the allocated memory
 */
void* __memory_allocate_zero(size_t n, size_t data_size, const char *file, const char* function, size_t line);

/**
 * @brief check for null pointer and throws error if necessary
 * 
 * @param pointer the pointer to check
 */
void __check_null_pointer(void* pointer, const char *file, const char* function, size_t line);

#endif /* __MEMORY_H__ */
