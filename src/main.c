#include <stdio.h>

#include "fileio.h"
#include "compiler.h"



int main() {
    size_t file_size = 0;
    char* file = NULL;
    const char* file_name = "test.asm";
    
    if(!map_file(file_name, PROT_READ, &file, &file_size)) {
        return 1;
    }

    set_log_context((LogContext) {
        .file_name = (char*)file_name,
        .file_data = file,
        .file_size = file_size
    });
    
    TokenArray* token_array = tokenize_data(file, file_size);
    printf("========== Tokenizer output ==========\n");
    for(size_t i = 0; i < token_array->count; i++) {
        Token* token = &token_array->tokens[i];
        printf("\033[32m%s\033[0m ", token_kind_to_str(token->kind));
        if(token->text != NULL) {
            printf("%s", token->text);
        }
        printf("\n");
        //printf("%s\n", text == NULL ? "null" : text);
    }


    IRcmdArray* ircmds = intermediate_codegen(token_array);
    printf("========== IR gen output ==========\n");
    
    for(size_t i = 0; i < ircmds->count; i++) {
        ircmd_print(&ircmds->commands[i]);
    }


    munmap(file, file_size);
    return 0;
}
