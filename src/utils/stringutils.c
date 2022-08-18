#include <utils/stringutils.h>

#include <stdbool.h>
#include <string.h>
#include <types/memory.h>

bool is_string_equal(const char *a, const char *b)
{
    check_null_pointer((void *)a);
    check_null_pointer((void *)b);
    return strcmp(a, b) == 0;
}