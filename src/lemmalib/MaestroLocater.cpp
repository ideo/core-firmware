#include "DebugUtil.h"
#include "spark_wiring.h"
#include "MaestroLocater.h"
#include "spark_wiring_udp.h"
#include "spark_wiring_usbserial.h"
#include "MessageParser.h"
#include <string.h>

#define MARCO_PERIOD 2003

MaestroLocater::MaestroLocater( UDP& udpClient, const char * lemmaId, const char * desiredRoomName ) :
    udpClient( udpClient )
  , lemmaId(lemmaId)
  , roomName(desiredRoomName)
  , port( 0 )
  , locating( false )
{
  ip[0] = 0;
  restartingUDP = false;
  lastBroadcastMillis = 0;
}

void MaestroLocater::begin()
{
  ip[0] = 0;
  port = 0;
  udpClient.begin(1032);
  locating = true;
}

void MaestroLocater::sendBroadcast()
{
    char message[RX_BUF_MAX_SIZE];
    snprintf(message, RX_BUF_MAX_SIZE, "[\"marco\", \"%s\", \"%s\", \"spark\", \"1.1\"]", lemmaId, roomName);
    Serial.print("Sending Marco message: "); Serial.println( message );
    IPAddress ipAddr( 255 , 255 , 255 , 255 );
    udpClient.beginPacket( ipAddr , 1030 );
    size_t totalSent = 0;
    int iter = 0;
    size_t messageSize = strlen(message);
    while(totalSent != messageSize && ++iter < 5) {
      totalSent = udpClient.write((uint8_t*)message, messageSize);
    }
}


/* tryLocate() only attempts to get an UDP datagrame and parse IP:port from it, does not actually
 * attempt to connect to server (this is a non-blocking call that returns immediately), the
 * connecting attempt is made by its caller, Lemma::tryConnectingWithMaestro()
 */
void MaestroLocater::tryLocate()
{
  if (!locating) { begin(); }

  unsigned long currentMillis = millis();
  if (currentMillis - lastBroadcastMillis > MARCO_PERIOD || currentMillis < lastBroadcastMillis)
  {
    sendBroadcast();
    lastBroadcastMillis = currentMillis;
  }

  /* udpClient is instance of EthernetUDP class, passed to constructor, the parsePacket() function
   * checks for the presence of a UDP packet, and reports the size. parsePacket() must be called
   * before reading the buffer with UDP.read(packetBuffer, MaxSize)
   */
  if( udpClient.parsePacket() )
  {
    char packet[RX_BUF_MAX_SIZE];
    size_t bytesRead = udpClient.read( packet, RX_BUF_MAX_SIZE );
    packet[bytesRead] = 0;

    PRINT_FUNCTION_PREFIX;
    Serial.print("raw datagram: ");
    Serial.println(packet);

    char name[128];
    if ( MessageParser::parsePolo( &packet[0], name, 128, port ) )
    {
      PRINT_FUNCTION_PREFIX;
      Serial.println("captured valid UDP datagram");

      address = udpClient.remoteIP();
      sprintf( ip, "%d.%d.%d.%d", address[0], address[1], address[2], address[3] );

      PRINT_FUNCTION_PREFIX;
      Serial.print("IP from UDP datagram: ");
      Serial.println(ip);
      PRINT_FUNCTION_PREFIX;
      Serial.print("port from UDP datagram: ");
      Serial.println(port);
      locating = false;
    }
    else
    {
      PRINT_FUNCTION_PREFIX;
      Serial.println("not a valid UDP datagram from Noam server broadcast");
      port = 0;
    }
    udpClient.stop();
  }
}

bool MaestroLocater::wasLocated()
{
  return ip[0] != 0;
}

char const * MaestroLocater::maestroIp()
{
  return ip;
}

uint16_t MaestroLocater::maestroPort()
{
  return port;
}

IPAddress MaestroLocater::maestroIpAddress()
{
  return address;
}

