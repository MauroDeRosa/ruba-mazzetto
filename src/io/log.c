#include <io/log.h>

#include <io/ansi-color.h>

#include <stdarg.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char last_log[4096];

void __log(short type, int line, const char *file, const char *function, const char *fmt, ...)
{
    // copying error number to prevent it being changed by next instructions
    int error_number = errno;
    va_list args;
    // selecting output stream by the type of log (DEBUG, INFO, WARNING, ERROR)
    FILE *stream = (type == LOG_TYPE_INFO || type == LOG_TYPE_DEBUG) ? stdout : stderr;

    // creating timestamp string
    char timestamp[26] = {0};
    const time_t time_now = time(NULL);
    struct tm *ltm = localtime(&time_now);

    //if LOG_DATE preprocessor macro is defined log date and time, else log time only
#ifdef LOG_DATE
    strftime(timestamp, 25, "%Y-%m-%d %H:%M:%S", ltm);
#else
    strftime(timestamp, 25, "%H:%M:%S", ltm);
#endif // LOG_DATE
    va_start(args, fmt);


    switch (type)
    {
        // if log is of type INFO just print to stdout
    case LOG_TYPE_INFO:
        fprintf(stream, CR_CYN "[%s]" C_RESET, timestamp);
        vsprintf(last_log, fmt, args);
        fprintf(stream, last_log);
        fprintf(stream, "\n");
        break;

        // if log is of type WARNING just print to stderr with debug info
    case LOG_TYPE_WARNING:
        fprintf(stream, CR_CYN "[%s]" C_RESET, timestamp);
        fprintf(stream, CR_YEL "[%s:%d] " C_RESET, file, line);
        #ifdef LOG_DEBUG
        fprintf(stream, CR_YEL "%s() " C_RESET, function);
        #endif // LOG_DEBUG
        vsprintf(last_log, fmt, args);
        fprintf(stream, last_log);
        fprintf(stream, "\n");
        break;

        // if log is of type ERROR print to stderr with debug info, and print system generated error using strerror with the previously copied error_number
    case LOG_TYPE_ERROR:
        fprintf(stream, CR_CYN "[%s]" C_RESET, timestamp);
        fprintf(stream, CR_RED "[%s:%d] " C_RESET, file, line);
        fprintf(stream, CR_RED "%s() " C_RESET, function);
        vsprintf(last_log, fmt, args);
        fprintf(stream, last_log);
        fprintf(stream, "\n%s\n", error_number != 0 ? strerror(error_number) : "");
        // if LOG_EXIT_ON_ERROR preprocessor macro is defined exit(EXIT_FAILURE) is called
#ifdef LOG_EXIT_ON_ERROR
        va_end(args);
        fflush(stream);
        exit(EXIT_FAILURE);
#endif // LOG_EXIT_ON_ERROR
        break;

        // if LOG_DEBUG preprocessor macro is set then debug messages are logged to stdout
#ifdef LOG_DEBUG
    case LOG_TYPE_DEBUG: // debug
        fprintf(stream, CR_CYN "[%s]" C_RESET, timestamp);
        fprintf(stream, CR_CYN "[%s:%d] " C_RESET, file, line);
        fprintf(stream, CR_CYN "%s() " C_RESET, function);
        vsprintf(last_log, fmt, args);
        fprintf(stream, last_log);
        fprintf(stream, "\n");
        break;
#endif // DEBUG
    }
    va_end(args);
    fflush(stream);
}