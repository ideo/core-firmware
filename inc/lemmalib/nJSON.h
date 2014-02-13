/* nJSON is not a JSON parser */


#ifndef nJSON__h
#define nJSON__h


#include "spark_wiring.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#define nJSON_INT       0
#define nJSON_FLOAT     1
#define nJSON_DOUBLE    2
#define nJSON_STRING    3
#define nJSON_BOOL      4
#define nJSON_ARRAY     5


class nJSON {
public:
	nJSON();
private:
};


#endif

