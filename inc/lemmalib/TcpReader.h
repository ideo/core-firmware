#ifndef TCP_READER_H
#define TCP_READER_H

#define TCPREADER_EXTRA_BUFFER_LEN 255

class TCPClient;

class TcpReader
{
private:
  TCPClient& client;

public:
  TcpReader( TCPClient& client );

  char buffer[TCPREADER_EXTRA_BUFFER_LEN];
  int bufferPos;
  char* read();

private:
  int readPayloadLength();
  char* readPayload( int length );
  void readBlocked( char* destination, int length );
};

#endif

