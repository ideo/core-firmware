#include "MessageParser.h"
#include <stddef.h>
#include <cJSON.h>
#include "Event.h"
#include <string.h>

bool MessageParser::parsePolo( char const * message, char * name, int nameSize, uint16_t& outPort) {
  cJSON* json = cJSON_Parse( message );

  if(json == NULL || json->type != cJSON_Array) { return false; }
  cJSON* messageType = cJSON_GetArrayItem( json, 0 );
  cJSON* roomName = cJSON_GetArrayItem( json, 1 );
  if (roomName == NULL) { return false; }
  strncpy(name, roomName->valuestring, nameSize);

  cJSON* portNumber = cJSON_GetArrayItem( json, 2 );
  if (portNumber == NULL) { return false; }
  outPort = portNumber->valueint;
  return true;
}

Event MessageParser::parse( char const * message )
{
  cJSON* json = cJSON_Parse( message );

  if(json->type != cJSON_Array) {
    Event result;
    return result;
  }

  cJSON* eventName = cJSON_GetArrayItem( json, 2 );
  cJSON* eventValue = cJSON_GetArrayItem( json, 3 );

  switch(eventValue->type)
  {
    case cJSON_Number:
      {
        Event result( eventName->valuestring, eventValue->valueint );
        result.floatValue = eventValue->valuedouble;
        cJSON_Delete( json );
        return result;
      }

    case cJSON_False:
    case cJSON_True:
      {
        Event result( eventName->valuestring, eventValue->valueint );
        cJSON_Delete( json );
        return result;
      }

    default:
      {
        Event result( eventName->valuestring, eventValue->valuestring );
        cJSON_Delete( json );
        return result;
      }
  }
}

