#include "UnitTest++.h"
#include "MessageParser.h"
#include "Event.h"

SUITE(MessageParser)
{
  TEST(parsingEventMessage) {
    const char * message = "[\"event\",\"some id\",\"a name\",\"a value\"]";
    const Event& event = MessageParser::parse( message );

    CHECK_EQUAL( "a name", event.name );
    CHECK_EQUAL( "a value", event.stringValue );
    CHECK( 0 == event.intValue );
    CHECK( 0 == event.floatValue );
  }

  TEST(parsingIntValue) {
    const char * message = "[\"event\",\"some id\",\"a name\",142]";
    const Event& event = MessageParser::parse( message );

    CHECK_EQUAL( "a name", event.name );
    CHECK_EQUAL( "142", event.stringValue );
    CHECK_EQUAL( 142 , event.intValue );
    CHECK_EQUAL( 142.0f , event.floatValue );
  }

  TEST(parsingFloatValue) {
    const char * message = "[\"event\",\"some id\",\"a name\",14.10]";
    Event event = MessageParser::parse( message );

    CHECK_EQUAL( "a name", event.name );
    CHECK_EQUAL( 14 , event.intValue );
    CHECK(abs(event.floatValue - 14.10) < 0.01 );
  }

}
