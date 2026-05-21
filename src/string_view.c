#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "string_view.h"


#include <stdio.h>

StringView sv_trim(StringView sv) {
    StringView sv_ret = sv;


    size_t end = sv_ret.len;

    for(ssize_t i = end-1; i >= 0; i--) {
        if(!isspace(sv_ret.ptr[i])) {
            break;
        }

        sv_ret.len--;
    }


    return sv_ret;
}

bool sv_cmp(StringView a, StringView b) {
    if(a.len != b.len) {
        return false;
    }

    for(size_t i = 0; i < a.len; i++) {
        if(a.ptr[i] != b.ptr[i]) {
            return false;
        }
    }

    return true;
}


char* sv_strdup(StringView sv) {
    char* buf = malloc(sv.len);
    memcpy(buf, sv.ptr, sv.len);
    return buf;
}
