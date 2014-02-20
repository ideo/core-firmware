#define PART_NAME "PhotoItr_Core_"
#define PART_ID PART_NAME PART_NUM
#define DEBOUNCE 25

#include "Bounce.h"

/*
Noam stuff: search "NOAM:" for places to fill in Noam functionality.
 */

const int itr1Pin = 3;
const int itr2Pin = 4;
const int itr3Pin = 5;
const int itr4Pin = 6;

Bounce itr1( itr1Pin , DEBOUNCE );
Bounce itr2( itr2Pin , DEBOUNCE );
Bounce itr3( itr3Pin , DEBOUNCE );
Bounce itr4( itr4Pin , DEBOUNCE );

//Noam:
Lemma lemma(PART_ID, ROOM_ID);

void pokeItr1Handler(Event const & e);
void pokeItr2Handler(Event const & e);
void pokeItr3Handler(Event const & e);
void pokeItr4Handler(Event const & e);
void sendItr1(void);
void sendItr2(void);
void sendItr3(void);
void sendItr4(void);

//NOAM:
void pokeItr1Handler(Event const & e){
 #if SERIAL_DEBUG
 Serial.println("received poke 1");
 #endif
 sendItr1();
}

void pokeItr2Handler(Event const & e){
 #if SERIAL_DEBUG
 Serial.println("received poke 2");
 #endif
 sendItr2();
}

void pokeItr3Handler(Event const & e){
 #if SERIAL_DEBUG
 Serial.println("received poke 3");
 #endif
 sendItr3();
}

void pokeItr4Handler(Event const & e){
 #if SERIAL_DEBUG
 Serial.println("received poke 4");
 #endif
 sendItr4();
}

void sendItr1(){
#define speakString "Itr1State_" PART_NUM
#if SERIAL_DEBUG
  Serial.print("Itr 1: ");
  Serial.println( (bool)!itr1.read() );
#endif
  //NOAM:
  lemma.sendEvent( speakString , (bool)!itr1.read() );  
}

void sendItr2(){
#define speakString "Itr2State_" PART_NUM
#if SERIAL_DEBUG
  Serial.print("Itr 2: ");
  Serial.println( (bool)!itr2.read() );
#endif
  //NOAM:
   lemma.sendEvent( speakString , (bool)!itr2.read() );  
}

void sendItr3(){
#define speakString "Itr3State_" PART_NUM
#if SERIAL_DEBUG
  Serial.print("Itr 3: ");
  Serial.println( (bool)!itr3.read() );
#endif
  //NOAM:
   lemma.sendEvent( speakString , (bool)!itr3.read() );  
}

void sendItr4(){
#define speakString "Itr4State_" PART_NUM
#if SERIAL_DEBUG
  Serial.print("Itr 4: ");
  Serial.println( (bool)!itr4.read() );
#endif
  //NOAM:
   lemma.sendEvent( speakString , (bool)!itr4.read() );  
}

void setup(){
#if SERIAL_DEBUG
  Serial.begin(9600);
  delay(50);
  Serial.println("Serial initialized.");
#endif
  delay(5);  

  pinMode( itr1Pin , INPUT_PULLUP );
  pinMode( itr2Pin , INPUT_PULLUP );
  pinMode( itr3Pin , INPUT_PULLUP );
  pinMode( itr4Pin , INPUT_PULLUP );

  itr1.update();
  itr2.update();
  itr3.update();
  itr4.update();

  sendItr1();
  sendItr2();
  sendItr3();
  sendItr4();

#define hearString "Itr1Poke_" PART_NUM
  //  NOAM: 
     lemma.hear( hearString , pokeItr1Handler );
#define hearString "Itr2Poke_" PART_NUM
  //  NOAM: 
     lemma.hear( hearString , pokeItr2Handler );
#define hearString "Itr3Poke_" PART_NUM
  //  NOAM: 
     lemma.hear( hearString , pokeItr3Handler );
#define hearString "Itr4Poke_" PART_NUM
  //  NOAM: 
     lemma.hear( hearString , pokeItr4Handler );

  //  NOAM: 
   lemma.begin();
}

void loop(){  
  //NOAM: 
  lemma.run();
  itr1.update();
  itr2.update();
  itr3.update();
  itr4.update();

  //Itr 1 handling
#define TOPIC_ID "Itr1State_" PART_NUM
  if(itr1.fallingEdge() ){
#if SERIAL_DEBUG
    Serial.print(TOPIC_ID);
    Serial.println(": true");
#endif
    //NOAM:
    lemma.sendEvent( TOPIC_ID , true );
  }
  else if( itr1.risingEdge() ){
#if SERIAL_DEBUG
    Serial.print(TOPIC_ID);
    Serial.println(": false");
#endif
    //NOAM:
    lemma.sendEvent( TOPIC_ID , false );
  }
  
    //Itr 2 handling
#define TOPIC_ID "Itr2State_" PART_NUM
  if(itr2.fallingEdge() ){
#if SERIAL_DEBUG
    Serial.print(TOPIC_ID);
    Serial.println(": true");
#endif
    //NOAM:
    lemma.sendEvent( TOPIC_ID , true );
  }
  else if( itr2.risingEdge() ){
#if SERIAL_DEBUG
    Serial.print(TOPIC_ID);
    Serial.println(": false");
#endif
    //NOAM:
    lemma.sendEvent( TOPIC_ID , false );
  }
  
      //Itr 3 handling
#define TOPIC_ID "Itr3State_" PART_NUM
  if(itr3.fallingEdge() ){
#if SERIAL_DEBUG
    Serial.print(TOPIC_ID);
    Serial.println(": true");
#endif
    //NOAM:
    lemma.sendEvent( TOPIC_ID , true );
  }
  else if( itr3.risingEdge() ){
#if SERIAL_DEBUG
    Serial.print(TOPIC_ID);
    Serial.println(": false");
#endif
    //NOAM:
    lemma.sendEvent( TOPIC_ID , false );
  }
  
        //Itr 4 handling
#define TOPIC_ID "Itr4State_" PART_NUM
  if(itr4.fallingEdge() ){
#if SERIAL_DEBUG
    Serial.print(TOPIC_ID);
    Serial.println(": true");
#endif
    //NOAM:
    lemma.sendEvent( TOPIC_ID , true );
  }
  else if( itr4.risingEdge() ){
#if SERIAL_DEBUG
    Serial.print(TOPIC_ID);
    Serial.println(": false");
#endif
    //NOAM:
    lemma.sendEvent( TOPIC_ID , false );
  }
}




