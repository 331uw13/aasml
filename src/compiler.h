#ifndef COMPILER_H
#define COMPILER_H


#include <stddef.h>
#include "log.h"
#include "token.h"
#include "ir_codegen.h"
#include "codegen/codegen.h"



extern TokenArray* tokenize_data(char* data, size_t size);
extern void        parse_tokens(TokenArray* token_array);
//extern void        check_syntax(TokenArray* token_array);
//extern IRcmdArray* intermediate_codegen(TokenArray* token_array);



#endif
