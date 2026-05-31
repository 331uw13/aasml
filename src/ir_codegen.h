#ifndef CODEGEN_CMD_H
#define CODEGEN_CMD_H

#include <stddef.h>
#include "types.h"
#include "token.h"


typedef struct {
    char* text;
    OperandKind kind;
}
IROperand;

typedef struct {
    TokenKind kind;
   

    union {

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
