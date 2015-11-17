#include <string.h>
#include <stdlib.h>
#include <memory.h>
#include <stdio.h>
#include "list.h"
#include "tokenizer.h"

#define TOKEN_BUFFER 512

int getQuotedString(char* str, char** raw) {
    if (str == NULL || raw == NULL or *raw == NULL)
        return 0;

    char quote_char = **raw;
    int index = 0;
    *raw += 1;
    while (**raw != quote_char) {
        str[index++] = **raw;
        *raw += 1;
    }

    return index;
}

void addToList(plist* head, char* token) {
    if (*head == NULL) {
        *head = list_create(strdup(token));
    }
    else {
        list_push(*head, strdup(token));
    }
}

#define reset_buffer()  memset(str, 0, TOKEN_BUFFER); \
                        index = 0

ptokenizer tokenize(const char* text) {
    if (text == NULL)
        return NULL;

    char* str = (char*) malloc(TOKEN_BUFFER);
    int index = 0;
    int count = 0;
    plist head = NULL;
    char* currentChar = (char*) text;

    memset(str, 0, TOKEN_BUFFER);
    while (*currentChar != 0) {
        switch (*currentChar) {
            case ' ':
                if (strlen(str) > 0) {
                    addToList(&head, str);
                    reset_buffer();
                }
                break;

            case '\'':
            case '\"':
                getQuotedString(str, &currentChar);
                addToList(&head, str);
                reset_buffer();
                break;

            case '{':
            case '}':
            case '[':
            case ']':
            case ',':
            case ':':
                if (strlen(str) > 0) {
                    addToList(&head, str);
                    reset_buffer();
                }

                str[0] = *currentChar;
                addToList(&head, str);
                str[0] = '\0';
                break;

            default:
                str[index++] = *currentChar;
                break;
        }
        currentChar++;
    }

    ptokenizer _tokenizer = (ptokenizer) malloc(sizeof(tokenizer));
    _tokenizer->tokens = head;

    return _tokenizer;
}

/*
 * Deletes the tokenizer
 *
 */
void deleteTokenizer(ptokenizer _tokenizer) {
    void* data = NULL;
    while ((data = list_pop(&_tokenizer->tokens)) != NULL) {
        free(data);
        if (_tokenizer->tokens == NULL)
            break;
    }
}
