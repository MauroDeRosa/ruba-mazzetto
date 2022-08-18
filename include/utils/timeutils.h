#ifndef __TIMEUTILS_H__
#define __TIMEUTILS_H__

#include <stdlib.h>
#include <time.h>

#define DATETIME_STR_SIZE 25
typedef struct tm datetime;

datetime datetime_now();
datetime datetime_time(unsigned int hours, unsigned int minutes, unsigned int seconds);
datetime datetime_datetime(unsigned int year, unsigned int month, unsigned int day, unsigned int hours, unsigned int minutes, unsigned int seconds);
void datetime_string(datetime dt, char string[DATETIME_STR_SIZE]);
void datetime_date_string(datetime dt, char string[DATETIME_STR_SIZE]);
void datetime_time_string(datetime dt, char string[DATETIME_STR_SIZE]);

/**
 * @brief compare two datetime (ascending order)
 * 
 * @param a first datetime
 * @param b second datetime
 * @return int time difference between the two
 */
int compare_datetime_asc(const void *a, const void *b);

/**
 * @brief compare two datetime (descending order)
 * 
 * @param a first datetime
 * @param b second datetime
 * @return int time difference between the two
 */
int compare_datetime_desc(const void *a, const void *b);

#endif /* __TIMEUTILS_H__ */
