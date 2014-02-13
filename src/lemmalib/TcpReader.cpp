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
  int readResult = 0;
  while( bytesRead != length )
  {
    readResult = client.read( (uint8_t*) destination + bytesRead, length - bytesRead );
    if(readResult < 0){
      if(!client.available()){
        // The client.available() is responsible for fetching the amount left on buffer
        // Without this call the read will return 0 waiting for more causing this to hang.
        break;
      }
    } else {
      bytesRead += readResult;
    }
  }
}

