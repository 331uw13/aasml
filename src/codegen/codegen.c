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

void cg_printf(const char* fmt, ...) {
    va_list args;
    va_start(args);


    char buf[1024 * 4] = { 0 };
    ssize_t len = vsnprintf(buf, sizeof(buf)-1, fmt, args);

    write(STDOUT_FILENO, buf, len);
    
    if(cg.alt_output_fd >= 0) {
        write(cg.alt_output_fd, buf, len);
    }
   
    va_end(args);
}




