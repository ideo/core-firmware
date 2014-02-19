#define PART_NAME "Stepper_Core_"
#define PART_ID PART_NAME PART_NUM
#define TIMER_INTERVAL 1000

/*
Noam stuff: search "NOAM:" for places to fill in Noam functionality.
 */

#include <AccelStepper.h>

//Noam:
Lemma lemma(PART_ID, ROOM_ID);

unsigned long lemmaTimer = millis();
unsigned long stepTimer1 = micros();
unsigned int stepHalfDuration1 = 5000;
unsigned int stepCount1 = 0;

const int dir1Pin = 0;
const int step1Pin = 1;
const int ledPin = D7;
const int ledWaitTime = 100;

int sendNum = 0;

int timeUntilLEDOff = 0;

bool jogMode1 = true;

bool step1State = false;

bool stepper1WasMoving = false;
// bool stepper2WasMoving = false;
unsigned long step1PosTimer = millis();
// unsigned long step2PosTimer = millis();

// NOAM:

void handleSteps( 
  unsigned long stepTimer, unsigned int stepHalfDuration, 
  long stepCount, const int stepPin, bool stepState, bool jogMode 
  ){
  if( jogMode1 && (stepCount1 <= 0) ) return;
  if( micros() - stepTimer1 > stepHalfDuration1 ){
    stepTimer1 = micros();
    step1State = !step1State;
    digitalWrite(step1Pin , step1State);
    if(step1State && jogMode1) stepCount1--;
  }
}
//Motor 1 handlers
void move1Handler(Event const & e){
  #if SERIAL_DEBUG
  Serial.print(e.name);Serial.print("  "); Serial.println(e.longValue);
  #endif
  jogMode1 = true;  
  if( e.longValue < 0 ){
    digitalWrite(dir1Pin , LOW);  
  }
  else{
    digitalWrite(dir1Pin , HIGH);
  }
  stepCount1 = abs(e.longValue);
  digitalWrite(ledPin, 1);
  timeUntilLEDOff = ledWaitTime;
  lemma.sendEvent("Fired", sendNum++);
}

void setSpeed1Handler(Event const & e){
#if SERIAL_DEBUG
  Serial.print(e.name);Serial.print("  "); Serial.println(e.floatValue);
  #endif  
  double halfPeriod = (double)e.floatValue;
  halfPeriod = 1/halfPeriod;
  halfPeriod = 500000 * halfPeriod;
  halfPeriod = (long)halfPeriod;
  halfPeriod = abs(halfPeriod);
  stepHalfDuration1 = halfPeriod;
  #if SERIAL_DEBUG
  Serial.print("half period us: "); Serial.println(halfPeriod);
  #endif
  digitalWrite(ledPin, 1);
  timeUntilLEDOff = ledWaitTime;
}
void stopMotor1Handler(Event const & e){
  #if SERIAL_DEBUG
  Serial.println(e.name);
  #endif   
  jogMode1 = true;
  stepCount1 = 0;
  digitalWrite(ledPin, 1);
  timeUntilLEDOff = ledWaitTime;
}

void setup()
{
pinMode(step1Pin , OUTPUT);
pinMode(dir1Pin , OUTPUT);
pinMode(ledPin, OUTPUT);
#if SERIAL_DEBUG
  Serial.begin(9600);
  delay(50);  
  Serial.println("Serial initialized.");
#endif
  //  NOAM:   

  //Motor 1 listeners
#define HEAR_STRING "move1_" PART_NUM
  lemma.hear( HEAR_STRING , move1Handler );
#define HEAR_STRING "setSpeed1_" PART_NUM
  lemma.hear( HEAR_STRING , setSpeed1Handler );
#define HEAR_STRING "stopMotor1_" PART_NUM
  lemma.hear( HEAR_STRING , stopMotor1Handler );

  lemma.begin();
}

void loop()
{
  //NOAM:
  if( millis() - lemmaTimer > 25 ){
    lemmaTimer = millis();
    lemma.run();
  } 

  if(timeUntilLEDOff-- > 0){
    if(timeUntilLEDOff == 0){
      digitalWrite(ledPin, 0);
    }
  }


  //Stepper 1 Handling
 // if( jogMode1 ){
  handleSteps( stepTimer1, stepHalfDuration1, stepCount1, step1Pin, step1State, jogMode1 );
}

