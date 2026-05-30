#ifndef CODEGEN_CMD_H
#define CODEGEN_CMD_H

#include <stddef.h>
#include "types.h"
#include "token.h"


/*
typedef enum {
    IR_FUNC_IMPL,
    IR_NEW_VAR,
    IR_OP_MOV,
    IR_OP_ADD,
    IR_OP_SUB,
    IR_OP_MUL,
    IR_OP_DIV,
}
IRcmdKind;
*/

typedef struct {
    TokenKind kind;
    
    union {
        struct {
            char* name;
        }
        func_impl;

        struct {
            char* lhs_name;
            char* rhs_name;
        }
        op;

        struct {
            char* name;
            VarType type;
            char* init_val;
        }
        new_var;

        struct {
            char* val;
        }
        ret;
    }
    as;
}
IRcmd;


typedef struct {
    IRcmd* commands;
    size_t count;
    size_t allocated_count;
}
IRcmdArray;

extern void ircmd_print(IRcmd* ircmd);


#endif
