
#ifndef __ERROR_H__
#define __ERROR_H__

/**
 * @file log.h
 * @author Mauro De Rosa (it.mauro.derosa@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-08-23
 * 
 * @copyright Copyright (c) 2022
 * @license licensed under MIT license
 * 
 */

#define LOG_TYPE_INFO 0 ///< log level: information
#define LOG_TYPE_WARNING 1 ///< log level: warning (stderr)
#define LOG_TYPE_ERROR 2 ///< log level: error (stderr)
#define LOG_TYPE_DEBUG 3 ///< log level: debug (works only if cmake log_debug option is active)

extern char last_log[4096]; ///< variable containing the last logged message (log level doesn't matter)

/**
 * @brief macro function logs information level formatted string
 * @param fmt format string
 * @param ... other variables used by the fmt string
 */
#define log_info(fmt, ...) __log(LOG_TYPE_INFO, __LINE__, __FILE__, __FUNCTION__, fmt __VA_OPT__(,) __VA_ARGS__)

/**
 * @brief macro function logs warning level formatted string
 * @param fmt format string
 * @param ... other variables used by the fmt string
 */
#define log_warning(fmt, ...) __log(LOG_TYPE_WARNING, __LINE__, __FILE__, __FUNCTION__, fmt __VA_OPT__(,) __VA_ARGS__)

/**
 * @brief macro function logs error level formatted string
 * @param fmt format string
 * @param ... other variables used by the fmt string
 */
#define log_error(fmt, ...) __log(LOG_TYPE_ERROR, __LINE__, __FILE__, __FUNCTION__, fmt __VA_OPT__(,) __VA_ARGS__)

/**
 * @brief macro function logs debug level formatted string
 * @param fmt format string
 * @param ... other variables used by the fmt string
 */
#define log_debug(fmt, ...) __log(LOG_TYPE_DEBUG, __LINE__, __FILE__, __FUNCTION__, fmt __VA_OPT__(,) __VA_ARGS__)

/**
 * @brief the actual log function (use macros instead)
 * 
 * @param type log level type (info, warn, error, debug)
 * @param line line where the log function is called
 * @param file file from which the log function is called
 * @param function function from which the log function is called
 * @param fmt format string
 * @param ... variable arguments for the format string
 */
void __log(short type, int line, const char* file, const char* function, const char *fmt, ...);

#endif /* __ERROR_H__ */
