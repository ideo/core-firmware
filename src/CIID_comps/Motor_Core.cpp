#define PART_NAME "Motor_Core_"
#define PART_ID PART_NAME PART_NUM
#define TIMER_INTERVAL 1000

/*
Noam stuff: search "NOAM:" for places to fill in Noam functionality.
 */

//Noam:
Lemma lemma(PART_ID, ROOM_ID);

const int mot1Pin = A0;
const int mot2Pin = A1;
const int mot3Pin = A4;
const int mot4Pin = A5;

void mot1Handler(Event const & e);
void mot2Handler(Event const & e);
void mot3Handler(Event const & e);
void mot4Handler(Event const & e);

void setup()
{
pinMode(mot1Pin , OUTPUT);
pinMode(mot2Pin , OUTPUT);
pinMode(mot3Pin , OUTPUT);
pinMode(mot4Pin , OUTPUT);
pinMode(D0 , OUTPUT);
digitalWrite(D0,HIGH);

#if SERIAL_DEBUG
  Serial.begin(9600);
  delay(50);  
  Serial.println("Serial initialized.");
#endif
  //  NOAM:   
  analogWrite( mot1Pin , 0);
  //Motor 1 listeners
#define HEAR_STRING "mot1_" PART_NUM
  lemma.hear( HEAR_STRING , mot1Handler );
#define HEAR_STRING "mot2_" PART_NUM
  lemma.hear( HEAR_STRING , mot2Handler );
#define HEAR_STRING "mot3_" PART_NUM
  lemma.hear( HEAR_STRING , mot3Handler );
#define HEAR_STRING "mot4_" PART_NUM
  lemma.hear( HEAR_STRING , mot4Handler );

  lemma.begin();
}

void loop()
{    
  //NOAM:
  lemma.run();
}

//Motor 1 handlers
void mot1Handler(Event const & e){
  #if SERIAL_DEBUG
  Serial.print(e.name);Serial.print("  "); Serial.println(e.intValue);
  #endif
  byte pwmVal = 0;
  if( e.intValue > 255 ) pwmVal = 255;
  else if( e.intValue < 0 ) pwmVal = 0;
  else pwmVal = e.intValue;
  analogWrite( mot1Pin , pwmVal );
}

//Motor 2 handlers
void mot2Handler(Event const & e){
  #if SERIAL_DEBUG
  Serial.print(e.name);Serial.print("  "); Serial.println(e.intValue);
  #endif
  byte pwmVal = 0;
  if( e.intValue > 255 ) pwmVal = 255;
  else if( e.intValue < 0 ) pwmVal = 0;
  else pwmVal = e.intValue;
  analogWrite( mot2Pin , pwmVal );
}

//Motor 3 handlers
void mot3Handler(Event const & e){
  #if SERIAL_DEBUG
  Serial.print(e.name);Serial.print("  "); Serial.println(e.intValue);
  #endif
  byte pwmVal = 0;
  if( e.intValue > 255 ) pwmVal = 255;
  else if( e.intValue < 0 ) pwmVal = 0;
  else pwmVal = e.intValue;
  analogWrite( mot3Pin , pwmVal );
}

//Motor 4 handlers
void mot4Handler(Event const & e){
  #if SERIAL_DEBUG
  Serial.print(e.name);Serial.print("  "); Serial.println(e.intValue);
  #endif
  byte pwmVal = 0;
  if( e.intValue > 255 ) pwmVal = 255;
  else if( e.intValue < 0 ) pwmVal = 0;
  else pwmVal = e.intValue;
  analogWrite( mot4Pin , pwmVal );
}