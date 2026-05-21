#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "compiler.h"
#include "string_view.h"


typedef struct {
    const TokenKind kind;
    const char* text;
}
TokenMapEntry;

static const TokenMapEntry TOKEN_MAP[] = {
    { .kind = TOK_VAR, .text = "var" },
    { .kind = TOK_MOV, .text = "mov" },
    { .kind = TOK_ADD, .text = "add" },
    { .kind = TOK_SUB, .text = "sub" },
    { .kind = TOK_MUL, .text = "mul" },
    { .kind = TOK_DIV, .text = "div" },
    { .kind = TOK_FUNC, .text = ".func" },
    { .kind = TOK_O_PAREN, .text = "(" },
    { .kind = TOK_C_PAREN, .text = ")" },
    { .kind = TOK_OC_PAREN, .text = "{" },
    { .kind = TOK_CC_PAREN, .text = "}" },
    { .kind = TOK_SEMICOLON, .text = ";" },
    { .kind = TOK_COLON, .text = ":" },
    { .kind = TOK_COMMA, .text = "," },
    { .kind = TOK_L_ARROW, .text = "<-" },
    { .kind = TOK_BYTE, .text = "byte" },
    { .kind = TOK_INT16, .text = "int16" },
    { .kind = TOK_INT32, .text = "int32" },
    { .kind = TOK_INT64, .text = "int64" },
};

#define CONCATABLE_TOKENS " (){};:,"

#define TOK_COMMENT_CHAR '|'
#define ARRAY_LENGTH(arr) (sizeof(arr) / sizeof *arr)


VarType token_kind_to_vartype(TokenKind kind) {
    switch(kind) {
        case TOK_BYTE: return TYPE_BYTE;
        case TOK_INT16: return TYPE_INT16;
        case TOK_INT32: return TYPE_INT32;
        case TOK_INT64: return TYPE_INT64;
    }

    return TYPE_BYTE;
}

const char* token_kind_to_str(TokenKind kind) {
    switch(kind) {
        case TOK_IDENTIFIER: return "IDENTIFIER";
        case TOK_LITERAL : return "LITERAL";
        case TOK_VAR : return "VAR";
        case TOK_MOV : return "MOV";
        case TOK_ADD : return "ADD";
        case TOK_SUB : return "SUB";
        case TOK_MUL : return "MUL";
        case TOK_DIV : return "DIV";
        case TOK_FUNC : return "FUNC";
        case TOK_O_PAREN : return "O_PAREN";
        case TOK_C_PAREN : return "C_PAREN";
        case TOK_OC_PAREN : return "OC_PAREN";
        case TOK_CC_PAREN : return "CC_PAREN";
        case TOK_SEMICOLON : return "SEMI_COLON";
        case TOK_COLON : return "COLON";
        case TOK_COMMA : return "COMMA";
        case TOK_L_ARROW : return "LEFT_ARROW";
        case TOK_BYTE : return "BYTE";
        case TOK_INT16 : return "INT16";
        case TOK_INT32 : return "INT32";
        case TOK_INT64 : return "INT64";
        
        default: return "\033[31mUNKNOWN\033[0m";
                         //case TOK_ : return "";
    }
}


// 'group_mark' can be used to group words instead of separating them.
// for example if group_mark == '"' and input is: "hello world"
// the output word is: |hello world| instead of: |hello|world|
char* get_word(StringView* sv, char* iter, char* end, const char* separators, char group_mark) {
    if(iter >= end) {
        return NULL;
    }

    sv->len = 0;
    sv->ptr = iter;

    int num_group_marks = 0;

    size_t num_separators = strlen(separators);


    while(iter < end) {

        if(*iter == group_mark) {
            num_group_marks++;
        }

        bool iter_is_separator = false;
        for(size_t i = 0; i < num_separators; i++) {
            if(*iter == separators[i]) {
                iter_is_separator = true;
                break;
            }
        }

        bool iter_is_ascii = *iter >= 0x20 && *iter <= 0x7E;
        
        if(iter_is_ascii) {
            sv->len++;
        }
        iter++;

        if((iter_is_separator && num_group_marks == 0)
        || (num_group_marks >= 2)) {
            //if(!iter_is_separator) { sv->len++; }
            //sv->len++;
            if(sv->len > 1 && num_group_marks == 0 && iter_is_ascii) {
                iter--;
                sv->len--;
            }
            break;
        }
        
    }


    return iter;
}


void print_sv(StringView* sv) {
    for(size_t i = 0; i < sv->len; i++) {     
        printf("%c", sv->ptr[i]);
    }
}




void add_token(TokenArray* token_array, StringView sv) {
    Token token = {0};

    sv = sv_trim(sv);
    if(sv.len == 0) {
        return;
    }

    bool found_token_kind = false;

    for(size_t i = 0; i < ARRAY_LENGTH(TOKEN_MAP); i++) {
        const TokenMapEntry* entry = &TOKEN_MAP[i];
        StringView entry_text_sv = (StringView) {
            .ptr = (char*)entry->text,
            .len = strlen(entry->text)
        };

            
        if(sv_cmp(sv, entry_text_sv)) {
            
            token.text = NULL;
            token.kind = entry->kind;
            found_token_kind = true;
        } 
    }


    if(!found_token_kind) {
        bool is_literal 
             = sv.ptr[0] == '"'
            || (sv.ptr[0] >= '0' && sv.ptr[0] <= '9');

        token.text = sv_strdup(sv);
        
        if(!is_literal) {
            token.kind = TOK_IDENTIFIER;
        }
        else {
            token.kind = TOK_LITERAL;
        }
    }


    if(token_array->count+1 >= token_array->allocated_count) {
        token_array->allocated_count = token_array->count + 64;
        token_array->tokens
            = realloc
            (
                token_array->tokens,
                token_array->allocated_count * sizeof *token_array->tokens
            );
    }

    token_array->tokens[token_array->count] = token;
    token_array->count += 1;
}

TokenArray* tokenize_data(char* data, size_t size) {
    TokenArray* token_array = malloc(sizeof *token_array);
    token_array->tokens = NULL;
    token_array->count = 0;
    token_array->allocated_count = 0;


    StringView line = {0};
    char* end = data + size;
    char* iter = data;

    while((iter = get_word(&line, iter, end, "\n", 0))) {

        if(line.len > 0 && line.ptr[0] == TOK_COMMENT_CHAR) {
            continue;
        }
       
        //print_sv(&line);
        //printf("\n");


        StringView word = {0};
        char* word_end = line.ptr + line.len;
        char* word_iter = line.ptr;
        while((word_iter = get_word(&word, word_iter, word_end, CONCATABLE_TOKENS, '\"'))) {
            add_token(token_array, word);

            printf("\033[32m(\033[0m");
            print_sv(&word);
            printf("\033[32m)\033[0m");
        }
            
        printf("\n");
    }


    return token_array;
}



