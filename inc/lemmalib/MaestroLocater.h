#ifndef MAESTRO_LOCATER_H
#define MAESTRO_LOCATER_H

class UDP;

class MaestroLocater
{
private:
  UDP& udpClient;
  char ip[16];
  const char * lemmaId;
  const char * roomName;
  uint16_t port;

public:
  MaestroLocater( UDP& udpClient, const char * lemmaId, const char * desiredRoomName );

  void reset();
  void tryLocate();
  bool wasLocated();
  char const * maestroIp();
  uint16_t maestroPort();

  bool restartingUDP;

private:
  int lastBroadcastMillis;
  bool lastCharacterIsBracket( char const * string );
  void sendBroadcast();

};

#endif

