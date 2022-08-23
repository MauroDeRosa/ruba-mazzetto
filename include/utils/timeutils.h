#ifndef __TIMEUTILS_H__
#define __TIMEUTILS_H__

/**
 * @file timeutils.h
 * @author Mauro De Rosa (it.mauro.derosa@gmail.com)
 * @brief utility functions for timestamp management and formatting
 * @version 1.0.0 alpha
 * @date 2022-08-23
 * 
 * @copyright Copyright (c) 2022
 * @license licensed under MIT license
 * 
 */

#include <stdlib.h>
#include <time.h>

#define DATETIME_STR_SIZE 25 ///< suggested size for a string containing a formatted datetime
typedef struct tm datetime; ///< datetime data type @see struct tm

/**
 * @brief current date and time
 * 
 * @return datetime 
 */
datetime datetime_now();

/**
 * @brief custom time datetime
 * 
 * @param hours 0 to 23
 * @param minutes 0 to 59
 * @param seconds 0 to 59
 * @return datetime 
 */
datetime datetime_time(unsigned int hours, unsigned int minutes, unsigned int seconds);

/**
 * @brief custom date and time
 * 
 * @param year 
 * @param month 0 to 11 
 * @param day 1 to 31
 * @param hours 0 to 23 
 * @param minutes 0 to 59
 * @param seconds 0 to 59
 * @return datetime 
 */
datetime datetime_new(unsigned int year, unsigned int month, unsigned int day, unsigned int hours, unsigned int minutes, unsigned int seconds);

/**
 * @brief formatted date and time string
 * 
 * @param dt @see datetime
 * @param string the formatted string
 */
void datetime_string(datetime dt, char string[DATETIME_STR_SIZE]);

/**
 * @brief formatted date string
 * 
 * @param dt @see datetime
 * @param string the formatted string
 */
void datetime_date_string(datetime dt, char string[DATETIME_STR_SIZE]);

/**
 * @brief formatted time string
 * 
 * @param dt @see datetime
 * @param string the formatted string
 */
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
