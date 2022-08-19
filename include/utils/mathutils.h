#ifndef __MATHUTILS_H__
#define __MATHUTILS_H__

#define MIN(a, b) ((a) < (b)) ? (a) : (b)
#define MAX(a, b) ((a) > (b)) ? (a) : (b)

unsigned int clamp_uint(unsigned int value, unsigned int min, unsigned int max);
unsigned long hash(unsigned char *str);

#endif /* __MATHUTILS_H__ */
