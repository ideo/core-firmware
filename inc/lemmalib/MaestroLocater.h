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

  void begin();
  void tryLocate();
  bool wasLocated();
  char const * maestroIp();
  IPAddress maestroIpAddress();
  uint16_t maestroPort();

  bool restartingUDP;

private:
  unsigned long lastBroadcastMillis;
  void sendBroadcast();
  bool locating;
  IPAddress address;

};

#endif

