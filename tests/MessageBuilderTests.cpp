#include "MessageBuilder.h"

test(createsStringMessage) {
  MessageBuilder builder("spallaId");

  String expected("[\"event\",\"spallaId\",\"speed\",\"65\"]");
  char * actual = builder.buildEvent("speed", "65");

  assertTrue(String(actual).equals(expected));
  free(actual);
}

test(createsIntMessage) {
  MessageBuilder builder("spallaId");

  String expected("[\"event\",\"spallaId\",\"speed\",65]");
  char * actual = builder.buildEvent("speed", 65);

  assertTrue(String(actual).equals(expected));
  free(actual);
}

test(createsFloatMessage) {
  MessageBuilder builder("spallaId");

  String expected("[\"event\",\"spallaId\",\"speed\",23.01000]");
  char * actual = builder.buildEvent("speed", 23.01);

  assertTrue(String(actual).equals(expected));
  free(actual);
}

test(createRegistrationMessage) {
  MessageBuilder builder("spallaId");

  char const* hears[2] = {"event1", "event2"};
  char const* plays[3] = {"event3", "event4", "event5"};

  char* expected = "[\"register\",\"spallaId\",123,[\"event1\",\"event2\"],[\"event3\",\"event4\",\"event5\"],\"arduino\",\"0.2\"]";
  char* actual = builder.buildRegister(123, hears, 2, plays, 3);

  assertTrue(String(expected).equals(String(actual)));
  free(actual);
}

