#include "DebugUtil.h"
#include "Lemma.h"
#include "spark_wiring_network.h"
#include "Event.h"
#include "MessageParser.h"
#include "TcpReader.h"
#include "nJSON.h"


Lemma::Lemma( const char * lemmaId, const char * desiredRoomName ) :
    server( LISTEN_PORT )
  , messageBuilder( lemmaId )
  , messageSender( lemmaId, maestroConnection )
  , maestroLocater( udpClient, lemmaId, desiredRoomName )
{
}

void Lemma::beginEthernet(unsigned char mac[])
{
    PRINT_FUNCTION_PREFIX;
    Serial.print("IP address obtained: ");
    Serial.println(Network.localIP());
    Serial.print("on WiFi network: ");
    Serial.println(Network.SSID());
    connected = false;
}

void Lemma::begin(unsigned char mac[])
{
  beginEthernet(mac);

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
  tryConnectingWithMaestro();
  handleIncomingConnections();
}

void Lemma::hear(char const * name, handler_t callback)
{
  filter.add(name, callback);
}

void Lemma::tryConnectingWithMaestro()
{
  /* maestroConnection is of type EthernetClient, connected() is a built-in function */
  if( !maestroConnection.connected() )
  {
    maestroLocater.tryLocate();

    /* if IP is filled by MaestroLocater::tryLocate() call above, wasLocated() returns true */
    if( maestroLocater.wasLocated() )
    {
      PRINT_FUNCTION_PREFIX;
      Serial.println("maestro located");
      // srand(1);
      // delay(rand()%1000);
      // char const * ip = maestroLocater.maestroIp();
      // unsigned int port = maestroLocater.maestroPort();
      PRINT_FUNCTION_PREFIX;
      Serial.print("Connecting to Noam @ ");
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
        Serial.println("Connected to Noam server");
        connected = true;
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


void Lemma::sendEvent( char const * name, char const * value )
{
  if (false == connected) {
    return;
  }

  if (!messageSender.sendEvent( name, value ))
  {
    reset();
  }
}

void Lemma::sendEvent( char const * name, int value )
{
  if (false == connected) {
    return;
  }

  if (!messageSender.sendEvent( name, value ))
  {
    reset();
  }
}

void Lemma::sendEvent( char const * name, unsigned long value )
{
  if (false == connected) {
    return;
  }

  if (!messageSender.sendEvent( name, value ))
  {
    reset();
  }
}

void Lemma::sendEvent( char const * name, double value )
{
  if (false == connected) {
    return;
  }

  if (!messageSender.sendEvent( name, value ))
  {
    reset();
  }
}

void Lemma::sendEvent( char const * name, bool value )
{
  if (false == connected) {
    return;
  }

  if (!messageSender.sendEvent( name, value ))
  {
    reset();
  }
}

void Lemma::sendIntArray( char const * name, int * array, int size )
{
  if (false == connected) {
    return;
  }

  if (!messageSender.sendEvent( name, array, size, nJSON_INT ))
  {
    reset();
  }
}

void Lemma::sendDoubleArray( char const * name, double * array, int size )
{
  if (false == connected) {
    return;
  }

  if (!messageSender.sendEvent( name, array, size, nJSON_DOUBLE ))
  {
    reset();
  }
}

void Lemma::sendStringArray( char const * name, char ** array, int size )
{
  if (false == connected) {
    return;
  }

  if (!messageSender.sendEvent( name, array, size, nJSON_STRING ))
  {
    reset();
  }
}


void Lemma::handleIncomingConnections()
{
  if (false == connected) {
    return;
  }

  static int counter = 0;
  counter++;

  if (maestroConnection.connected()) {
    TCPClient incomingClient = server.available();
    if (incomingClient.available()) {
      TcpReader reader( incomingClient );
      char* message = reader.read();
      // Release the incoming client
      incomingClient.stop();
      if (message) {
        Serial.print(counter);
        Serial.print(" : RECEIVED: ");
        Serial.println(message);
        free(message);
      }
      else {
        Serial.print(counter);
        Serial.println(" : FAILED to read incoming data");
      }
    }
  }
  else {
    connected = false;
  }

  // TCPClient incomingClient = server.available();
  // if( incomingClient == true )
  // {
  //   PRINT_FUNCTION_PREFIX;
  //   Serial.println("incoming data available");
  //   // TcpReader reader( incomingClient );
  //   // char* message = reader.read();
  //   // if (message != 0)
  //   // {
  //   //   Event const & event = MessageParser::parse( message );
  //   //   filter.handle( event );
  //   //   free( message );
  //   // }
  // }
}

