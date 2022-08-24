/**
 * @file mathutils.c
 * @author Mauro De Rosa (it.mauro.derosa@gmail.com)
 * @brief math utilities module implementation @see mathutils.h
 * @version 1.0.0 alpha
 * @date 2022-08-24
 * 
 * @copyright Copyright (c) 2022
 * @license licensed under MIT license
 * 
 */

#include <utils/mathutils.h>

unsigned int clamp_uint(unsigned int value, unsigned int min, unsigned int max)
{
    value = value < min ? min : value;
    return value > max ? max : value;
}

unsigned long hash(unsigned char *str)
{
    unsigned long hash = 5381;
    int c;
    while ((c = *str++) != '\0')
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    return hash;
}