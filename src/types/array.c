#include <types/array.h>
#include <types/memory.h>
#include <io/log.h>
#include <string.h>
#include <stdlib.h>

void *binary_search(void *value, void *array, size_t array_size, size_t data_size, __compar_fn_t compare_function)
{
    check_null_pointer(value);
    check_null_pointer(array);
    check_null_pointer(compare_function);

    if (array_size == 0)
    {
        return NULL;
    }

    qsort(array, array_size, data_size, compare_function);
    size_t low = 0, high = array_size - 1, mid = 0;
    int compare;

    while (low <= high)
    {
        mid = (low + high) / 2;
        compare = compare_function(value, array + (mid * data_size));
        if (compare == 0)
        {
            return array + (mid * data_size);
        }
        else if (compare > 0)
        {
            if (low == high)
                break;
            low = mid + 1;
        }
        else
        {
            if (low == high || mid == 0)
                break;
            high = mid - 1;
        }
    }
    return NULL;
}

void array_delete(size_t index, void *array, size_t *array_size, size_t data_size)
{
    check_null_pointer(array);
    check_null_pointer(array_size);

    if (index >= *array_size)
    {
        log_error("index out of bound");
    }

    memcpy(array + index * data_size, array + (index + 1) * data_size, (*array_size - index - 1) * data_size);
    (*array_size)--;
}

size_t array_index_from_pointer(void *array, void *element, size_t data_size)
{
    check_null_pointer(array);
    check_null_pointer(element);
    return (element - array) / data_size;
}

void array_shuffle(void *array, size_t array_size, size_t data_size)
{
    check_null_pointer(array);
    size_t rand1, rand2;

    for (size_t i = 0; i < array_size; i++)
    {
        rand1 = rand() % array_size;
        rand2 = rand() % array_size;
        memory_swap(array + (rand1 * data_size), array + (rand2 * data_size), data_size);
    }
}

void array_reverse(void *array, size_t array_size, size_t data_size)
{
    check_null_pointer(array);

    for (size_t i = 0; i < array_size / 2; i++)
    {
        memory_swap(array + i * data_size, array + (array_size - i - 1) * data_size, data_size);
    }
}

void *array_copy(void *array, size_t array_size, size_t data_size)
{
    check_null_pointer(array);

    void *new_array = memory_allocate_typed(array_size, data_size);
    memcpy(new_array, array, array_size * data_size);
    return new_array;
}

void* array_filter(void *array, size_t array_size, size_t data_size, filter_callback filter, void *filter_value, size_t *filtered_size)
{
    check_null_pointer(array);
    check_null_pointer(filtered_size);
    check_null_pointer(filter_value);

    void *filtered = memory_allocate_typed(array_size, data_size);
    size_t new_size = 0;

    for (size_t i = 0; i < array_size; i++)
    {
        if (filter(array + (i * data_size), filter_value))
        {
            memcpy(filtered+(new_size*data_size), array+(i*data_size), data_size);
            new_size++;
        }
    }

    *filtered_size = new_size;
    return memory_resize_typed(filtered, new_size, data_size);
}