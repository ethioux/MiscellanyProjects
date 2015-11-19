# json

This is a small JSON Parser/Writer.
There is no Makefile or anything for the time being

To compile the test program:

gcc -g -o test array.c list.c tokenizer.c json.c test.c

array.c: a simple file to handle arrays
list.c: a file to handle linked list_push
tokenizer.c: This is where the JSON text is tokenized
json.c: Takes a tokenizer as input and parses it to create a json_object
    It is also used to write a JSON object into text.

There are some limitations to that library.

- Arrays only support strings (not objects yet)
- The tokenizer doesn't support non ANSI strings
- The currently supported data types are: string, long and double

There should be a lot more pointer checking and the internal buffers
should clearly not be defined on the stack...

How to use the code:

The JSON parser can be used easily by calling the following function:

JSON_parse(char* json_text, **json).
If the parsing was successful, the json pointer will contain a JSON object and
its children.

The jsonObject structure is defined as such:

typedef struct jsonObject {
    char* name;                 // { name: value }
    jsonType type;              // string, int, float, array
    void* value;                // based on the type, we'll know how to cast it
    plist nodes;                // nodes of the object
} json, *pjson;

Example:

The following JSON text:

{
    abc: "this is a string",
    def: 12345
}

Will be represented as such:

json->name = "root"
json->type = json_object
json->value = NULL
json->nodes = list

    list[0]->data = jsonobject
    list[0]->name = "abc"
    list[0]->type = json_string
    list[0]->value = "this is a string"
    list[0]->nodes = NULL

    list[1]->data = jsonobject
    list[1]->name = "def"
    list[1]->type = json_int
    list[1]->value = 12345
    list[1]->nodes = NULL
    
