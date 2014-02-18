#include "DebugUtil.h"
#include "MessageSender.h"
#include "spark_wiring_tcpclient.h"
#include "MessageBuilder.h"
#include "TcpProtocol.h"
#include <nJSON.h>


MessageSender::MessageSender( const char * spallaId, TCPClient& outboundClient ) :
    messageBuilder( spallaId )
  , outboundClient( outboundClient )
{ }

bool MessageSender::sendRegistration( int listenPort, char const ** hears, int hearsCount, char const ** plays, int playsCount )
{
  PRINT_FUNCTION_PREFIX;
  PRINT("send registration on listen port: ");
  PRINTLN(listenPort);
  char* registrationMessage = messageBuilder.buildRegister( listenPort, hears, hearsCount, plays, playsCount );
  bool result = sendMessage( registrationMessage );
  free( registrationMessage );
  return result;
}

bool MessageSender::sendHeartbeat()
{
  char* heartbeatMessage = messageBuilder.buildHeartbeat();
  bool result = sendMessage( heartbeatMessage );
  free( heartbeatMessage );
  return result;
}

bool MessageSender::sendEvent( char const * name, char const * value )
{
  PRINT_FUNCTION_PREFIX;
  PRINTLN(value);
//  char* eventMessage = messageBuilder.buildEvent( name, value );
  char* eventMessage = buildEvent(name, nJSON_STRING, value);
  PRINT_FUNCTION_PREFIX;
  PRINT("eventMessage from char*: ");
  PRINTLN(eventMessage);
  bool result = sendMessage( eventMessage );
  free( eventMessage );
  return result;
}

bool MessageSender::sendEvent( char const * name, int value )
{
  PRINT_FUNCTION_PREFIX;
  PRINTLN(value);
  // char* eventMessage = messageBuilder.buildEvent( name, value );
  char* eventMessage = buildEvent(name, nJSON_INT, &value);
  PRINT_FUNCTION_PREFIX;
  PRINT("eventMessage from int: ");
  PRINTLN(eventMessage);
  bool result = sendMessage( eventMessage );
  free( eventMessage );
  return result;
}

bool MessageSender::sendEvent( char const * name, double value )
{
  PRINT_FUNCTION_PREFIX;
  PRINTLN(value);
  char* eventMessage = buildEvent(name, nJSON_DOUBLE, &value);
  PRINT_FUNCTION_PREFIX;
  PRINT("eventMessage from double: ");
  PRINTLN(eventMessage);
  bool result = sendMessage( eventMessage );
  free( eventMessage );
  return result;
}

/* [TODO] add unsigned long int type */
bool MessageSender::sendEvent( char const * name, unsigned long int value )
{
  PRINT_FUNCTION_PREFIX;
  PRINTLN(value);
  char* eventMessage = buildEvent(name, nJSON_INT, &value);
  PRINT_FUNCTION_PREFIX;
  PRINT("eventMessage from unsigned long int: ");
  PRINTLN(eventMessage);
  bool result = sendMessage( eventMessage );
  free( eventMessage );
  return result;
}

bool MessageSender::sendEvent( char const * name, long int value )
{
  PRINT_FUNCTION_PREFIX;
  PRINTLN(value);
  char* eventMessage = buildEvent(name, nJSON_LONG, &value);
  PRINT_FUNCTION_PREFIX;
  PRINT("eventMessage from unsigned long int: ");
  PRINTLN(eventMessage);
  bool result = sendMessage( eventMessage );
  free( eventMessage );
  return result;
}

bool MessageSender::sendEvent( char const * name, void * array, int size, int elemType )
{
  PRINT_FUNCTION_PREFIX;
  PRINT("array of size: ");
  PRINTLN(size);
  char* eventMessage = buildArrayEvent(name, array, size, elemType );
  PRINT_FUNCTION_PREFIX;
  PRINT("eventMessage from array: ");
  PRINTLN(eventMessage);
  bool result = sendMessage( eventMessage );
  free( eventMessage );
  return result;
}

bool MessageSender::sendMessage( char const * message )
{
  bool result = false;
  if( outboundClient.connected() )
  {
    char* encoded = TcpProtocol::encode( message );
    int sent = outboundClient.print( encoded );
    result = (sent == strlen(encoded));

    free( encoded );
  }
  return result;
}

