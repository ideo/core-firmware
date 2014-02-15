#ifndef BUTTONS_CORE
#define BUTTONS_CORE

#define ROOM_ID "evan test room"
#define PART_NAME "Buttons_Core_"
#define PART_NUM "2"
#define PART_ID PART_NAME PART_NUM
#define DEBOUNCE 50
#define SERIAL_DEBUG true

#include "Bounce.h"
const int button1Pin = A0;
const int button2Pin = A1;
const int button3Pin = A2;
const int button4Pin = A3;
const int button5Pin = A4;
const int button6Pin = A5;
const int button7Pin = 2;
const int button8Pin = 3;
const int button9Pin = 4;
const int button10Pin = 5;

Bounce button1( button1Pin , DEBOUNCE );
Bounce button2( button2Pin , DEBOUNCE );
Bounce button3( button3Pin , DEBOUNCE );
Bounce button4( button4Pin , DEBOUNCE );
Bounce button5( button5Pin , DEBOUNCE );
Bounce button6( button6Pin , DEBOUNCE );
Bounce button7( button7Pin , DEBOUNCE );
Bounce button8( button8Pin , DEBOUNCE );
Bounce button9( button9Pin , DEBOUNCE );
Bounce button10( button10Pin , DEBOUNCE );

// Initialize the lemma with the part and room names
Lemma lemma(PART_ID, ROOM_ID);

void hearTestCB(const Event& event){
  Serial.print("Heard event: ");
  Serial.println(event.stringValue);
}


void setup(){
  pinMode( button1Pin , INPUT_PULLUP );
  pinMode( button2Pin , INPUT_PULLUP );
  pinMode( button3Pin , INPUT_PULLUP );
  pinMode( button4Pin , INPUT_PULLUP );
  pinMode( button5Pin , INPUT_PULLUP );
  pinMode( button6Pin , INPUT_PULLUP );
  pinMode( button7Pin , INPUT_PULLUP );
  pinMode( button8Pin , INPUT_PULLUP );
  pinMode( button9Pin , INPUT_PULLUP );
  pinMode( button10Pin , INPUT_PULLUP );

  //  NOAM: 
  lemma.hear("hearTest", hearTestCB);
  lemma.begin();
  
#if SERIAL_DEBUG
  Serial.begin(9600);
  delay(25);
  Serial.println("initialized");
#endif
}

