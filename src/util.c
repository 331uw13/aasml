#include <string.h>


#include "util.h"


bool is_text_number_literal(char* text) {
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

bool is_text_string_literal(char* text) {
    if(text == NULL) {
        return false;
    }

    return text[0] == '"';
}


