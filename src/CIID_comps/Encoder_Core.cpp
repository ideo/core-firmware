#define PART_NAME "Encoder_Core_"
#define PART_ID PART_NAME PART_NUM

const int encPinA1 = 2;
const int encPinB1 = 3;
const int encPinA2 = 4;
const int encPinB2 = 5;

  // Initialize the lemma with the part and room names
Lemma lemma(PART_ID, ROOM_ID);

int8_t read_encoder1();
int8_t read_encoder2();

void setup(){
  #if SERIAL_DEBUG
  Serial.begin(9600);
  delay(25);
  Serial.println("Encoder serial initialized.");
#endif
  /* Setup encoder pins as inputs */
  pinMode(encPinA1, INPUT_PULLUP );
  pinMode(encPinB1, INPUT_PULLUP );
  pinMode(encPinA2, INPUT_PULLUP );
  pinMode(encPinB2, INPUT_PULLUP );
  //  NOAM:   
  lemma.begin();  
}

void loop(){  
  //NOAM: 
  lemma.run();
  //
  static uint8_t counter1 = 0;      //this variable will be changed by encoder input
  int8_t tmpdata1;

  tmpdata1 = read_encoder1();
  if( tmpdata1 ) {
    #if SERIAL_DEBUG
  Serial.print("Counter1 value: ");
  Serial.println(counter1, DEC);
  #endif
  counter1 += tmpdata1;
  #define speakStringUp "enc1_Up_" PART_NUM
  #define speakStringDown "enc1_Down_" PART_NUM
  if(tmpdata1 > 0){
    lemma.sendEvent(speakStringUp , 1 );
  }
  else if(tmpdata1 < 0 ){
    lemma.sendEvent(speakStringDown , 1);
  }
}
  
  static uint8_t counter2 = 0;      //this variable will be changed by encoder input
  int8_t tmpdata2;

  tmpdata2 = read_encoder2();
  if( tmpdata2 ) {
    #if SERIAL_DEBUG
    Serial.print("Counter2 value: ");
    Serial.println(counter2, DEC);
    #endif
    counter2 += tmpdata2;
      #define speakStringUp "enc2_Up_" PART_NUM
    #define speakStringDown "enc2_Down_" PART_NUM
    if(tmpdata2 > 0){
      lemma.sendEvent(speakStringUp , 1 );
    }
    else if(tmpdata2 < 0 ){
      lemma.sendEvent(speakStringDown , 1);
    }
  }

  delay(2);
}

/* returns change in encoder state (-1,0,1) */
int8_t read_encoder1(){
static int8_t enc_states[] = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0};
static uint8_t old_AB = 0;
  uint8_t portVal = digitalRead(encPinA1);
  portVal = portVal<<1;
  portVal |= digitalRead(encPinB1);
  old_AB <<= 2;                   //remember previous state
  old_AB |= ( portVal & 0x03 );  //add current state
  return ( enc_states[( old_AB & 0x0f )]);
}

/* returns change in encoder state (-1,0,1) */
int8_t read_encoder2(){
static int8_t enc_states[] = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0};
static uint8_t old_AB = 0;
  uint8_t portVal = digitalRead(encPinA2);
  portVal = portVal<<1;
  portVal |= digitalRead(encPinB2);
  old_AB <<= 2;                   //remember previous state
  old_AB |= ( portVal & 0x03 );  //add current state
  return ( enc_states[( old_AB & 0x0f )]);
}