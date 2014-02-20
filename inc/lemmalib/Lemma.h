#ifndef LEMMA_H
#define LEMMA_H

#include "spark_wiring_tcpclient.h"
#include "spark_wiring_udp.h"
#include "spark_wiring_tcpserver.h"
#include "MessageBuilder.h"
#include "MessageSender.h"
#include "MaestroLocater.h"
#include "EventFilter.h"
#include "Event.h"

class Lemma
{
  public:
    Lemma( const char * id, const char * desiredRoomName );

    void begin( );
    void run();
    void hear( char const * name, handler_t callback );
    bool sendEvent( char const * name, char const * value );
    bool sendEvent( char const * name, int value );
    bool sendEvent( char const * name, unsigned long value );
    bool sendEvent( char const * name, long value );
    bool sendEvent( char const * name, double value );
    bool sendEvent( char const * name, bool value );
    bool sendIntArray( char const * name, int * array, int size );
    bool sendDoubleArray( char const * name, double * array, int size );
    bool sendStringArray( char const * name, char ** array, int size );
    bool isConnected();

  private:
    void displayIP();
    void tryConnectingWithMaestro();
    void handleIncomingConnections();    
    void testHeartbeat();
    void testConnection();
    void reset();

    static const uint16_t LISTEN_PORT = 9933;

    bool _connectedToHost;
    unsigned long heartbeatTimer;
    TCPServer server;
    TCPClient maestroConnection;
    UDP udpClient;

    EventFilter filter;
    MessageBuilder messageBuilder;
    MessageSender messageSender;
    MaestroLocater maestroLocater;
};

#endif

