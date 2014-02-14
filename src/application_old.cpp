/*

 This Arduino project is a demonstration of the Noam platform's Arduino Lemma Library (Ethernet).
 This project was developed on an Arduino Mega 2560 with Ethernet shield.
 When the Arduino is connected to a Noam network, pushing the buttong will send a signal that is echoed and causes the LED to light up while the button is pressed.
 
 When developing Arduino for Noam, it is important to use an event-centric paradigm.  Using software debounce ensures messages are only sent when the button state changes.
 Keeping delays out of the main loop will ensure fast message response, as new messages are polled in the loop.
 
 Author: Evan Shapiro [ IDEO ]
 
 */

#include "application.h"


#define MSG_PREFIX "  [Arduino] "


//function prototype for polling button status
void periodicalSend();
void buttonMonitor();
//function prototype for event handler that's called when 'buttonPressed' message is received
void buttonHandler(Event const & e);

static const long BAUD_RATE = 9600L;  // Serial communication baude rate for serial monitor
static byte MAC[] = { 
  0x90, 0xA2, 0xDA, 0x0D, 0xA1, 0x55 };  // Network MAC address must be unique on network
//There is a sticker with a guaranteed unique MAC on your Ethernet Arduino or shield
static char const * lemma_ID = "ARDUINO_LEMMA_ID";  // The Noam network ID, must be unique on Noam network
static const int UDP_port = 1030;

const int buttonPin = 6;  // Input button pin, connect other side of button to GND
const int ledPin =  D7;
int ledValue = 0;

boolean ledState = HIGH;
boolean buttonState = LOW;
boolean lastButtonState = LOW;

const unsigned int DEBOUNCE = 100;
unsigned long lastDebounceTime = millis();
long startTime;


static int intArray[] = {
  1, 
  2, 
  3, 
  4
};


static double doubleArray[] = {
  1.2, 
  2.3, 
  3.4, 
  4.5,
  5.6,
  6.7,
  7.8,
  8.9
};


Lemma lemma(lemma_ID);  // Initialize the Arduino Noam Lemma with ID <lemma_ID>

void setup() {
  Serial.begin( BAUD_RATE );  // start serial communication for serial monitor output

  delay(2000);
  Serial.flush();
  delay(2000);
  Serial.println(MSG_PREFIX"beginning setup");
  
  delay(2000);
  Serial.println(MSG_PREFIX"start configuring lemma ...");

  lemma.hear( "sentFromObject2", buttonHandler );  // Register for Noam 'buttonMessage', call buttonHandler() on new event.
  lemma.begin( MAC , UDP_port);  // start listening on Noam network 1030

  Serial.println(MSG_PREFIX"end configuring lemma");

  pinMode(buttonPin, INPUT);
  digitalWrite(buttonPin, HIGH);  // enable the internal pull-up on the button

  pinMode(ledPin, OUTPUT);
  digitalWrite( ledPin, HIGH);
  
  Serial.println(MSG_PREFIX"end setup");

  /* for periodically sending messages */
  startTime = millis();

  /* disable Spark Cloud */
  // Serial.println(MSG_PREFIX"disable Spark Cloud");
  // Spark.disconnect();
}


void loop() {

  if (millis() - startTime > 100) {

    lemma.run();

    periodicalSend();

    startTime = millis();
  }
}


void periodicalSend() {

    static int messageCounter = 0;
    
    messageCounter++;
    messageCounter %= 60000;
    Serial.print("sendEvent #");
    Serial.println(messageCounter);
    lemma.sendEvent( "buttonMessage" , messageCounter );
    lemma.sendEvent("buttonMessage", messageCounter);
}


// void buttonMonitor() {
//   if( millis() - lastDebounceTime < DEBOUNCE ) return; // If DEBOUNCE time hasn't passed since last state change, exit  
  
//   // Otherwise, compare the state of the button pin to the buttonState bit, and change button state if they differ
//   int buttonRead = digitalRead(buttonPin);
  
//   if (buttonRead != buttonState) {
//     Serial.print(MSG_PREFIX"value changed on buttonRead: "); Serial.println(buttonRead);
//     lastDebounceTime = millis();
//     buttonState = buttonRead;

//     lemma.sendEvent( "buttonMessage" , buttonState );
//     lemma.sendEvent( "buttonMessage" , 1.23456 );
//     lemma.sendEvent( "buttonMessage" , "__TEST_string__" );
//     lemma.sendEvent( "buttonMessage" , HIGH );
//     lemma.sendEvent( "buttonMessage" , buttonPin );
//     lemma.sendIntArray( "buttonMessage" , intArray, 4 );
//     lemma.sendDoubleArray( "buttonMessage" , doubleArray, 8 );
//     lemma.sendStringArray( "buttonMessage" , stringArray, 3 );
//   }
// }

// This callback is called when a new Noam 'buttonPressed' message is detected.
// An Event e is passed to the function - 
    //e has e.intValue, e.floatValue, boolValue and e.stringValue, which are interpreted from then message'JSON list
void buttonHandler(Event const & e)
{
  Serial.print(MSG_PREFIX"buttonHandler() callback: received event (name:"); Serial.print( e.name ); Serial.println("): ");

  /*
  if (0 != e.isArray) {
    int i;
    switch (e.arrayElemType) {
        case nJSON_INT:
          Serial.print(MSG_PREFIX"int array of size "); Serial.print(e.isArray); Serial.println(":");
          for (i = 0; i < e.isArray; i++) {
            Serial.print(MSG_PREFIX);
            Serial.println(((int *)e.array)[i]);
          }
          break;
        case nJSON_FLOAT:
        case nJSON_DOUBLE:
          Serial.print(MSG_PREFIX"double array of size "); Serial.print(e.isArray); Serial.println(":");
          for (i = 0; i < e.isArray; i++) {
            Serial.print(MSG_PREFIX);
            Serial.println(((double *)e.array)[i]);
          }
          break;
        case nJSON_BOOL:
          // do something
          break;
        case nJSON_STRING:
          Serial.print(MSG_PREFIX"string array of size "); Serial.print(e.isArray); Serial.println(":");
          for (i = 0; i < e.isArray; i++) {
            Serial.print(MSG_PREFIX);
            Serial.println(((char **)e.array)[i]);
          }
          break;
        default:
          // do something
          break;
    }
  }
  else {
    Serial.print(MSG_PREFIX"Int value: ");  Serial.println( e.intValue );
    Serial.print(MSG_PREFIX"Float value: ");  Serial.println( e.floatValue );
    Serial.print(MSG_PREFIX"String value: ");  Serial.println( e.stringValue );
    Serial.print(MSG_PREFIX"Boolean value: ");  Serial.println( e.boolValue );
  }
  */

  ledValue = 1 - ledValue;
  digitalWrite( ledPin, ledValue );
}


