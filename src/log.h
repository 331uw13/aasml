#ifndef LOG_H
#define LOG_H

#include <stddef.h>


typedef struct {
    char* file_name;
    char* file_data;
    size_t file_size;
}
LogContext;

extern void set_log_context(LogContext context);
extern void error_printf(size_t line_num, const char* msg_fmt, ...);




#endif
