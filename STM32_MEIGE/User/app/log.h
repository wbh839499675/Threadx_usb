
#ifndef __LOG_H__
#define __LOG_H__

#include "includes.h"


typedef enum
{
    LOG_DEBUG = 0,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
} LOG_LEVEL;

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"


#define log(level, fmt, ...)\
    LOG(level, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)

#define log_d(fmt, ...) \
    LOG(LOG_DEBUG, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)

#define log_i(fmt, ...) \
    LOG(LOG_INFO, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)

#define log_w(fmt, ...) \
    LOG(LOG_WARN, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)

#define log_e(fmt, ...) \
    LOG(LOG_ERROR, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)



void LOG(const int level, const char *fun, const int line, const char *fmt, ...);

#endif
