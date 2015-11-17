#include <stdlib.h>
#include <memory.h>
#include <stdio.h>
#include <errno.h>
#include "json.h"
#include "tokenizer.h"
#include "array.h"

/*
 * Get the type of value from a json token
 * if it's all digits, it's an int, if all digits and one period, double
 * otherwise, string
 */
jsonType getValueType(const char* string) {
    if (string == NULL)
        return json_invalid;

    jsonType type = json_int;
    char* p = (char*) string;
    while (*p) {
        // If the character isn't a digit nor a period, we
        // return a string type
        if (!isdigit(*p) && *p != '.') {
            return json_string;
        }

        // The character was a digit or a period. If it's a period we check
        // if the type is currently an int. If it is, we set it to double
        // if it's already a double, then we have an invalid double, so we return invalid
        // Maybe I can return string instead
        //
        if (*p == '.') {
            if (type == json_int)
                type = json_double;
            else
                return json_invalid;
        }

        p++;
    }

    return type;
}

/*
 * Set the value for the give item
 * Based on the type, we create either a new string or a new int or a new
 * double.
 * Those pointers will have to be deleted once the json item is freed
 *
 */
void setItemValue(pjson item, const char* str) {
    long *longValue = NULL;
    double* doubleValue = NULL;
    if (str == NULL)
        return;

    switch (item->type) {
        case json_string:
            item->value = strdup(str);
            break;
        case json_int:
            longValue = malloc(sizeof(long));
            *longValue = atol(str);
            item->value = longValue;
            break;
        case json_double:
            doubleValue = malloc(sizeof(double));
            *doubleValue = atof(str);
            item->value = doubleValue;
            break;
    }
}

int AppendToString(char* string, int cclen, char* concat) {
    if (string == NULL || concat == NULL)
        return ENOMEM;

    if (strlen(string) + strlen(concat) < cclen) {
        strcat(string, concat);
        return 0;
    }

    return ENOMEM;
}

/*
 * The value given as a list of tokens represent an array
 * We go through it until we find the closing array symbol: ]
 *
 */
void* parseArray(plist* tokens) {
    if (tokens == NULL)
        return NULL;

    plist curr = *tokens;
    parray _array = array_create();
    while (curr != NULL) {
        char* cdata = (char*) curr->data;
        if (strcmp(cdata, "]") == 0)
            break;
        if (strcmp(cdata, ",") != 0)
            array_add(_array, cdata);

        curr = getnext(curr);
    }

    *tokens = curr;
    return _array;
}

#define newJSON(a) a = (pjson) malloc(sizeof(json)); \
                   memset(a, 0, sizeof(json));


/*
 * This is the main parser
 * it is recursive and that is used to parse embeded json items
 *
 */
pjson parse(plist* tokens, char* name) {
    if (tokens == NULL || name == NULL)
        return NULL;

    plist curr = *tokens;
    pjson object, child;

    newJSON(object);
    newJSON(child);
    object->type = json_object;
    object->name = strdup(name);

    while (curr != NULL) {
        char* cdata = (char*) curr->data;
        switch (*cdata) {
            case '}':
                if (object->nodes == NULL) {
                    object->nodes = list_create(child);
                }
                else {
                    list_push(object->nodes, child);
                }

                *tokens = curr;
                return object;

            case '{':
                if (child != NULL ) {
                    pjson childSave = child;
                    curr = getnext(curr);
                    child = parse(&curr, child->name);
                    free(childSave->name);
                    free(childSave);
                }
                break;
            case ':':
                curr = getnext(curr);
                cdata = (char*) curr->data;
                if (*cdata == '[') {
                    // parse the array
                    curr = getnext(curr);
                    child->type = json_array;
                    child->value = parseArray(&curr);
                }
                else
                if (*cdata == '{') { // new object so we let the current loop do it
                    continue;
                } else {
                    // Not an array or a new object
                    //
                    if (child->type == json_invalid) {
                        child->type = getValueType(cdata);
                        setItemValue(child, cdata);
                    }
                }

                break;

            case ',':
                if (object->nodes == NULL) {
                    object->nodes = list_create(child);
                }
                else {
                    list_push(object->nodes, child);
                }

                newJSON(child);
                break;

            default:
                child->name = strdup(cdata);
                break;
        }

        curr = getnext(curr);
    }

    *tokens = curr;
    return object;
}

/*
 * This is the exposed function to parse a JSON text
 * First, we tokenize the whole text to make the parsing easier
 * Then we parse it.
 *
 */
int JSON_parse(const char* raw_json, pjson* jsonroot) {
    int err = 0;
    if (raw_json == NULL || jsonroot == NULL)
        return EFAULT;

    ptokenizer _tokenizer = tokenize(raw_json);

    if (strcmp((char*) _tokenizer->tokens->data, "{") == 0) {
        plist tokens = getnext(_tokenizer->tokens);
        *jsonroot = parse(&tokens, "root");
    }
    else {
        printf("Error, the json payload should start with {");
        err = EINVAL;
    }

    deleteTokenizer(_tokenizer);

    return err;
}

/*
 * This returns an array of strings representing a path for a json item
 * for instance, we can use "item.subitem.subitem"
 *
 */
