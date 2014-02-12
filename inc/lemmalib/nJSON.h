/* nJSON is not a JSON parser.
 * totally experimental, zero guarantee.
 */


#ifndef nJSON__h
#define nJSON__h

//#include <Arduino.h>
#include "spark_wiring.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#define  NJSON_FUNCTION_PREFIX              { \
  Serial.print("nJSON[");                     \
  Serial.print(__PRETTY_FUNCTION__);          \
  Serial.print("] ");                         \
}


typedef struct nJSONObject {
	char *name; // The item's name string, if this item is the child of, or is in the list of subitems of an object.
	struct nJSONObject *next, *prev; // next/prev allow you to walk array/object chains. Alternatively, use GetArraySize/GetArrayItem/GetObjectItem
	struct nJSONObject *child; // An array or object item will have a child pointer pointing to a chain of the items in the array/object.

	char type; // The type of the item, as above.

	union {
		char *valuestring; // The item's string, if type==nJSON_String
		char valuebool; //the items value for true & false
		int valueint; // The item's number, if type==nJSON_Number
		double valuefloat; // The item's number, if type==nJSON_Number
	};
} nJSONObject;


typedef struct nJSONObject aJsonObject;


#define aJson_False 0
#define aJson_True 1
#define aJson_NULL 2
#define aJson_Int 3
#define aJson_Float 4
#define aJson_String 5
#define aJson_Array 6
#define aJson_Object 7
#define aJson_IsReference 128

#define nJSON_INT       0
#define nJSON_FLOAT     1
#define nJSON_DOUBLE    2
#define nJSON_STRING    3
#define nJSON_BOOL      4
#define nJSON_ARRAY     5


class nJSON {
public:
	nJSON();

	void addItemToArray(nJSONObject *array, nJSONObject *item);
	nJSONObject* createArray();
	nJSONObject* createItem(char b);
	nJSONObject* createItem(int num);
	nJSONObject* createItem(double num);
	nJSONObject* createItem(const char *string);
	nJSONObject* createStringArray(const char **strings, unsigned char count);
	void deleteItem(nJSONObject *c);
	char* print(nJSONObject* item);
	nJSONObject* getArrayItem(nJSONObject *array, unsigned char item);
	nJSONObject* parse(char *value);

private:

};


/* singleton */
extern nJSON aJson;


#ifdef __cplusplus
extern "C"
{
#endif

/* C */

#ifdef __cplusplus
}
#endif


#endif

