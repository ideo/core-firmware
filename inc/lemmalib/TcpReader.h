#ifndef TCP_READER_H
#define TCP_READER_H

class TCPClient;

class TcpReader
{
private:
  TCPClient& client;

public:
  TcpReader( TCPClient& client );

  char* read();

private:
  int readPayloadLength();
  char* readPayload( int length );
  void readBlocked( char* destination, int length );
};

#endif

