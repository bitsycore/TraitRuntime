#ifndef LOG_H
#define LOG_H

typedef enum { DEBUG, INFO, WARN, _ERROR, NO_LEVEL } LogLevel;

void Log_send(LogLevel level, const char* format, ...);

#define LOG(format, ...) Log_send(INFO, format, __VA_ARGS__)
#define LOG_ERR(format, ...) Log_send(_ERROR, format, __VA_ARGS__)
#define LOG_WARN(format, ...) Log_send(WARNING, format, __VA_ARGS__)
#define LOG_DEBUG(format, ...) Log_send(DEBUG, format, __VA_ARGS__)
#define LINE_BREAK() Log_send(NO_LEVEL, "\n")

#endif //LOG_H
