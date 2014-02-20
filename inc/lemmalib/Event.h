#ifndef EVENT_H
#define EVENT_H

class Event {
  public:
  char name[128];
  int isArray;
  int arrayElemType;
  char stringValue[128];
  int intValue;
  long longValue;
  double floatValue;
  bool boolValue;
  void *array;

  Event();
  Event( char const * name, char const * value );
  Event( char const * name, int value );
  Event( char const * name, long value );
  Event( char const * name, double value );
  Event( char const * name, bool value );
  Event( char const * name, int * array, int size );
  Event( char const * name, double * array, int size );
  Event( char const * name, char ** array, int size );
  Event( Event const & other);

  Event & operator=(Event const & other);
};

#endif

