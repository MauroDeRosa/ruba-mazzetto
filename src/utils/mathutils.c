#include <utils/mathutils.h>

unsigned int clamp_uint(unsigned int value, unsigned int min, unsigned int max)
{
    value = value < min ? min : value;
    return value > max ? max : value;
}