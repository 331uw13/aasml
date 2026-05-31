#ifndef STACK_FRAME_H
#define STACK_FRAME_H

#include <stdint.h>
#include "../types.h"

// TODO: Replace this later with a hashmap or something.
//       this may get pretty slow...

#define MAX_NESTED_STACKFRAMES 256



// This struct's element values are assembler and platform specific.
typedef struct VariableSize {
    uint32_t in_bytes;
    char*    asm_spec;
}
VariableSize;


typedef struct StackFrameVarInfo_t {
    char*        name;
    VariableSize size;

    int          offset;   // Set by 'stackframe_add()'
    VarType      var_type; // Not needed to be set in assembly code generation step.
}
StackFrameVarInfo;

typedef struct StackFrame_t {
    StackFrameVarInfo* variables;
    size_t             variables_count;
    size_t             variables_alloc;
    int                next_offset;
}
StackFrame;


StackFrame stackframe_init_empty();
void stackframe_free                     (StackFrame* sf);
StackFrameVarInfo* stackframe_add        (StackFrame* sf, StackFrameVarInfo varinfo);
StackFrameVarInfo* stackframe_get        (StackFrame* sf, char* var_name);


#endif
