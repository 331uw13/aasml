#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "codegen.h"



static struct {
    int alt_output_fd;
    int indent_level;
}
cg = {
    .alt_output_fd = -1,
};


void cg_set_altoutput_fd(int fd) {
    cg.alt_output_fd = fd;
}


static
void p_write_fds(void* buf, size_t len) {
    write(STDOUT_FILENO, buf, len);
    if(cg.alt_output_fd >= 0) {
        write(cg.alt_output_fd, buf, len);
    }
}


#define BUFFER_MAX (1024 * 4)

void cg_printf(const char* fmt, ...) {
    va_list args;
    va_start(args);


    char buf[BUFFER_MAX] = { 0 };
    ssize_t len = vsnprintf(buf, sizeof(buf)-1, fmt, args);

    p_write_fds(buf, len);
    va_end(args);
}


void cg_printf_comment(const char* fmt, ...) {
    va_list args;
    va_start(args);


    char buf[BUFFER_MAX] = { 0 };
    ssize_t len = vsnprintf(buf, sizeof(buf)-1, fmt, args);

    p_write_fds("# ", 2);
    p_write_fds(buf, len);
    va_end(args);
}



