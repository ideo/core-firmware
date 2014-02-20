#ifndef MESSAGE_BUILDER_H
#define MESSAGE_BUILDER_H

class MessageBuilder
{
  private:
  char const * spallaId;

  public:
  MessageBuilder( char const * spallaId );
  char * buildRegister( int port, char const** hears, int hearsSize, char const** plays, int playsSize ) const;
  char * buildHeartbeat() const;
};


#ifdef __cplusplus
extern "C"
{
#endif

/* C */
char * buildEvent( char const * name, int type, void const * value );
char * buildArrayEvent( char const * name, void * array, int size, int elemType );

#ifdef __cplusplus
}
#endif


#endif

