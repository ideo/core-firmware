#define PART_NAME "PIR_Core_"
#define PART_ID PART_NAME PART_NUM
#define DEBOUNCE 50
#define PIR_INTERVAL

#include "Bounce.h"
const int pir1Pin = D4;
const int pir2Pin = D5;
const int pir3Pin = D6;

Bounce pir1( pir1Pin , DEBOUNCE );
Bounce pir2( pir2Pin , DEBOUNCE );
Bounce pir3( pir3Pin , DEBOUNCE );

// Initialize the lemma with the part and room names
Lemma lemma(PART_ID, ROOM_ID);

void setup(){
  #if SERIAL_DEBUG
  Serial.begin(9600);
  delay(25);
  Serial.println("PIR serial initialized");
#endif

  pinMode( pir1Pin , INPUT_PULLUP );
  pinMode( pir2Pin , INPUT_PULLUP );
  pinMode( pir3Pin , INPUT_PULLUP );

  //  NOAM:   
  lemma.begin();  
}

void loop(){  
  
  //NOAM: 
  lemma.run();
  //

  pir1.update();
  pir2.update();
  pir3.update();

  //pir 1 handling
  if(pir1.fallingEdge()){
#define TOPIC_ID "PIR1Alarm_" PART_NUM
#if SERIAL_DEBUG
    Serial.println(TOPIC_ID);
#endif
    //NOAM:
    lemma.sendEvent( TOPIC_ID , true );
    //
  }
  if(pir1.risingEdge()){
#define TOPIC_ID "PIR1Alarm_" PART_NUM
#if SERIAL_DEBUG
    Serial.println(TOPIC_ID);
#endif
    //NOAM:
    lemma.sendEvent( TOPIC_ID , false );
    //
  }
  
    //pir 2 handling
  if(pir2.fallingEdge()){
#define TOPIC_ID "PIR2Alarm_" PART_NUM
#if SERIAL_DEBUG
    Serial.println(TOPIC_ID);
#endif
    //NOAM:
    lemma.sendEvent( TOPIC_ID , true );
    //
  }
  if(pir2.risingEdge()){
#define TOPIC_ID "PIR2Alarm_" PART_NUM
#if SERIAL_DEBUG
    Serial.println(TOPIC_ID);
#endif
    //NOAM:
    lemma.sendEvent( TOPIC_ID , false );   
    //    
  }
  
    //pir 3 handling
  if(pir3.fallingEdge()){
#define TOPIC_ID "PIR3Alarm_" PART_NUM
#if SERIAL_DEBUG
    Serial.println(TOPIC_ID);
#endif
    //NOAM:
    lemma.sendEvent( TOPIC_ID , true );
    //
  }
  if(pir3.risingEdge()){
#define TOPIC_ID "PIR3Alarm_" PART_NUM
#if SERIAL_DEBUG
    Serial.println(TOPIC_ID);
#endif
    //NOAM:
    lemma.sendEvent( TOPIC_ID , false );
    //
  }
}
