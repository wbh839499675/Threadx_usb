/*
********************************************************************************
*
*
*
*                          (c) Copyright 2018-2022; ...
*
*          All rights reserved.  Protected by international copyright laws.
*
*
*
*
********************************************************************************
*/

/*
********************************************************************************
*
*
* Filename      : log.h
* Version       : V1.0.1
* Programmer(s) : MEIGE
********************************************************************************
* Note(s)       : This header file is written for log system.
*
*
********************************************************************************
*/


/*
********************************************************************************
*                                 MODULE
*
* Note(s) : 
********************************************************************************
*/
#ifndef LOG_MODULE_PRESENT
#define LOG_MODULE_PRESENT

/*
********************************************************************************
*                                INCLUDES
********************************************************************************
*/
#include "includes.h"

/*
********************************************************************************
*                                DEFINES
********************************************************************************
*/

#define DBG_ENABLE                          (1)
#define DBG_COLOR                           (1)

#define DBG_VERISION                        (2)

/* DEBUG level */
typedef enum
{
    DBG_ERROR = 0,
    DBG_WARN,
    DBG_INFO,
    DBG_LOG,
} LOG_LEVEL;

#ifdef BDG_TAG
#ifndef DBG_SECTION_NAME
#define DBG_SECTION_NAME                    DBG_TAG
#endif
#else
/* compatible with old version */
#ifndef DBG_SECTION_NAME
#define DBG_SECTION_NAME    "DBG"
#endif
#endif /* DBG_TAG */

#ifdef DBG_ENABLE


#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#ifdef DBG_COLOR
#define	_DBG_COLOR(n)           printf("\033["#n"m")
#define _DBG_LOG_HDR(lvl_name, color_n)                     \
    printf("\033["#color_n"m["lvl_name"/"DBG_SECTION_NAME"] ")
#define _DBG_LOG_X_END                                      \
    printf("\033[0m\n")
#else
#define _DBG_COLOR(n)
#define _DBG_LOG_HDR(lvl_name, color_n)                     \
    printf("["lvl_name"/"DBG_SECTION_NAME"] ")
#define _DBG_LOG_X_END                                      \
    printf("\n")
#endif

////////////////////////////////////////////////////////////////////////////////
#if 0
inline void dbg_log_func(const int level, const char *fmt, ...)
{
    va_list arg;
    va_start(arg, fmt);
    char buf[128] = { 0 };
    vsnprintf(buf, sizeof(buf), fmt, arg);
    va_end(arg);

    do
    {
        _DBG_LOG_HDR(lvl, color_n);
        printf("[%-20s%4d]    %s",
                 __FUNCTION__, __LINE__, buf);
        _DBG_LOG_X_END;
    }
    while (0);
}
#endif
////////////////////////////////////////////////////////////////////////////////
/*
 * static debug routine
 * NOTE: This is a NOT RECOMMENDED API. Please using LOG_X API.
 *       It will be DISCARDED later. Because it will take up more resources.
 */
#define dbg_log(level, fmt, ...)                            \
    if ((level) <= DBG_LEVEL)                               \
    {                                                       \
        switch(level)                                       \
        {                                                   \
            case DBG_ERROR:   _DBG_LOG_HDR("E", 31); break; \
            case DBG_WARNING: _DBG_LOG_HDR("W", 33); break; \
            case DBG_INFO:    _DBG_LOG_HDR("I", 32); break; \
            case DBG_LOG:     _DBG_LOG_HDR("D", 0); break;  \
            default: break;                                 \
        }                                                   \
        printf(fmt, ##__VA_ARGS__);                         \
        _DBG_COLOR(0);                                      \
    }

#define dbg_here                                            \
    if ((DBG_LEVEL) <= DBG_LOG){                            \
        printf(DBG_SECTION_NAME " Here %s:%d\n",            \
            __FUNCTION__, __LINE__);                        \
    }

#define dbg_log_line(lvl, color_n, fmt, ...)                \
    do                                                      \
    {                                                       \
        _DBG_LOG_HDR(lvl, color_n);                         \
        printf(fmt, ##__VA_ARGS__);                         \
        _DBG_LOG_X_END;                                     \
    }                                                       \
    while (0)


#define dbg_log_func(lvl, color_n, fmt, ...)                \
    do                                                      \
    {                                                       \
        _DBG_LOG_HDR(lvl, color_n);                         \
        printf("[%-20s%4d]\t", __FUNCTION__, __LINE__);       \
        printf(fmt, ##__VA_ARGS__);                         \
        _DBG_LOG_X_END;                                     \
    }                                                       \
    while (0)

#define dbg_raw(...)         printf(__VA_ARGS__);

#else
#define dbg_log(level, fmt, ...)
#define dbg_here
#define dbg_enter
#define dbg_exit
#define dbg_log_line(lvl, color_n, fmt, ...)
#define dbg_raw(...)
#endif /* DBG_ENABLE */

#if (DBG_VERISION == 1)
#if (DBG_LEVEL >= DBG_LOG)
#define log_d(fmt, ...)      dbg_log_line("D", 0, fmt, ##__VA_ARGS__)
#else
#define log_d(...)
#endif

#if (DBG_LEVEL >= DBG_INFO)
#define log_i(fmt, ...)      dbg_log_line("I", 32, fmt, ##__VA_ARGS__)
#else
#define log_i(...)
#endif

#if (DBG_LEVEL >= DBG_WARNING)
#define LOG_W(fmt, ...)      dbg_log_line("W", 33, fmt, ##__VA_ARGS__)
#else
#define LOG_W(...)
#endif

#if (DBG_LEVEL >= DBG_ERROR)
#define log_e(fmt, ...)      dbg_log_line("E", 31, fmt, ##__VA_ARGS__)
#else
#define log_e(...)
#endif

#define LOG_RAW(...)         dbg_raw(__VA_ARGS__)

#elif (DBG_VERISION == 2)
#if (DBG_LEVEL >= DBG_LOG)
#define log_d(fmt, ...)      dbg_log_func("D", 0, fmt, ##__VA_ARGS__)
#else
#define log_d(...)
#endif

#if (DBG_LEVEL >= DBG_INFO)
#define log_i(fmt, ...)      dbg_log_func("I", 32, fmt, ##__VA_ARGS__)
#else
#define log_i(...)
#endif

#if (DBG_LEVEL >= DBG_WARNING)
#define LOG_W(fmt, ...)      dbg_log_func("W", 33, fmt, ##__VA_ARGS__)
#else
#define LOG_W(...)
#endif

#if (DBG_LEVEL >= DBG_ERROR)
#define log_e(fmt, ...)      dbg_log_func("E", 31, fmt, ##__VA_ARGS__)
#else
#define log_e(...)
#endif

#define LOG_RAW(...)         dbg_raw(__VA_ARGS__)

#endif

/*

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

*/

void LOG(const int level, const char *fun, const int line, const char *fmt, ...);

#endif
