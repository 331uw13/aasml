#ifndef TOKEN_H
#define TOKEN_H

#include <stddef.h>

#include "types.h"


typedef enum {
    TOK_IDENTIFIER,
    TOK_LITERAL,
    TOK_RET,
    TOK_VAR,
    TOK_MOV,
    TOK_ADD,
    TOK_SUB,
    TOK_MUL,
    TOK_DIV,
    TOK_FUNC,     // .func
    TOK_O_PAREN,  // (
    TOK_C_PAREN,  // )
    TOK_OC_PAREN, // {
    TOK_CC_PAREN, // }
    TOK_SEMICOLON, // ;
    TOK_COLON, // :
    TOK_COMMA, // ,
    TOK_L_ARROW, // <-
    TOK_BYTE,
    TOK_INT16,
    TOK_INT32,
    TOK_INT64
}
TokenKind;


typedef struct {
    TokenKind kind;
    char*     text;
}
Token;


typedef struct {
    Token* tokens;
    size_t count;
    size_t allocated_count;
}
TokenArray;

extern const char* token_kind_to_str(TokenKind kind);
extern VarType token_kind_to_vartype(TokenKind kind);


#endif
