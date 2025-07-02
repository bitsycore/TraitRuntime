#ifndef LOG_H
#define LOG_H

typedef enum { LOGTYPE_DEBUG, LOGTYPE_INFO, LOGTYPE_WARN, LOGTYPE_ERROR, LOGTYPE_NO_LEVEL } LogLevel;

void Log_send(LogLevel level, const char* format, ...);

#define LOG(format, ...) Log_send(LOGTYPE_INFO, format, __VA_ARGS__)
#define LOG_DEBUG(format, ...) Log_send(LOGTYPE_DEBUG, format, __VA_ARGS__)
#define LOG_WARN(format, ...) Log_send(LOGTYPE_WARN, format, __VA_ARGS__)
#define LOG_ERR(format, ...) Log_send(LOGTYPE_ERROR, format, __VA_ARGS__)
#define LINE_BREAK() Log_send(LOGTYPE_NO_LEVEL, "\n")

#endif //LOG_H
