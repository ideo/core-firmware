/* nJSON is not a JSON parser.
 * totally experimental, zero guarantee.
 */


#include "DebugUtil.h"
#include "nJSON.h"


/* singleton */
nJSON aJson;


nJSON::nJSON()
{
}


void nJSON::addItemToArray(nJSONObject *array, nJSONObject *item)
{
}


nJSONObject* nJSON::createArray()
{
  return NULL;
}


nJSONObject* nJSON::createItem(char b)
{
  /* dummy test code in C */
  nJSONObject* node = (nJSONObject *) calloc(1, sizeof(aJsonObject));

  return node;
}


nJSONObject* nJSON::createItem(int num)
{
  return NULL;
}


nJSONObject* nJSON::createItem(double num)
{
  return NULL;
}


nJSONObject* nJSON::createItem(const char *string)
{
  return NULL;
}


nJSONObject* nJSON::createStringArray(const char **strings, unsigned char count)
{
  return NULL;
}


void nJSON::deleteItem(nJSONObject *c)
{
  PRINTLN("");
}


char* nJSON::print(nJSONObject* item)
{
  return NULL;
}


nJSONObject* nJSON::getArrayItem(nJSONObject *array, unsigned char item)
{
  return NULL;
}


nJSONObject* nJSON::parse(char *value)
{
  return NULL;
}


#ifdef __cplusplus
extern "C"
{
#endif

/* C */

#ifdef __cplusplus
}
#endif




