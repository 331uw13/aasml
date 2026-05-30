#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "stack_frame.h"



StackFrame stackframe_init_empty() {
    return (StackFrame) {
        .variables = NULL,
        .variables_count = 0,
        .variables_alloc = 0,
        .next_offset = 0
    };
}

void stackframe_free(StackFrame* sf) {
    if(sf->variables != NULL) {
        for(size_t i = 0; i < sf->variables_count; i++) {
            StackFrameVarInfo* varinfo = &sf->variables[i];

            if(varinfo->size.asm_spec != NULL) {
                free(varinfo->size.asm_spec);
            }
        }
        free(sf->variables);
    }
}

StackFrameVarInfo* stackframe_get(StackFrame* sf, char* var_name) {
    for(size_t i = 0; i < sf->variables_count; i++) {
        StackFrameVarInfo* varinfo = &sf->variables[i];
        if(strcmp(varinfo->name, var_name) == 0) {
            return varinfo;
        }
    }

    fprintf(stderr, "%s: %s(): Could not find variable stack frame offset!\n",
            __FILE__, __func__);
    abort();

    // Unreachable.
    return 0;
}

StackFrameVarInfo* stackframe_add(StackFrame* sf, StackFrameVarInfo varinfo) {
    if(sf->variables_count + 1 >= sf->variables_alloc) {
        sf->variables_alloc = sf->variables_count + 256;
        sf->variables
            = realloc
            (
                sf->variables,
                sf->variables_alloc * sizeof *sf->variables
            );
    }

    sf->next_offset += varinfo.size.in_bytes;
    varinfo.offset = sf->next_offset;

    sf->variables[ sf->variables_count ] = varinfo;

    StackFrameVarInfo* ret = &sf->variables[ sf->variables_count ];
    sf->variables_count++;

    return ret;
}

