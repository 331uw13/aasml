#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "log.h"




static LogContext g_ctx;




void set_log_context(LogContext context) {
    g_ctx = context;
}


void print_location(size_t line_num) {
    size_t line_counter = 0;


    char* ch = g_ctx.file_data;
    char* end = g_ctx.file_data + g_ctx.file_size;
    
    printf("\033[1;90m");
    bool line_printed = false;
    while(ch < end) {
        if(line_counter == line_num) {
        
            printf("%c", *ch);

            line_printed = true;
        }
        if(*ch == '\n') {
            line_counter++;
            if(line_counter > line_num) {
                break;
            }
        }
        ch++;
    }

    if(!line_printed) {
        printf(" < Location not found >\n");
    }

    printf("\033[0m\n");
}

void error_printf(size_t line_num, const char* msg_fmt, ...) {
    va_list args;
    va_start(args);


    char buffer[512] = {0};


    fprintf(stderr, "\033[1;31m> Error in '%s' at line:%li\033[0m\n",
            g_ctx.file_name,
            line_num);

    print_location(line_num);
    fprintf(stderr, "  ");
    fflush(stderr);

    ssize_t buffer_len = vsnprintf(buffer, sizeof(buffer)-1, msg_fmt, args);
    write(STDERR_FILENO, buffer, buffer_len);
    write(STDERR_FILENO, "\n\n", 2);


    va_end(args);
    exit(EXIT_FAILURE);
}

