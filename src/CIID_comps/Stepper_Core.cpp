#define PART_NAME "Stepper_Core_"
#define PART_NUM "1"
#define PART_ID PART_NAME PART_NUM
#define TIMER_INTERVAL 200

/*
Noam stuff: search "NOAM:" for places to fill in Noam functionality.
 */

#include <AccelStepper.h>

//Noam:
Lemma lemma(PART_ID, ROOM_ID);

bool jogMode1 = true;
bool jogMode2 = true;
bool jogMode3 = true;
bool stepper1WasMoving = false;
bool stepper2WasMoving = false;
bool stepper3WasMoving = false;
unsigned long step1PosTimer = millis();
unsigned long step2PosTimer = millis();
unsigned long step3PosTimer = millis();
AccelStepper stepper1(1,1,0); // stepperDriver => step: D1, dir: D0
AccelStepper stepper2(1,3,2); // stepperDriver => step: D3, dir: D2
AccelStepper stepper3(1,5,4); // stepperDriver => step: D5, dir: D4

// NOAM:

//Motor 1 handlers
void moveRelative1Handler(Event const & e){
 jogMode1 = true;  
 stepper1.move(e.longValue);
}
void moveAbsolute1Handler(Event const & e){
  jogMode1 = true;
  stepper1.move(e.longValue);  
}
void setConstSpeed1Handler(Event const & e){
 jogMode1 = false;
 stepper1.setSpeed(e.floatValue);
}
void setMaxSpeed1Handler(Event const & e){   
 stepper1.setMaxSpeed(e.floatValue);
}
void setAcceleration1Handler(Event const & e){   
 stepper1.setAcceleration(e.floatValue);
}
void setZero1Handler(Event const & e){
 stepper1.setCurrentPosition(0);
}
void stopMotor1Handler(Event const & e){
  jogMode1 = true;
  stepper1.stop();
}

//Motor 2 handlers
void moveRelative2Handler(Event const & e){
 jogMode2 = true;  
 stepper2.move(e.longValue);
}
void moveAbsolute2Handler(Event const & e){
  jogMode2 = true;
  stepper2.move(e.longValue);  
}
void setConstSpeed2Handler(Event const & e){
 jogMode2 = false;
 stepper2.setSpeed(e.floatValue);
}
void setMaxSpeed2Handler(Event const & e){   
 stepper2.setMaxSpeed(e.floatValue);
}
void setAcceleration2Handler(Event const & e){   
 stepper2.setAcceleration(e.floatValue);
}
void setZero2Handler(Event const & e){
 stepper2.setCurrentPosition(0);
}
void stopMotor2Handler(Event const & e){
  jogMode2 = true;
  stepper2.stop();
} 

//Motor 3 handlers
void moveRelative3Handler(Event const & e){
 jogMode3 = true;  
 stepper3.move(e.longValue);
}
void moveAbsolute3Handler(Event const & e){
  jogMode3 = true;
  stepper3.move(e.longValue);  
}
void setConstSpeed3Handler(Event const & e){
 jogMode3 = false;
 stepper3.setSpeed(e.floatValue);
}
void setMaxSpeed3Handler(Event const & e){   
 stepper3.setMaxSpeed(e.floatValue);
}
void setAcceleration3Handler(Event const & e){   
 stepper3.setAcceleration(e.floatValue);
}
void setZero3Handler(Event const & e){
 stepper3.setCurrentPosition(0);
}
void stopMotor3Handler(Event const & e){
  jogMode3 = true;
  stepper3.stop();
} 

void setup()
{
  Serial.begin(57600);
  delay(50);  
  //  NOAM: 

  //Motor 1 listeners
#define HEAR_STRING "moveRelative1_ " PART_NUM
  lemma.hear( HEAR_STRING , moveRelative1Handler );
#define HEAR_STRING "moveAbsolute1_ " PART_NUM
  lemma.hear( HEAR_STRING , moveAbsolute1Handler );
#define HEAR_STRING "setConstSpeed1_ " PART_NUM
  lemma.hear( HEAR_STRING , setConstSpeed1Handler );
#define HEAR_STRING "setMaxSpeed1_ " PART_NUM
  lemma.hear( HEAR_STRING , setMaxSpeed1Handler );
#define HEAR_STRING "setAcceleration1_ " PART_NUM
  lemma.hear( HEAR_STRING , setAcceleration1Handler );
#define HEAR_STRING "setZero1_ " PART_NUM
  lemma.hear( HEAR_STRING , setZero1Handler );
#define HEAR_STRING "stopMotor1_ " PART_NUM
  lemma.hear( HEAR_STRING , stopMotor1Handler );

//Motor 2 listeners
#define HEAR_STRING "moveRelative2_ " PART_NUM
  lemma.hear( HEAR_STRING , moveRelative2Handler );
#define HEAR_STRING "moveAbsolute2_ " PART_NUM
  lemma.hear( HEAR_STRING , moveAbsolute2Handler );
#define HEAR_STRING "setConstSpeed2_ " PART_NUM
  lemma.hear( HEAR_STRING , setConstSpeed2Handler );
#define HEAR_STRING "setMaxSpeed2_ " PART_NUM
  lemma.hear( HEAR_STRING , setMaxSpeed2Handler );
#define HEAR_STRING "setAcceleration2_ " PART_NUM
  lemma.hear( HEAR_STRING , setAcceleration2Handler );
#define HEAR_STRING "setZero2_ " PART_NUM
  lemma.hear( HEAR_STRING , setZero2Handler );
#define HEAR_STRING "stopMotor2_ " PART_NUM
  lemma.hear( HEAR_STRING , stopMotor2Handler );

  //Motor 3 listeners
#define HEAR_STRING "moveRelative3_ " PART_NUM
  lemma.hear( HEAR_STRING , moveRelative3Handler );
#define HEAR_STRING "moveAbsolute3_ " PART_NUM
  lemma.hear( HEAR_STRING , moveAbsolute3Handler );
#define HEAR_STRING "setConstSpeed3_ " PART_NUM
  lemma.hear( HEAR_STRING , setConstSpeed3Handler );
#define HEAR_STRING "setMaxSpeed3_ " PART_NUM
  lemma.hear( HEAR_STRING , setMaxSpeed3Handler );
#define HEAR_STRING "setAcceleration3_ " PART_NUM
  lemma.hear( HEAR_STRING , setAcceleration3Handler );
#define HEAR_STRING "setZero3_ " PART_NUM
  lemma.hear( HEAR_STRING , setZero3Handler );
#define HEAR_STRING "stopMotor3_ " PART_NUM
  lemma.hear( HEAR_STRING , stopMotor3Handler );

  lemma.begin();  // start listening on Noam network 1030    
}

void loop()
{    
  //NOAM: 
 lemma.run();

  //Stepper 1 Handling
 if( jogMode1 ) stepper1.run();
 else stepper1.runSpeed();
 if( stepper1.distanceToGo() != 0 || !jogMode1 ){
  stepper1WasMoving = true;
}
if( millis() - step1PosTimer > TIMER_INTERVAL ){
  step1PosTimer = millis();
  if( stepper1WasMoving ){
#define SPEAK_STRING "MotorPosition1_" PART_NUM
   lemma.sendEvent( SPEAK_STRING , stepper1.currentPosition());
 }
 if( stepper1.distanceToGo() == 0 && stepper1WasMoving ){
  stepper1WasMoving = false;
}
}
}