parray parsePath(const char* itemPath) {
    if (itemPath == NULL)
        return array_create("");

    char* path = strdup(itemPath);
    char* item = strtok(path, ".");
    parray a = array_create();
    while (item != NULL) {
        array_add(a, item);
        item = strtok(NULL, ".");
    }

    free(path);
    return a;
}

/*
 * Returns the value of an item from a json object
 *
 */
void* JSON_GetObjectItem(pjson root, const char* itemPath, jsonType* valueType) {
    if (root == NULL || itemPath == NULL || valueType == NULL)
        return NULL;

    plist curr = root->nodes;
    if (curr == NULL)
        return NULL;

    parray path = parsePath(itemPath);
    pjson item = NULL;
    int i = 0;

    for (i = 0; i < path->size; i++) {
        while (curr != NULL) {
            item = (pjson) curr->data;
            if (strcmp(item->name, path->items[i]) != 0)
                curr = getnext(curr);
            else
                break;
        }

        if (item != NULL & ((i + 1) < path->size)) {
            curr = item->nodes;
        }
    }

    array_clear(path);

    if (item != NULL) {
        *valueType = item->type;
        return item->value;
    }

    return NULL;
}

/*
 * prints the given value based on its type
 *
 */
void JSON_PrintItemValue(void* value, jsonType type) {
    parray a = NULL;
    int i = 0;
    int size = 0;

    if (value != NULL) {
        switch (type) {
            case json_string:
                printf("value: %s\n", (char*) value);
                break;
            case json_int:
                printf("value: %l\n", *(long*) value);
                break;
            case json_double:
                printf("value: %f\n", *(double*) value);
                break;
            case json_array:
                a = (parray) value;
                size = array_size(a);
                for (i = 0; i < size; i++) {
                    char* avalue = array_getAt(a, i);
                    if (avalue != NULL)
                        printf("value[%d]: %s\n", i, avalue);
                }
        }
    }
}

/*
 * create a JSON string from the given object
 *
 */
int JSON_Stringify(pjson object, char* buffer, int cclen) {
    // Inner buffer should not be defined like this because that's dumb
    // Will fix later
    //
    char innerBuffer[512] = {0};
    if (object == NULL)
        return EFAULT;

    if (object->type == json_object) {
        if (AppendToString(buffer, cclen, "{ ") == ENOMEM)
            return ENOMEM;

        plist curr = object->nodes;
        while (curr != NULL) {
            pjson item = (pjson) curr->data;
            parray a = NULL;
            int size = 0;
            int i = 0;

            if (item != NULL) {
                sprintf(innerBuffer, "\"%s\": ", item->name);
                if (AppendToString(buffer, cclen, innerBuffer) == ENOMEM)
                    return ENOMEM;

            }
            switch (item->type) {
                case json_object:
                    JSON_Stringify(item, buffer, cclen);
                    break;
                case json_string:
                    sprintf(innerBuffer, "\"%s\"", (char*) item->value);
                    break;
                case json_int:
                    sprintf(innerBuffer, "%ld", *(long*) item->value);
                    break;
                case json_double:
                    sprintf(innerBuffer, "%f", *(double*) item->value);
                    break;
                case json_array:
                    a = (parray) item->value;
                    size = array_size(a);
                    if (AppendToString(buffer, cclen, "[ ") == ENOMEM)
                        return ENOMEM;

                    for (i = 0; i < size; i++) {
                        char* avalue = array_getAt(a, i);
                        if (avalue != NULL) {
                            sprintf(innerBuffer, "\"%s\" %c", avalue, i + 1 < size ? ',' : ' ');
                            if (AppendToString(buffer, cclen, innerBuffer) == ENOMEM)
                                return ENOMEM;
                        }
                    }
                    sprintf(innerBuffer, "]");
                    break;
            }

            if (item->type != json_object) {
                if (AppendToString(buffer, cclen, innerBuffer) == ENOMEM)
                    return ENOMEM;
            }

            curr = getnext(curr);
            if (curr != NULL) {
                if (AppendToString(buffer, cclen, ", ") == ENOMEM)
                    return ENOMEM;
            }
        }

        if (AppendToString(buffer, cclen, " }") == ENOMEM)
            return ENOMEM;
    }

    return 0;
}

/*
 * Create a new JSON object
 * Default type is json_object
 *
 */
pjson JSON_New(const char* name) {
    if (name != NULL) {
        pjson object = NULL;
        newJSON(object);
        object->type = json_object;
        object->name = strdup(name);
        return object;
    }

    return NULL;
}

/*
 * Set the value for the object
 * The value is a string that will be parsed and transformed accordingly
 *
 */
void JSON_SetValue(pjson object, const char* value) {
    if (object != NULL && value != NULL) {
        object->type = getValueType(value);
        setItemValue(object, value);
    }
}

/*
 * Same a JSON_SetValue but this is specific to arrays
 * I might modify JSON_SetValue to recognize an array from a string
 * e.g [ "xxxxx", "xxxxx", "xxxxx" ]
 *
 */
void JSON_SetArrayValue(pjson object, parray value) {
    if (object != NULL && value != NULL) {
        object->type = json_array;
        object->value = value;
    }
}

/*
 * Adds an object to another
 *
 */
pjson JSON_AddObject(pjson root, pjson object) {
    if (root != NULL && object != NULL) {
        if (root->nodes == NULL)
            root->nodes = list_create(object);
        else
            list_push(root->nodes, object);
    }

    return object;
}
