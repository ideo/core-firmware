#define PART_NAME "TH_Core_"
#define PART_NUM "1"
#define PART_ID PART_NAME PART_NUM
#define POLL_PERIOD 500
#define SERIAL_DEBUG 0

/*
Noam stuff: search "NOAM:" for places to fill in Noam functionality.
 */

//Noam:
Lemma lemma(PART_ID, ROOM_ID);

unsigned long int pollTimer = millis();

byte fetch_humidity_temperature(unsigned int *p_H_dat, unsigned int *p_T_dat);

void setup()
{
  #if SERIAL_DEBUG
  Serial.begin(9600);
  delay(5);
  #endif  
  Wire.begin();
  delay(5);

  //  NOAM: 
  lemma.begin();
}

void loop()
{  
  // NOAM: 
  lemma.run();
  if( millis() - pollTimer > POLL_PERIOD){
    pollTimer = millis();
    byte _status;
    unsigned int H_dat, T_dat;
    double RH, T_C;
    _status = fetch_humidity_temperature(&H_dat, &T_dat);        
    RH = (double) H_dat * 6.10e-3;
    T_C = (double) T_dat * 1.007e-2 - 40.0;
    double dataArray[2];
    dataArray[0] = RH;
    dataArray[1] = T_C;   

    #define topicID "TempHumid " PART_NUM

    #if SERIAL_DEBUG
    Serial.print(topicID);Serial.print("  ");Serial.print(dataArray[0]);Serial.print(" ");Serial.println(dataArray[1]);
    #endif
    // NOAM:
    // if(lemma.isConnectedToNoam){
     lemma.sendDoubleArray( topicID , dataArray , 2 );
   // }   
 }
}

byte fetch_humidity_temperature(unsigned int *p_H_dat, unsigned int *p_T_dat)
{
  byte address, Hum_H, Hum_L, Temp_H, Temp_L, _status;
  unsigned int H_dat, T_dat;
  address = 0x27;
  Wire.beginTransmission(address); 
  Wire.endTransmission();
  delay(100);

  Wire.requestFrom((int)address, (int) 4);
  Hum_H = Wire.read();
  Hum_L = Wire.read();
  Temp_H = Wire.read();
  Temp_L = Wire.read();
  Wire.endTransmission();

  _status = (Hum_H >> 6) & 0x03;
  Hum_H = Hum_H & 0x3f;
  H_dat = (((unsigned int)Hum_H) << 8) | Hum_L;
  T_dat = (((unsigned int)Temp_H) << 8) | Temp_L;
  T_dat = T_dat / 4;
  *p_H_dat = H_dat;
  *p_T_dat = T_dat;
  return(_status);
}