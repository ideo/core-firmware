#include "TcpReader.h"
#include "spark_wiring_tcpclient.h"
#include "spark_wiring_usbserial.h"


TcpReader::TcpReader( TCPClient& client ) :
    client( client )
{ }

char* TcpReader::read()
{
  int length = readPayloadLength();

  char *ret = readPayload( length );
  if ('0' == ret[0] &&
      '[' == ret[6] &&
      ']' == ret[strlen(ret)-1]) 
  {
    char *ptr = strstr(ret, "\",\"");
    if (ptr && (ptr-ret) < strlen(ret)-3) 
    {
      /* attempt to patch up the corrupted message, the first 2 fields, "event" and "<sender name>"
       * are not needed by callbacks, construct a message with dummy strings for these 2 fields
       * while preserving the original event name and event value
       */
       ptr += 2;
       char *dummy = "[\"event\",\"?\",";
       int size = strlen(dummy) + strlen(ptr) + 1;
       char *patch = (char *)malloc(size * sizeof(char));
       memset(patch, 0, size);
       memcpy(patch, dummy, strlen(dummy));
       memcpy(patch + strlen(dummy), ptr, strlen(ptr));
       free(ret);
       ret = patch;
       // Serial.print("patched message: ");
       // Serial.println(ret);
    }
    else 
    {
      free(ret);
      ret = NULL;
    }
  }

  return ret;
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

