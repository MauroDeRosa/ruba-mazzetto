#include <stdlib.h>
#include <time.h>
#include <utils/mathutils.h>
#include <utils/timeutils.h>
#include <string.h>

datetime datetime_now()
{
    time_t now = time(NULL);
    datetime *dt = localtime(&now);
    return *dt;
}

datetime datetime_time(unsigned int hours, unsigned int minutes, unsigned int seconds)
{
    datetime dt = datetime_now();
    dt.tm_hour = clamp_uint(hours, 0, 23);
    dt.tm_min = clamp_uint(minutes, 0, 59);
    dt.tm_sec = clamp_uint(seconds, 0, 59);
    return dt;
}

datetime datetime_datetime(unsigned int year, unsigned int month, unsigned int day, unsigned int hours, unsigned int minutes, unsigned int seconds)
{
    datetime dt = {0};
    dt.tm_year = year - 1900;
    dt.tm_mon = clamp_uint(month, 0, 11);
    dt.tm_mday = clamp_uint(day, 1, 31);
    dt.tm_hour = clamp_uint(hours, 0, 23);
    dt.tm_min = clamp_uint(minutes, 0, 59);
    dt.tm_sec = clamp_uint(seconds, 0, 59);
    return dt;
}

void datetime_string(datetime dt, char string[DATETIME_STR_SIZE])
{
    memset(string, 0, DATETIME_STR_SIZE);
    strftime(string, DATETIME_STR_SIZE-1, "%Y-%m-%d %H:%M:%S", &dt);
}

void datetime_date_string(datetime dt, char string[DATETIME_STR_SIZE])
{
    memset(string, 0, DATETIME_STR_SIZE);
    strftime(string, DATETIME_STR_SIZE-1, "%Y-%m-%d", &dt);
}

void datetime_time_string(datetime dt, char string[DATETIME_STR_SIZE])
{
    memset(string, 0, DATETIME_STR_SIZE);
    strftime(string, DATETIME_STR_SIZE-1, "%H:%M:%S", &dt);
}

int compare_datetime(const void *a, const void *b)
{
    const datetime *dta = a, *dtb = b;

    return difftime(mktime((struct tm *)dta), mktime((struct tm *)dtb));
}

int compare_datetime_desc(const void *a, const void *b)
{
    const datetime *dta = a, *dtb = b;

    return difftime(mktime((struct tm *)dtb), mktime((struct tm *)dta));
}