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
