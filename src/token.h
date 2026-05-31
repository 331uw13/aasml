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


typedef enum {
    OPERAND_VARIABLE,
    OPERAND_LITERAL_INT,
    OPERAND_LITERAL_STRING
}
OperandKind;


typedef enum {
    P_OPERATION,
    P_FUNCTION_IMPL
}
ParsedTokenKind;

typedef struct {
    char*       text; // <- Content differs from what the operand is.
    VarType     type;
    OperandKind kind;
}
Operand;

typedef struct {
    Operand lhs;
    Operand rhs;
}
Operation;

typedef struct {
    char*     text;
    TokenKind kind;

    bool is_parsed;
    struct {
        ParsedTokenKind p_kind;
        union {
            Operation operation;
        
            struct {
                char* name;
                VarType ret_type;
            }
            function_impl;
        }
        as;
    }
    parsed;
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