void loop(){  
  
  //NOAM: 
  lemma.run();
  //

  button1.update();
  button2.update();
  button3.update();
  button4.update();
  button5.update();
  button6.update();
  button7.update();
  button8.update();
  button9.update();
  button10.update();

  //Button 1 handling
  if(button1.fallingEdge()){
#define TOPIC_ID "B1Pressed_" PART_NUM
#if SERIAL_DEBUG
    Serial.println(TOPIC_ID);
#endif
    //NOAM:
    lemma.sendEvent( TOPIC_ID , true );
    //
  }
  if(button1.risingEdge()){
#define TOPIC_ID "B1Released_" PART_NUM
#if SERIAL_DEBUG
    Serial.println(TOPIC_ID);
#endif
    //NOAM:
    lemma.sendEvent( TOPIC_ID , true );
    //
  }
  
    //Button 2 handling
  if(button2.fallingEdge()){
#define TOPIC_ID "B2Pressed_" PART_NUM
#if SERIAL_DEBUG
    Serial.println(TOPIC_ID);
#endif
    //NOAM:
    lemma.sendEvent( TOPIC_ID , true );
    //
  }
  if(button2.risingEdge()){
#define TOPIC_ID "B2Released_" PART_NUM
#if SERIAL_DEBUG
    Serial.println(TOPIC_ID);
#endif
    //NOAM:
    lemma.sendEvent( TOPIC_ID , true );   
    //    
  }
  
    //Button 3 handling
  if(button3.fallingEdge()){
#define TOPIC_ID "B3Pressed_" PART_NUM
#if SERIAL_DEBUG
    Serial.println(TOPIC_ID);
#endif
    //NOAM:
    lemma.sendEvent( TOPIC_ID , true );
    //
  }
  if(button3.risingEdge()){
#define TOPIC_ID "B3Released_" PART_NUM
#if SERIAL_DEBUG
    Serial.println(TOPIC_ID);
#endif
    //NOAM:
    lemma.sendEvent( TOPIC_ID , true );
    //
  }
  
    //Button 4 handling
  if(button4.fallingEdge()){
#define TOPIC_ID "B4Pressed_" PART_NUM
#if SERIAL_DEBUG
    Serial.println(TOPIC_ID);
#endif
    //NOAM:
    lemma.sendEvent( TOPIC_ID , true );
    //
  }
  if(button4.risingEdge()){
#define TOPIC_ID "B4Released_" PART_NUM
#if SERIAL_DEBUG
    Serial.println(TOPIC_ID);
#endif
    //NOAM:
    lemma.sendEvent( TOPIC_ID , true );
    //
  }
  
    //Button 5 handling
  if(button5.fallingEdge()){
#define TOPIC_ID "B5Pressed_" PART_NUM
#if SERIAL_DEBUG
    Serial.println(TOPIC_ID);
#endif
    //NOAM:
    lemma.sendEvent( TOPIC_ID , true );
    //
  }
  if(button5.risingEdge()){
#define TOPIC_ID "B5Released_" PART_NUM
#if SERIAL_DEBUG
    Serial.println(TOPIC_ID);
#endif
    //NOAM:
    lemma.sendEvent( TOPIC_ID , true );
    //
  }
  
    //Button 6 handling
  if(button6.fallingEdge()){
#define TOPIC_ID "B6Pressed_" PART_NUM
#if SERIAL_DEBUG
    Serial.println(TOPIC_ID);
#endif
    //NOAM:
    lemma.sendEvent( TOPIC_ID , true );
    //
  }
  if(button6.risingEdge()){
#define TOPIC_ID "B6Released_" PART_NUM
#if SERIAL_DEBUG
    Serial.println(TOPIC_ID);
#endif
    //NOAM:
    lemma.sendEvent( TOPIC_ID , true );
    //
  }
  
    //Button 7 handling
  if(button7.fallingEdge()){
#define TOPIC_ID "B7Pressed_" PART_NUM
#if SERIAL_DEBUG
    Serial.println(TOPIC_ID);
#endif
    //NOAM:
    lemma.sendEvent( TOPIC_ID , true );
    //
  }
  if(button7.risingEdge()){
#define TOPIC_ID "B7Released_" PART_NUM
#if SERIAL_DEBUG
    Serial.println(TOPIC_ID);
#endif
    //NOAM:
    lemma.sendEvent( TOPIC_ID , true );
    //
  }
  
    //Button 8 handling
  if(button8.fallingEdge()){
#define TOPIC_ID "B8Pressed_" PART_NUM
#if SERIAL_DEBUG
    Serial.println(TOPIC_ID);
#endif
    //NOAM:
    lemma.sendEvent( TOPIC_ID , true );
    //    
  }
  if(button8.risingEdge()){
#define TOPIC_ID "B8Released_" PART_NUM
#if SERIAL_DEBUG
    Serial.println(TOPIC_ID);
#endif
    //NOAM:
    lemma.sendEvent( TOPIC_ID , true );
    //
  }
  
    //Button 9 handling
  if(button9.fallingEdge()){
#define TOPIC_ID "B9Pressed_" PART_NUM
#if SERIAL_DEBUG
    Serial.println(TOPIC_ID);
#endif
    //NOAM:
    lemma.sendEvent( TOPIC_ID , true );
    //
  }
  if(button9.risingEdge()){
#define TOPIC_ID "B9Released_" PART_NUM
#if SERIAL_DEBUG
    Serial.println(TOPIC_ID);
#endif
    //NOAM:
    lemma.sendEvent( TOPIC_ID , true );
    //
  }
  
    //Button 10 handling
  if(button10.fallingEdge()){
#define TOPIC_ID "B10Pressed_" PART_NUM
#if SERIAL_DEBUG
    Serial.println(TOPIC_ID);
#endif
    //NOAM:
    lemma.sendEvent( TOPIC_ID , true );
    //
  }
  if(button10.risingEdge()){
#define TOPIC_ID "B10Released_" PART_NUM
#if SERIAL_DEBUG
    Serial.println(TOPIC_ID);
#endif
    //NOAM:
    lemma.sendEvent( TOPIC_ID , true ); 
    //
  }
}

#endif