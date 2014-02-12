#include "DebugUtil.h"
#include "MessageParser.h"
#include <nJSON.h>
#include "Event.h"


#define PARSER_BUF_SIZE 32


#ifdef __cplusplus
extern "C"
{
#endif

/* return value is nJSON_INT | nJSON_FLOAT | nJSON_DOUBLE | nJSON_STRING, event in case of array data,
 * the value type returned is the type of array element, the existance of array is indicated by the 
 * resulting content of void ** array not being NULL
 */
int parseCodedString( char const * message, char * messageType, char * lemmaID, char * messageName, void * value, void ** array, int * size )
{
  int valueType = -1;

  char *orig = (char *)calloc(1, strlen(message) * sizeof(char));
  strncpy(orig, message+1, strlen(message)-2);
  char *token = NULL;
  token = strtok (orig, ",");
  int index = 0;
  bool isArray = false;
  int arrayIndex = 0;
  int *intArray = NULL;
  double *doubleArray = NULL;
  char **stringArray = NULL;
  char tempToken[PARSER_BUF_SIZE];

  while (NULL != token)
  {
    switch (index) {
      case 0:
        /* message type, in this case should be "event" */
        if (strcmp("\"event\"", token)) {
          free(orig);
          return valueType;
        }
        else {
          strcpy(messageType, token);
          index++;
        }
      break;
      case 1:
        /* Lemma ID */
        PRINT_FUNCTION_PREFIX;
        PRINT("lemma ID: ");
        PRINTLN(token);
        strncpy(lemmaID, token+1, strlen(token)-2);
        index++;
      break;
      case 2:
        /* message name */
        PRINT_FUNCTION_PREFIX;
        PRINT("message name: ");
        PRINTLN(token);
        strncpy(messageName, token+1, strlen(token)-2);
        index++;
      break;
      case 3:
        /* value can be primitive or array of identical primitive type elements. 
         * when isArray is true, this is going through the individual elements of the array
         */
        PRINT_FUNCTION_PREFIX;
        PRINT("message value: ");
        PRINTLN(token);
        if ('"' == token[0] && '"' == token[strlen(token)-1] && strlen(token) >= 2) {
          PRINT_FUNCTION_PREFIX;
          PRINTLN("string");
          valueType = nJSON_STRING;
          if (true == isArray) {
            if (NULL == intArray && NULL == doubleArray) {
              /* fill in stringArray */
              if (NULL == stringArray) {
                stringArray = (char **)calloc(16, sizeof(char *));
                arrayIndex = 0;
                tempToken[strlen(tempToken)-1] = '\0';
                stringArray[arrayIndex++] = strdup(tempToken+1);
              }
              sprintf(tempToken, "%s", token+1);
              tempToken[strlen(token)-2] = '\0';
              stringArray[arrayIndex++] = strdup(tempToken);
            }
            else {
              free(orig);
              return valueType;
            }
          }
          else {
            strncpy((char *)value, token+1, strlen(token)-2);
          }
        }
        else if ('[' == token[0]) {
          if (true == isArray) {
              free(orig);
              return valueType;
          }
          isArray = true;
          memset(tempToken, 0, PARSER_BUF_SIZE);
          //strncpy(tempToken, token+1, strlen(token)-2);
          sprintf(tempToken, "%s", token+1);
          tempToken[strlen(token)] = '\0';
        }
        else if (']' == token[strlen(token)-1]) {
          memset(tempToken, 0, PARSER_BUF_SIZE);
          sprintf(tempToken, "%s", token);
          tempToken[strlen(token)-1] = '\0';
          
          /* finish array, and set type to nJSON_ARRAY */
          if (true == isArray) {
            isArray = false;
            if (NULL != intArray) {
              intArray[arrayIndex++] = atoi(tempToken);
              *array = intArray;
            }
            else if (NULL != doubleArray) {
              doubleArray[arrayIndex++] = atof(tempToken);
              *array = doubleArray;
            }
            else if (NULL != stringArray) {
              tempToken[strlen(tempToken)-1] = '\0';
              stringArray[arrayIndex++] = strdup(tempToken+1);
              *array = stringArray;
            }
            *size = arrayIndex;
          }
          else {
            free(orig);
            return valueType;
          }
        }
        else if ('{' == token[0]) {
          free(orig);
          return valueType;
        }
        else {
          PRINT_FUNCTION_PREFIX;
          PRINTLN("number");
          /* determine if it is integer or floating point number */
          if (!strchr(token, '.')) {
            PRINT_FUNCTION_PREFIX;
            PRINTLN("integer");
            valueType = nJSON_INT;
            if (true == isArray) {
              /* this int is part of an array */
              if (NULL == doubleArray && NULL == stringArray) {
                /* fill in intArray */
                if (NULL == intArray) {
                  intArray = (int *)calloc(16, sizeof(int));
                  arrayIndex = 0;
                  intArray[arrayIndex++] = atoi(tempToken);
                }
                intArray[arrayIndex++] = atoi(token);
              }
              else {
                free(orig);
                return valueType;
              }
            }
            else {
              /* there is no array, the event contains only a single data element */
              int buf = 0;
              if (1 == sscanf(token, "%d", &buf)) {
                *((int *)value) = buf;
              }
              else {
                free(orig);
                return valueType;
              }
            }
          }
          else {
            PRINT_FUNCTION_PREFIX;
            PRINTLN("float (or double)");
            valueType = nJSON_DOUBLE;
            if (true == isArray) {
              if (NULL == intArray && NULL == stringArray) {
                /* fill in doubleArray */
                if (NULL == doubleArray) {
                  doubleArray = (double *)calloc(16, sizeof(double));
                  arrayIndex = 0;
                  doubleArray[arrayIndex++] = atof(tempToken);
                }
                doubleArray[arrayIndex++] = atof(token);
              }
              else {
                free(orig);
                return valueType;
              }
            }
            else {
              *((double *)value) = atof(token);
              PRINT_FUNCTION_PREFIX;
              PRINTLN(*((double *)value));
            }
          }
        }
        if (false == isArray) {
          index++;
        }
      break;
      default:
      break;
    }
    token = strtok (NULL, ",");
  }
  free(orig);

  return valueType; 
}


#ifdef __cplusplus
}
#endif


