#ifndef MESSAGE_PARSER_H
#define MESSAGE_PARSER_H

class Event;
#include <stdint.h>

class MessageParser
{
  public:
  static Event parse( char const * message );
  static bool parsePolo( char const * message, char * name, int nameSize, uint16_t& outPort);

  private:
  MessageParser();
};

#endif

