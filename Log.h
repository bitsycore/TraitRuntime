#ifndef LOG_H
#define LOG_H

typedef enum { DEBUG, INFO, WARN, ERROR, NO_LEVEL } LogLevel;

void send_log(LogLevel level, const char* format, ...);

#define LOG(format, ...) send_log(INFO, format, __VA_ARGS__)

#define LOG_ERR(format, ...) send_log(ERROR, format, __VA_ARGS__)
#define LOG_WARN(format, ...) send_log(WARNING, format, __VA_ARGS__)
#define LOG_DEBUG(format, ...) send_log(DEBUG, format, __VA_ARGS__)

#define LINE_BREAK() send_log(NO_LEVEL, "\n")

#endif //LOG_H
