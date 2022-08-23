#ifndef __MATHUTILS_H__
#define __MATHUTILS_H__

/**
 * @file mathutils.h
 * @author Mauro De Rosa (it.mauro.derosa@gmail.com)
 * @brief math utilities
 * @version 1.0.0 alpha
 * @date 2022-08-23
 * 
 * @copyright Copyright (c) 2022
 * @license licensed under MIT license
 * 
 */

#define MIN(a, b) ((a) < (b)) ? (a) : (b) ///< maximum of two values
#define MAX(a, b) ((a) > (b)) ? (a) : (b) ///< minimum of two values

/**
 * @brief clamp an unsigned int between a minumum and a maximum
 * 
 * @param value current value
 * @param min minimum accepted value
 * @param max maximum accepted value
 * @return unsigned int the clamped value
 */
unsigned int clamp_uint(unsigned int value, unsigned int min, unsigned int max);

/**
 * @brief generate an hash value from a given string (djb2 hash algorithm)
 * 
 * @param str the string from which the hash value will be calculated
 * @return unsigned long the calculated hash value
 */
unsigned long hash(unsigned char *str);

#endif /* __MATHUTILS_H__ */
