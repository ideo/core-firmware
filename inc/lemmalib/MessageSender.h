#ifndef MESSAGE_SENDER_H
#define MESSAGE_SENDER_H

#include "MessageBuilder.h"

class TCPClient;

class MessageSender
{
  public:
    MessageSender( char const * SPALLA_ID, TCPClient& outboundClient );
    bool sendRegistration( int listenPort, char const ** hears, int hearsCount, char const ** plays, int playsCount, int heartbeat_period_sec );
    bool sendHeartbeat();
    bool sendEvent( char const * name, char const * value );
    bool sendEvent( char const * name, int value );
    bool sendEvent( char const * name, double value );
    bool sendEvent( char const * name, long unsigned int value );    
    bool sendEvent( char const * name, void * array, int size, int elemType );

  private:
    bool sendMessage( char const * message );

    MessageBuilder messageBuilder;
    TCPClient& outboundClient;
};

#endif

