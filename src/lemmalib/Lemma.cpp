#include "DebugUtil.h"
#include "Lemma.h"
#include "spark_wiring_network.h"
#include "Event.h"
#include "MessageParser.h"
#include "TcpReader.h"
#include "nJSON.h"

#define HEARTBEAT_PERIOD 14950

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

bool Lemma::isConnected(){
  return _connectedToHost;
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
      // srand(1);
      // delay(rand()%1000);
      // char const * ip = maestroLocater.maestroIp();
      // unsigned int port = maestroLocater.maestroPort();
      PRINT_FUNCTION_PREFIX;
      Serial.print("Connecting to Noam host @ ");
      Serial.print( udpClient.remoteIP() );
      Serial.print( ":" );
      Serial.println( 7733 );
      /* maestroConnection is of type EthernetClient, connect() is a built-in function, connects to
       * a specified IP address and port. The return value indicates success or failure. Also
       * supports DNS lookups when using a domain name. Returns true if the connection succeeds.
       */
       if( maestroConnection.connect( udpClient.remoteIP(), 7733 ) )
       {
        PRINT_FUNCTION_PREFIX;
        Serial.println("Connected to Noam host");
        _connectedToHost = true;
        /* messageSender is initialized in constructor, filer is of type EventFilter, the events
         * array in EventFilter was filled by the Lemma::hear() call when it calls the add() function
         * of EventFilter. The play array is empty, so the last 2 arguments are empty.
         */
         messageSender.sendRegistration( LISTEN_PORT, filter.events(), filter.count(), 0, 0 );
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

  if ( _connectedToHost ) {
    TCPClient incomingClient = server.available();
    int sock = incomingClient.sock();
    if (sock != MAX_SOCK_NUM) {
      TcpReader reader( incomingClient );
      char* message = reader.read();
      // Release the incoming client
      int res = incomingClient.stop();
      Serial.print(res);
      if (message) {      
        Event const & event = MessageParser::parse( message );
        filter.handle(event);
        memset(message, 0, strlen(message)*sizeof(char));
        free(message);
      }
      else {        
        Serial.println("FAILED to read TCP, max_sock_num");
      }
    }
  }  
}

void Lemma::testHeartbeat(){
  if( false == _connectedToHost ){
    return;
  }
  if( millis() - heartbeatTimer > HEARTBEAT_PERIOD ){
    heartbeatTimer = millis();
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
