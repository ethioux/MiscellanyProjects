#include "json.h"
#include "array.h"
#include <stdio.h>

int main() {
    json* jsonObject = NULL;
    char* buff = (char*) malloc(2048);

    JSON_parse("{" \
        "_id: \"55f4e924dc0280cf398b4567\"," \
        "sequenceID: \"EW-00000002\"," \
        "\"users\": [" \
        "    \"3948590903446507\"," \
        "    \"980111083745926\"" \
        "    ]," \
        "\"started\": 1440205510328," \
        "embed: { inner : \"inner_val\", testfloat: 12345678.9432 }" \
        "}", &jsonObject);

    memset(buff, 0, 2048);
    if (JSON_Stringify(jsonObject, buff, 2048) == 0)
        printf("Parsed: %s\n", buff);

    jsonObject = JSON_New("root");

    json* item = JSON_New("_id");
    JSON_SetValue(item, "55f4e924dc0280cf398b4567");
    JSON_AddObject(jsonObject, item);

    item = JSON_New("sequenceID");
    JSON_SetValue(item, "EW-00000002");
    JSON_AddObject(jsonObject, item);

    array* a = array_create();
    array_add(a, "744566090321683857");
    array_add(a, "487411096483838");
    item = JSON_New("users");
    JSON_SetArrayValue(item, a);
    JSON_AddObject(jsonObject, item);

    item = JSON_New("started");
    JSON_SetValue(item, "1440205510328");
    JSON_AddObject(jsonObject, item);

    json* embed = JSON_New("embed");
    JSON_AddObject(jsonObject, embed);

    item = JSON_New("inner");
    JSON_SetValue(item, "inner_val");
    JSON_AddObject(embed, item);

    item = JSON_New("testfloat");
    JSON_SetValue(item, "12345678.9432");
    JSON_AddObject(embed, item);

    jsonType type;
    item = JSON_GetObjectItem(jsonObject, "sequenceID", &type);
    JSON_PrintItemValue(item, type);

    item = JSON_GetObjectItem(jsonObject, "embed.inner", &type);
    JSON_PrintItemValue(item, type);

    item = JSON_GetObjectItem(jsonObject, "embed.testfloat", &type);
    JSON_PrintItemValue(item, type);

    item = JSON_GetObjectItem(jsonObject, "users", & type);
    JSON_PrintItemValue(item, type);

    memset(buff, 0, 2048);
    if (JSON_Stringify(jsonObject, buff, 2048) == 0)
        printf("Built: %s\n", buff);
}
