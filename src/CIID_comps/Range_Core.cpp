#define PART_NAME "Range_Core_"
#define PART_ID PART_NAME PART_NUM
#define DATA_RATE 200

/*
Noam stuff: search "NOAM:" for places to fill in Noam functionality.
 */
unsigned long dataTimer = millis();

const int range1Pin = A0;
const int range2Pin = A1;
const int range3Pin = A2;

//Noam:
Lemma lemma(PART_ID, ROOM_ID);

void setup(){
  pinMode(D7, OUTPUT);
#if SERIAL_DEBUG
  Serial.begin(9600);
  delay(50);
  Serial.println("Serial initialized.");
#endif
  delay(5);  
  //  NOAM: 
  lemma.begin();
  digitalWrite(D7, 1);
}

void loop(){  
  //NOAM: 
  lemma.run();
  if( millis() - dataTimer > DATA_RATE ){
    dataTimer = millis();

    #define TOPIC_ID "Range1_" PART_NUM
    int _value = analogRead( range1Pin );
    lemma.sendEvent( TOPIC_ID , _value );
    delay(5);
    #if SERIAL_DEBUG
    Serial.print(TOPIC_ID);
    Serial.print(": ");Serial.println(_value);
#endif

//     #define TOPIC_ID "Range2_" PART_NUM
//     _value = analogRead( range2Pin );
//     lemma.sendEvent( TOPIC_ID , _value );
//     delay(5);
//     #if SERIAL_DEBUG
//     Serial.print(TOPIC_ID);
//     Serial.print(": ");Serial.println(_value);
// #endif

//     #define TOPIC_ID "Range3_" PART_NUM
//     _value = analogRead( range3Pin );
//     lemma.sendEvent( TOPIC_ID , _value );
//     delay(5);
//     #if SERIAL_DEBUG
//     Serial.print(TOPIC_ID);
//     Serial.print(": ");Serial.println(_value);
// #endif
  }
}




