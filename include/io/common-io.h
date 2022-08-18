#ifndef __COMMON_IO_H__
#define __COMMON_IO_H__

#include <stddef.h>

size_t menu(const char* prompt, size_t choices_count, ...);
void any_key();

#endif /* __COMMON_IO_H__ */
