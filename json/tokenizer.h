#ifndef __tokenizer_h__
#define __tokenizer_h__

#   include "list.h"

typedef struct _tokenizer {
    plist tokens;
    int count;
} tokenizer, *ptokenizer;

ptokenizer tokenize(const char* text);
void deleteTokenizer(ptokenizer tokens);

#endif
