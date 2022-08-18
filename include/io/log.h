#ifndef __ERROR_H__
#define __ERROR_H__

#define LOG_TYPE_INFO 0
#define LOG_TYPE_WARNING 1
#define LOG_TYPE_ERROR 2
#define LOG_TYPE_DEBUG 3

extern char last_log[2048];

#define log_info(fmt, ...) __log(LOG_TYPE_INFO, __LINE__, __FILE__, __FUNCTION__, fmt __VA_OPT__(,) __VA_ARGS__)
#define log_warning(fmt, ...) __log(LOG_TYPE_WARNING, __LINE__, __FILE__, __FUNCTION__, fmt __VA_OPT__(,) __VA_ARGS__)
#define log_error(fmt, ...) __log(LOG_TYPE_ERROR, __LINE__, __FILE__, __FUNCTION__, fmt __VA_OPT__(,) __VA_ARGS__)
#define log_debug(fmt, ...) __log(LOG_TYPE_DEBUG, __LINE__, __FILE__, __FUNCTION__, fmt __VA_OPT__(,) __VA_ARGS__)

void __log(short type, int line, const char* file, const char* function, const char *fmt, ...);

#endif /* __ERROR_H__ */
