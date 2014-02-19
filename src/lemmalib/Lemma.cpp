#include "DebugUtil.h"
#include "Lemma.h"
#include "spark_wiring_network.h"
#include "Event.h"
#include "MessageParser.h"
#include "TcpReader.h"
#include "nJSON.h"

#define HEARTBEAT_PERIOD 4950

Lemma::Lemma( const char * lemmaId, const char * desiredRoomName ) :
server( LISTEN_PORT )
, messageBuilder( lemmaId )
, messageSender( lemmaId, maestroConnection )
, maestroLocater( udpClient, lemmaId, desiredRoomName )
{
  heartbeatTimer = millis();
}

void Lemma::displayIP()
{
  PRINT_FUNCTION_PREFIX;
  Serial.print("IP address obtained: ");
  Serial.println(Network.localIP());
  Serial.print("on WiFi network: ");
  Serial.println(Network.SSID());
  _connectedToHost = false;
}

void Lemma::begin()
{
  displayIP();

  PRINT_FUNCTION_PREFIX;
  Serial.println("starting EthernetServer");
  /* server member varialble is of type EthernetServer, initialized with port LISTEN_PORT
   * in the constructor, the begine() call tells the server to begin listening for incoming
   * connections
   */
  // server.begin();

   PRINT_FUNCTION_PREFIX;
   Serial.println("starting EthernetUDP");
  /* updClient member variable is of type EthernetUDP, EthernetUDP.begin(localPort) initializes
   * the Ethernet UDP library and network settings.
   */

   tryConnectingWithMaestro();
 }

 void Lemma::run()
 {
  /* both are actually periodic polls, instead of callbacks */
  testConnection();
  tryConnectingWithMaestro();
  handleIncomingConnections();
  testHeartbeat();
}

void Lemma::hear(char const * name, handler_t callback)
{
  filter.add(name, callback);
}

void Lemma::testConnection(){
  _connectedToHost = maestroConnection.connected();
}

void Lemma::tryConnectingWithMaestro()
{
  /* maestroConnection is of type TCPClient, connected() is a built-in function */
  //TODO: remove debug prints
  if( !_connectedToHost )
  {
    maestroLocater.tryLocate();

    /* if IP is filled by MaestroLocater::tryLocate() call above, wasLocated() returns true */
    if( maestroLocater.wasLocated() )
    {
      PRINT_FUNCTION_PREFIX;
      Serial.println("host located");
      char const * ip = maestroLocater.maestroIp();
      unsigned int port = maestroLocater.maestroPort();
      PRINT_FUNCTION_PREFIX;
      Serial.print("Connecting to Noam host @ ");
      Serial.print( ip );
      Serial.print( ":" );
      Serial.println( port );
       if( maestroConnection.connect( ip, port ) )
       {
        /* messageSender is initialized in constructor, filer is of type EventFilter, the events
         * array in EventFilter was filled by the Lemma::hear() call when it calls the add() function
         * of EventFilter. The play array is empty, so the last 2 arguments are empty.
         */
         bool successfulRegistration = messageSender.sendRegistration( LISTEN_PORT, filter.events(), filter.count(), 0, 0 );
         if(successfulRegistration) {
           _connectedToHost = true;
         }
       }
     }
   }
 }


 void Lemma::reset()
 {
  PRINT_FUNCTION_PREFIX;
  Serial.println("reset maestro connection after failed to send event to NOAM server");
  maestroConnection.stop();
}

bool Lemma::sendEvent( char const * name, char const * value )
{
  if (false == _connectedToHost) {
    return false;
  }

  if (!messageSender.sendEvent( name, value ))
  {
    reset();
    return false;
  }
  return true;
}

bool Lemma::sendEvent( char const * name, int value )
{
  if (false == _connectedToHost) {
    return false;
  }

  if (!messageSender.sendEvent( name, value ))
  {
    reset();
    return false;
  }
  return true;
}

bool Lemma::sendEvent( char const * name, unsigned long value )
{
  if (false == _connectedToHost) {
    return false;
  }

  if (!messageSender.sendEvent( name, value ))
  {
    reset();
    return false;
  }
  return true;
}

bool Lemma::sendEvent( char const * name, long value )
{
  if (false == _connectedToHost) {
    return false;
  }

  if (!messageSender.sendEvent( name, value ))
  {
    reset();
    return false;
  }
  return true;
}

bool Lemma::sendEvent( char const * name, double value )
{
  if (false == _connectedToHost) {
    return false;
  }

  if (!messageSender.sendEvent( name, value ))
  {
    reset();
    return false;
  }
  return true;
}

bool Lemma::sendEvent( char const * name, bool value )
{
  if (false == _connectedToHost) {
    return false;
  }

  if (!messageSender.sendEvent( name, value ))
  {
    reset();
    return false;
  }
  return true;
}

bool Lemma::sendIntArray( char const * name, int * array, int size )
{
  if (false == _connectedToHost) {
    return false;
  }

  if (!messageSender.sendEvent( name, array, size, nJSON_INT ))
  {
    reset();
    return false;
  }
  return true;
}

bool Lemma::sendDoubleArray( char const * name, double * array, int size )
{
  if (false == _connectedToHost) {
    return false;
  }

  if (!messageSender.sendEvent( name, array, size, nJSON_DOUBLE ))
  {
    reset();
    return false;
  }
  return true;
}

bool Lemma::sendStringArray( char const * name, char ** array, int size )
{
  if (false == _connectedToHost) {
    return false;
  }

  if (!messageSender.sendEvent( name, array, size, nJSON_STRING ))
  {
    reset();
    return false;
  }
  return true;
}


void Lemma::handleIncomingConnections()
{
  if (false == _connectedToHost) {
    return;
  }

  TCPClient incomingClient = server.available();
  if (incomingClient.sock() != MAX_SOCK_NUM) {
    TcpReader reader( incomingClient );
    char* message = reader.read();
    // Release the incoming client
    incomingClient.stop();

    if (message) {
      // Serial.print("RECEIVED: ");
      // Serial.println(message);

      Event const & event = MessageParser::parse( message );
      filter.handle(event);
      memset(message, 0, strlen(message)*sizeof(char));
      free(message);
    }
  }
}

void Lemma::testHeartbeat(){
  if( false == _connectedToHost ){
    return;
  }
  unsigned long now = millis();
  if( now < heartbeatTimer || now - heartbeatTimer > HEARTBEAT_PERIOD ){
    heartbeatTimer = now;
    bool result = messageSender.sendHeartbeat();
    //TODO: remove debug
    Serial.print("heartbeat: "); Serial.print(millis()); Serial.print(" ok?: "); Serial.println(result);
    if( !result ){
      Serial.println("Lost heartbeat connection to server");
      _connectedToHost = false;
      maestroConnection.stop();
    }
  }
}
