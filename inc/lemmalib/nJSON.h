/* nJSON is not a JSON parser */


#ifndef nJSON__h
#define nJSON__h


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cJson.h>


#define nJSON_INT       0
#define nJSON_FLOAT     1
#define nJSON_DOUBLE    2
#define nJSON_STRING    3
#define nJSON_BOOL      4
#define nJSON_ARRAY     5


// #define nJSON_INT       cJSON_Number
// #define nJSON_FLOAT     cJSON_Number
// #define nJSON_DOUBLE    cJSON_Number
// #define nJSON_STRING    cJSON_String
// #define nJSON_BOOL      cJSON_True
// #define nJSON_ARRAY     cJSON_Array
class nJSON {
public:
	nJSON();
private:
};


#endif

