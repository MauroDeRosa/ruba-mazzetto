#include <utils/mathutils.h>

unsigned int clamp_uint(unsigned int value, unsigned int min, unsigned int max)
{
    value = value < min ? min : value;
    return value > max ? max : value;
}

/**
 * @brief djb2 @todo document hashing function
 * 
 * @param str 
 * @return unsigned long 
 */
unsigned long hash(unsigned char *str)
{
    unsigned long hash = 5381;
    int c;
    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash;
}