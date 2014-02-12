#ifndef MAESTRO_LOCATER_H
#define MAESTRO_LOCATER_H

class UDP;

class MaestroLocater
{
private:
  UDP& udpClient;
  char ip[16];
  uint16_t port;

public:
  MaestroLocater( UDP& udpClient );

  void reset();
  void tryLocate();
  bool wasLocated();
  char const * maestroIp();
  uint16_t maestroPort();

  bool restartingUDP;

private:
  bool lastCharacterIsBracket( char const * string );
};

#endif

