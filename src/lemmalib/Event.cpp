#include "Event.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <nJSON.h>


Event::Event()
{
  this->name[0] = 0;
  this->stringValue[0] = 0;
  this->intValue = 0;
  this->floatValue = 0.0;
  this->boolValue = false;
}

Event::Event( char const * name, char const * value )
{
  strncpy( this->name, name, 128 );
  strncpy( this->stringValue, value, 128 );
  this->isArray = 0;
  this->arrayElemType = -1;
  this->array = NULL;
  this->intValue = atoi( value );
  this->floatValue = atof( value );
  this->boolValue =  value;
}

Event::Event( char const * name, int value )
{
  strncpy( this->name, name, 128 );
  snprintf( this->stringValue, 128, "%d", value );
  this->isArray = 0;
  this->arrayElemType = -1;
  this->array = NULL;
  this->intValue = value;
  this->floatValue = (double) value;
  this->boolValue =  value;
}

Event::Event( char const * name, double value )
{
  strncpy( this->name, name, 128 );
  this->isArray = 0;
  this->arrayElemType = -1;
  this->array = NULL;
  this->stringValue[0] = 0;
  this->intValue = (int) value;
  this->floatValue =  value;
  this->boolValue =  value;
}

Event::Event( char const * name, bool value )
{
  strncpy( this->name, name, 128 );
  snprintf( this->stringValue, 128, "%d", value );
  this->isArray = 0;
  this->arrayElemType = -1;
  this->array = NULL;
  this->intValue = (int) value;
  this->floatValue =  value;
  this->boolValue =  value;
}

Event::Event( Event const & other)
{
  strncpy( this->name, other.name, 128 );
  strncpy( this->stringValue, other.stringValue, 128 );
  this->isArray = other.isArray;
  this->arrayElemType = other.arrayElemType;
  this->array = other.array;
  this->intValue = other.intValue;
  this->floatValue = other.floatValue;
  this->boolValue =  other.boolValue;
}

Event::Event( char const * name, int * array, int size )
{
  strncpy( this->name, name, 128 );
  this->isArray = size;
  this->arrayElemType = nJSON_INT;
  this->array = array;
  this->stringValue[0] = 0;
  this->intValue = 0;
  this->floatValue = 0;
  this->boolValue = 0;
}


Event::Event( char const * name, double * array, int size )
{
  strncpy( this->name, name, 128 );
  this->isArray = size;
  this->arrayElemType = nJSON_DOUBLE;
  this->array = array;
  this->stringValue[0] = 0;
  this->intValue = 0;
  this->floatValue = 0;
  this->boolValue = 0;
}


Event::Event( char const * name, char ** array, int size )
{
  strncpy( this->name, name, 128 );
  this->isArray = size;
  this->arrayElemType = nJSON_STRING;
  this->array = array;
  this->stringValue[0] = 0;
  this->intValue = 0;
  this->floatValue = 0;
  this->boolValue = 0;
}


Event & Event::operator=(Event const & other)
{
  strncpy( this->name, other.name, 128 );
  strncpy( this->stringValue, other.stringValue, 128 );
  this->isArray = other.isArray;
  this->arrayElemType = other.arrayElemType;
  this->array = other.array;
  this->intValue = other.intValue;
  this->floatValue = other.floatValue;
  this->floatValue = other.boolValue;
  return *this;
}
