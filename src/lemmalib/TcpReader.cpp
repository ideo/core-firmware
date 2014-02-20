#include "TcpReader.h"
#include "spark_wiring_tcpclient.h"
#include "spark_wiring_usbserial.h"


TcpReader::TcpReader( TCPClient& client ) :
    client( client ),
    bufferPos( 0 )
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
  return length;
}

char* TcpReader::readPayload( int length )
{
  char* payload = (char*) malloc( length + 1 );
  if ( payload == 0 )
  {
    return 0;
  }
  memcpy(&buffer[0], payload, bufferPos);
  memset(buffer, 0, TCPREADER_EXTRA_BUFFER_LEN);
  readBlocked( &payload[bufferPos], length - bufferPos );
  payload[ length ] = 0;

  // Check if anything extra is on the end of this message
  String messageString(payload);
  int closingBrace = messageString.lastIndexOf(']');
  int numCharsToBuffer = strlen(payload) - (closingBrace + 1);
  if(closingBrace < 0){
    free(payload);
    return 0;
  } else if(numCharsToBuffer > 0){
    memcpy(&payload[closingBrace+1], &buffer[0], numCharsToBuffer);
    bufferPos = numCharsToBuffer;
  } else {
    bufferPos = 0;
  }

  // Make sure the beginning is correct
  if(payload[0] != '['){
    free(payload);
    return 0;
  }

  Serial.println(payload);
  return payload;
}

void TcpReader::readBlocked( char* destination, int length )
{
  int bytesRead = 0;
  int readResult = 0;
  int maxTries = 3;
  int numTries = 0;
  while( bytesRead != length && numTries++ < maxTries)
  {
    readResult = client.read( (uint8_t*) destination + bytesRead, length - bytesRead );
    if(readResult < 0){
      if(client.available() <= 0){
        // The client.available() is responsible for fetching the amount left on buffer
        // Without this call the read will return 0 waiting for more causing this to hang.
        delay(1);
      }
    } else {
      bytesRead += readResult;
    }
  }
}

