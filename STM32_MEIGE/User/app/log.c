
#include "log.h"


char *log_get_level(const int level)
{
    if (level == LOG_DEBUG)
    {
        return "DEBUG";
    }
    else if (level == LOG_INFO)
    {
        return "INFO";
    }
    else if (level == LOG_WARN)
    {
        return "WARN";
    }
    else if (level == LOG_ERROR)
    {
        return "ERROR";
    }
    return "UNKNOW";
}

void LOG(const int level, const char *fun, const int line, const char *fmt, ...)
{
    va_list arg;

    va_start(arg, fmt);
    char buf[128] = { 0 };
    vsnprintf(buf, sizeof(buf), fmt, arg);
    va_end(arg);
    
    if (level >= 0)
        printf("[%-5s] [%-20s%4d]    %s \r\n",
                log_get_level(level), fun, line, buf);
}
