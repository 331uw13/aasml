#ifndef STRING_VIEW_H
#define STRING_VIEW_H

#include <stddef.h>

typedef struct {
    char* ptr;
    size_t len;
}
StringView;



extern StringView sv_trim(StringView sv);
extern bool sv_cmp(StringView a, StringView b);
extern char* sv_strdup(StringView sv);


#endif