Event MessageParser::parse( char const * message )
{
  char messageType[32];
  char lemmaID[32];
  char messageName[32];
  void *valueBuf = calloc(32, sizeof(char));
  void *array = NULL;
  int size = 0;
  memset(valueBuf, 0, 32*sizeof(int));
  memset(messageType, 0, 32);
  memset(lemmaID, 0, 32);
  memset(messageName, 0, 32);
  int valueType = parseCodedString(message, messageType, lemmaID, messageName, valueBuf, &array, &size);
  bool isArray = (NULL != array);

  switch (valueType) 
  {
    case nJSON_INT: 
    {
      if (true == isArray) {
        Event result(messageName, (int *)array, size);
        return result;
      }
      else {
        Event result(messageName, *((int *)valueBuf));
        free(valueBuf);
        return result;
      }
    }
    break;
    case nJSON_FLOAT:
    case nJSON_DOUBLE:
    {
      if (true == isArray) {
        Event result(messageName, (double *)array, size);
        return result;
      }
      else {
        Event result(messageName, *((double *)valueBuf));
        free(valueBuf);
        return result;
      }
    }
    break;
    case nJSON_BOOL:
    {
      Event result(messageName, 0 == *((unsigned int *)valueBuf) ? false : true);
      free(valueBuf);
      return result;
    }
    break;
    case nJSON_STRING:
    {
      if (true == isArray) {
        Event result(messageName, (char **)array, size);
        return result;
      }
      else {
        Event result(messageName, (char *)valueBuf);
        free(valueBuf);
        return result;
      }
    }
    break;
    default:
    {
      Event result("UNKOWN MESSAGE NAME", "INVALID VALUE");
      free(valueBuf);
      return result;
    }
  }
}

