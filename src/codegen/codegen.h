#ifndef CODEGEN_H
#define CODEGEN_H

#include "../token.h"
#include "stack_frame.h"



//extern void cg_set_output_fd(int fd);


extern void cg_set_altoutput_fd(int fd);
extern void cg_printf(const char* fmt, ...);
extern void cg_printf_comment(const char* fmt, ...);


// The input is parsed tokens.
extern void codegen_gnu_as_x86_64(TokenArray* token_array);




#endif
