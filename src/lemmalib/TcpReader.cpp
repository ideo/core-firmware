#include "TcpReader.h"

//#include <EthernetClient.h>
#include "spark_wiring_tcpclient.h"
#include "spark_wiring_usbserial.h"


TcpReader::TcpReader( TCPClient& client ) :
    client( client )
{ }

char* TcpReader::read()
{
  int length = readPayloadLength();
  return readPayload( length );
}


int TcpReader::readPayloadLength()
{
  char lengthBuffer[7];
  readBlocked( lengthBuffer, 6 );
  lengthBuffer[6] = 0;

  int length = atoi( lengthBuffer );

  Serial.print("payload length: ");
  Serial.println(length);

  return length;
}

char* TcpReader::readPayload( int length )
{
  char* payload = (char*) malloc( length + 1 );
  if ( payload == 0 )
  {
    return 0;
  }
  readBlocked( payload, length );
  payload[ length ] = 0;

  return payload;
}

void TcpReader::readBlocked( char* destination, int length )
{
  int bytesRead = 0;
  while( bytesRead != length )
  {
    bytesRead += client.read( (uint8_t*) destination + bytesRead, length - bytesRead );
  }
}

