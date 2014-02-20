#define PART_NAME "Buttons_Core_"
#define PART_ID PART_NAME PART_NUM
#define DEBOUNCE 50

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

void setup(){
  DEBUG_BEGIN();
  DEBUG_LED_CONFIG();
  
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
  lemma.begin();  
}

void sendButtonEvent(const char* topic_pressed, const char * topic_released, Bounce button){
  if(button.fallingEdge()){
    DEBUGLN(topic_pressed)
    DEBUG_LED_ON(100);
    //NOAM:
    int numTry = 0;
    while(!lemma.sendEvent( topic_pressed , true ) && numTry++ < 3){
      delay(1);
    }
  }

  if(button.risingEdge()){
    DEBUGLN(topic_released);
    DEBUG_LED_ON(100);
    //NOAM:
    int numTry = 0;
    while(!lemma.sendEvent( topic_released , true ) && numTry++ < 3){
      delay(1);
    }
  }
}

void loop(){
  DEBUG_LED_ITER();

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

  sendButtonEvent("B1Pressed_" PART_NUM, "B1Released_" PART_NUM, button1);
  sendButtonEvent("B2Pressed_" PART_NUM, "B2Released_" PART_NUM, button2);
  sendButtonEvent("B3Pressed_" PART_NUM, "B3Released_" PART_NUM, button3);
  sendButtonEvent("B4Pressed_" PART_NUM, "B4Released_" PART_NUM, button4);
  sendButtonEvent("B5Pressed_" PART_NUM, "B5Released_" PART_NUM, button5);
  sendButtonEvent("B6Pressed_" PART_NUM, "B6Released_" PART_NUM, button6);
  sendButtonEvent("B7Pressed_" PART_NUM, "B7Released_" PART_NUM, button7);
  sendButtonEvent("B8Pressed_" PART_NUM, "B8Released_" PART_NUM, button8);
  sendButtonEvent("B9Pressed_" PART_NUM, "B9Released_" PART_NUM, button9);
  sendButtonEvent("B10Pressed_" PART_NUM, "B10Released_" PART_NUM, button10);
  
}
