#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "codegen.h"



static struct {
    int output_fd;
    int indent_level;
}
cg = {
    .output_fd = STDOUT_FILENO,
};


void cg_set_output_fd(int fd) {
    cg.output_fd = fd;
}


void cg_printf(const char* fmt, ...) {
    va_list args;
    va_start(args);

    vdprintf(cg.output_fd, fmt, args);
    va_end(args);
}




