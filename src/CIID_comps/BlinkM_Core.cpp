#define PART_NAME "BlinkM_Core_"
#define PART_ID PART_NAME PART_NUM

/*
Noam stuff: search "NOAM:" for places to fill in Noam functionality.
 */ 

long int receivedCount = 0;

char serInStr[8];
Lemma lemma(PART_ID, ROOM_ID);  // Initialize the Arduino Noam Lemma with ID <PART_ID>

void scan(void);
void parseCommand(void);
uint8_t readSerialString(void);
int toHex(char hi, char lo);
static void BlinkM_setRGB(byte addr, byte red, byte grn, byte blu);

//NOAM:
void commandHandler(const Event & e){
  receivedCount++;
  DEBUG_LED_ON(40);
  DEBUG("Received: ");
  DEBUGLN(receivedCount);
  for(byte i=0; i<8; i++){
    serInStr[i] = e.stringValue[i];
  }
  parseCommand();
}

void setup(){
  DEBUG_LED_CONFIG();
  DEBUG_BEGIN();
  delay(50);
  scan();
  DEBUGLN("Serial initialized.");

  Wire.begin();

  //  NOAM:
  lemma.hear( "colorCommand_" PART_NUM , commandHandler );
  lemma.begin();

  DEBUG_LED_ITER();
}

void loop(){  
  //NOAM:   
  lemma.run();
  #if SERIAL_DEBUG
  if( readSerialString() > 0){
    parseCommand();
  }
  #endif
  DEBUG_LED_ITER()
}

void parseCommand(){
  DEBUG("> ");
  DEBUGLN(serInStr);
  if( serInStr[0] == 's' ){
    scan();
  } else{
    //ADDRESS PARSE
    char addrStr[2] = {
      serInStr[0] , serInStr[1]
    };
    int _addr = atoi( addrStr );
    if( _addr == 0){
      DEBUGLN("invalid address");
      return;
    }

    //RED PARSE
    int _red = toHex( serInStr[2] , serInStr[3] );
    if( _red == -1){
      DEBUGLN("invalid red");
      return;
    }

    //GREEN PARSE
    int _green = toHex( serInStr[4] , serInStr[5] );
    if( _green == -1){
      DEBUGLN("invalid green");
      return;
    }

    //BLUE PARSE
    int _blue = toHex( serInStr[6] , serInStr[7] );
    if( _blue == -1){
      DEBUGLN("invalid blue");
      return;
    }
    DEBUG_LED_ON(200);
    BlinkM_setRGB(_addr, _red, _green, _blue);
  }
  serInStr[0] = 0;
  serInStr[1] = 0; 
}

void scan(){
  #if SERIAL_DEBUG
  byte error = 2;  
  Serial.println("Scanning I2C bus from 1-100:");
  for(byte addr = 0; addr<128; addr++){
    Wire.beginTransmission(addr);
    error = Wire.endTransmission();
    if( error == 0 ){      
      Serial.print("found device at: "); 
      Serial.println( addr, DEC );
    }
  }
  Serial.println();
  #endif
}

//read a string from the serial and store it in an array
//you must supply the array variable
uint8_t readSerialString()
{
  if(!Serial.available()) {
    return 0;
  }
  delay(10);  // wait a little for serial data

  memset( serInStr, 0, sizeof(serInStr) ); // set it all to zero
  int i = 0;
  while (Serial.available()) {
    serInStr[i] = Serial.read();   // FIXME: doesn't check buffer overrun
    i++;
  }
  //serInStr[i] = 0;  // indicate end of read string
  return i;  // return number of chars read
}

// -----------------------------------------------------
// a really cheap strtol(s,NULL,16)
#include <ctype.h>
int toHex(char hi, char lo)
{
  uint8_t b;
  hi = toupper(hi);
  if( isxdigit(hi) ) {
    if( hi > '9' ) hi -= 7;      // software offset for A-F
    hi -= 0x30;                  // subtract ASCII offset
    b = hi<<4;
    lo = toupper(lo);
    if( isxdigit(lo) ) {
      if( lo > '9' ) lo -= 7;  // software offset for A-F
      lo -= 0x30;              // subtract ASCII offset
      b = b + lo;
      return b;
    } // else error
  }  // else error
  return -1;
}

// Sets an RGB color immediately
static void BlinkM_setRGB(byte addr, byte red, byte grn, byte blu)
{
  Wire.beginTransmission(addr);
  Wire.write('n');
  Wire.write(red);
  Wire.write(grn);
  Wire.write(blu);
  Wire.endTransmission();
}