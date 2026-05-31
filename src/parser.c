#include <stdio.h>
#include <stdlib.h>

#include "compiler.h"
#include "codegen/stack_frame.h"


// TODO: this needs hexadecimal support and negative numbers.
static
bool p_is_integer_literal(char* text) {
    if(text == NULL) {
        return false;
    }

    bool is_lit = true;
    
    while(*text != '\0') {

        if(*text < '0' || *text > '9') {
            is_lit = false;
            break;
        }

        text++;
    }

    return is_lit;
}

static
bool p_is_string_literal(char* text) {
    if(text == NULL) {
        return false;
    }

    return text[0] == '"';
}



static
Operand get_operand_from_token(StackFrame* stackframe, Token* tok, const VarType* lhs_type) {
    Operand op = {0};

    if(p_is_string_literal(tok->text)) {
        op.kind = OPERAND_LITERAL_STRING;
        op.type = TYPE_STRING;
    }
    else
    if(p_is_integer_literal(tok->text)) {
        if(lhs_type == NULL) {
            fprintf(stderr, "%s: %s(): The left hand side operand type is needed to know the integer size for literal.\n",
                    __FILE__, __func__);
            abort();
        }
        

        op.kind = OPERAND_LITERAL_INT;
        op.type = *lhs_type;
    }
    else {
        StackFrameVarInfo* var_info = stackframe_get(stackframe, tok->text);
        op.kind = OPERAND_VARIABLE;
        op.type = var_info->var_type;
    }

    op.text = tok->text;
    return op;
}


void parse_tokens(TokenArray* token_array) {
    Token* token = token_array->tokens;
    Token* tokens_end = token + token_array->count;


    StackFrame stackframe = stackframe_init_empty();


    VarType current_function_ret_type = TYPE_BYTE;


    while(token < tokens_end) {
        int num_skip = 0;
       
        // Assuming this is not an operation.
        token->is_parsed = false;


        switch(token->kind) {

            case TOK_FUNC:
                {
                    token->is_parsed = true;
                    token->parsed.p_kind = P_FUNCTION_IMPL;
                    
                    Token* tok_name_ident = token + 1;
                    Token* tok_ret_type = token + 3;

                    VarType ret_type = token_kind_to_vartype(tok_ret_type->kind);

                    token->parsed.as.function_impl.name = tok_name_ident->text;
                    token->parsed.as.function_impl.ret_type = ret_type;

                    current_function_ret_type = ret_type;

                    stackframe_free(&stackframe);
                    stackframe = stackframe_init_empty();
                }
                break;


                // RHS: the variable info,
                // LHS: other variable info or literal info.
            case TOK_VAR:
                {
                    token->is_parsed = true;
                    token->parsed.p_kind = P_OPERATION;
                    Operation* operation = &token->parsed.as.operation;

                    Token* tok_name_ident = token + 1;
                    Token* tok_type       = token + 3;
                    Token* tok_init_val   = token + 5; 
                
                    num_skip = 4;

                    VarType var_type = token_kind_to_vartype(tok_type->kind);
                    
                    token->parsed.as.operation.lhs = (Operand) {
                        .text = tok_name_ident->text,
                        .type = var_type,
                        .kind = OPERAND_VARIABLE
                    };

                    operation->rhs = get_operand_from_token(&stackframe, tok_init_val, &operation->lhs.type);

                    stackframe_add(&stackframe,
                    (StackFrameVarInfo) {
                        .name = tok_name_ident->text,
                        .var_type = var_type
                    });
                }
                break;


            case TOK_MOV:
            case TOK_ADD:
            case TOK_SUB:
            case TOK_MUL:
            case TOK_DIV:
                {
                    token->is_parsed = true;
                    token->parsed.p_kind = P_OPERATION;
                    Operation* operation = &token->parsed.as.operation;

                    Token* tok_lhs = token + 1;
                    Token* tok_rhs = token + 3;

                    operation->lhs = get_operand_from_token(&stackframe, tok_lhs, NULL);
                    operation->rhs = get_operand_from_token(&stackframe, tok_rhs, &operation->lhs.type);
                }
                break;

            case TOK_RET:
                {
                    token->is_parsed = true;
                    token->parsed.p_kind = P_OPERATION;
                    Operation* operation = &token->parsed.as.operation;
                    
                    Token* tok_rhs = token + 1;
                    operation->rhs = get_operand_from_token(&stackframe, tok_rhs, &current_function_ret_type);
                }
                break;

        }

        if(token->is_parsed) {
            printf("\033[34m parsed \"%s\"\033[0m\n", token_kind_to_str(token->kind));
        }


        token += (1 + num_skip);
    }

    stackframe_free(&stackframe);
}






