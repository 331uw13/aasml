#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "fileio.h"
#include "compiler.h"



int main() {
    size_t file_size = 0;
    char* file = NULL;
    const char* file_name = "test.aasml";
    
    if(!map_file(file_name, PROT_READ, &file, &file_size)) {
        return 1;
    }

    set_log_context((LogContext) {
        .file_name = (char*)file_name,
        .file_data = file,
        .file_size = file_size
    });
    
    TokenArray* token_array = tokenize_data(file, file_size);
    for(size_t i = 0; i < token_array->count; i++) {
        Token* token = &token_array->tokens[i];
        printf("\033[32m%s\033[0m ", token_kind_to_str(token->kind));
        if(token->text != NULL) {
            printf("%s", token->text);
        }
        printf("\n");
        //printf("%s\n", text == NULL ? "null" : text);
    }

    munmap(file, file_size);





    IRcmdArray* ircmds = intermediate_codegen(token_array);
  
   
    int file_out_fd = open("output.asm", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);


    cg_set_altoutput_fd(file_out_fd);
    codegen_gnu_as_x86_64(ircmds);

    printf("\033[1;32m(assemble, link and execute)\033[0m\n");
    system("./assemble_and_link.sh output.asm");

    close(file_out_fd);
    printf("\n\n");
    return 0;
}

