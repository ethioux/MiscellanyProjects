#ifndef __json_h__
#define __json_h__

#include "list.h"
#include "array.h"
#include "tokenizer.h"

/*
 * enums for different JSON types
 */
typedef enum {
    json_invalid = 0,   // invalid type
    json_string,        // string
    json_int,           // integer (64bit long)
    json_double,        // double (floating point)
    json_array,         // array
    json_object         // json object
} jsonType;

/*
 * Definition of a JSON Object
 * Should not be accessed directly but it's not a big deal if we do
 *
 */
typedef struct jsonObject {
    char* name;                 // { name: value }
    jsonType type;              // string, int, float, array
    void* value;                // based on the type, we'll know how to cast it
    plist nodes;
} json, *pjson;

/*
 * -----------------------------------------------------------------------------
 * Public functions for the JSON parser/buidler
 * -----------------------------------------------------------------------------
 */

// Returns a JSON object (root) for the given JSON text
int JSON_parse(const char* raw_json, pjson* jsonroot);

// Retrieves the value of an item based on the given path
// Path examples: "embed.inner", "sequence", "x.y.z"
void* JSON_GetObjectItem(pjson root, const char* itemPath, jsonType* valueType);

// Print the value of an item (this is more of a debugging function)
void JSON_PrintItemValue(void* object, jsonType type);

// Stringifies the given JSON object (basically is a JSON Writer)
int JSON_Stringify(pjson object, char* buffer, int cclen);

// Creates a new JSON Object
pjson JSON_New(const char* name);

// Sets the value of the JSON object
// The value is a string that's parsed to give the value the proper type
void JSON_SetValue(pjson object, const char* value);

// Sets the value of the JSON object (for arrays only)
void JSON_SetArrayValue(pjson object, parray value);

// Adds a JSON objet to another
pjson JSON_AddObject(pjson root, pjson object);

#endif
