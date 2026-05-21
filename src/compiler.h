#ifndef COMPILER_H
#define COMPILER_H


#include <stddef.h>
#include "token.h"
#include "log.h"



extern const char* token_kind_to_str(TokenKind kind);
extern TokenArray* tokenize_data(char* data, size_t size);



extern void check_syntax(TokenArray* token_array);





#endif
