#ifndef TOKEN_H
#define TOKEN_H



typedef enum {
    TOK_IDENTIFIER,
    TOK_LITERAL,
    TOK_VAR,      // var
    TOK_MOV,      // mov
    TOK_FUNC,     // .func
    TOK_O_PAREN,  // (
    TOK_C_PAREN,  // )
    TOK_OC_PAREN, // {
    TOK_CC_PAREN, // }
    TOK_SEMICOLON, // ;
    TOK_COLON, // :
    TOK_COMMA, // ,
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



#endif
