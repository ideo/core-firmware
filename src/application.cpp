/*

 This Arduino project is a demonstration of the Noam platform's Arduino Lemma Library (Ethernet).
 This project was developed on an Arduino Mega 2560 with Ethernet shield.
 When the Arduino is connected to a Noam network, pushing the buttong will send a signal that is echoed and causes the LED to light up while the button is pressed.
 
 When developing Arduino for Noam, it is important to use an event-centric paradigm.  Using software debounce ensures messages are only sent when the button state changes.
 Keeping delays out of the main loop will ensure fast message response, as new messages are polled in the loop.
 
 Author: Evan Shapiro [ IDEO ]
 
 */

#define IS_SPARK_CORE 1
#include "application.h"

#define ROOM_ID "test_CIID_room"
#define PART_NAME "IMU_Core_"
#define PART_NUM "1"
#define PART_ID PART_NAME PART_NUM

/*
Noam stuff: search "NOAM:" for places to fill in Noam functionality.
 */

/***************************************************************************************************************
 * TODOs:
 *   * Allow optional use of EEPROM for storing and reading calibration values.
 *   * Use self-test and temperature-compensation features of the sensors.
 ***************************************************************************************************************/
/*
  Serial commands that the firmware understands:
 
 "#o<params>" - Set OUTPUT mode and parameters. The available options are:
 
 // Streaming output
 "#o0" - DISABLE continuous streaming output. Also see #f below.
 "#o1" - ENABLE continuous streaming output.
 "#os" - output sensors.
 "#oa" - output angles.
// OUTPUT OPTIONS
/*****************************************************************/
#define OUTPUT__BAUD_RATE 57600
#define OUTPUT__DATA_INTERVAL 100  // in milliseconds

// Output mode definitions (do not change)
#define OUTPUT__MODE_SENSORS 0 // Outputs sensor min/max values as text for manual calibration
#define OUTPUT__MODE_ANGLES 1 // Outputs yaw/pitch/roll in degrees

// Select your startup output mode and format here!
int output_mode = OUTPUT__MODE_SENSORS;

// Select if serial continuous streaming output is enabled per default on startup.
#define OUTPUT__STARTUP_STREAM_ON false  // true or false

// If set true, an error message will be output if we fail to read sensor data.
// Message format: "!ERR: reading <sensor>", followed by "\r\n".
boolean output_errors = false;  // true or false
#define OUTPUT__HAS_RN_BLUETOOTH false  // true or false


// SENSOR CALIBRATION
/*****************************************************************/
// How to calibrate? Read the tutorial at http://dev.qu.tu-berlin.de/projects/sf-razor-9dof-ahrs
// Put MIN/MAX and OFFSET readings for your board here!
// Accelerometer
// "accel x,y,z (min/max) = X_MIN/X_MAX  Y_MIN/Y_MAX  Z_MIN/Z_MAX"
#define ACCEL_X_MIN ((float) -250)
#define ACCEL_X_MAX ((float) 250)
#define ACCEL_Y_MIN ((float) -250)
#define ACCEL_Y_MAX ((float) 250)
#define ACCEL_Z_MIN ((float) -250)
#define ACCEL_Z_MAX ((float) 250)

// Magnetometer (standard calibration mode)
// "magn x,y,z (min/max) = X_MIN/X_MAX  Y_MIN/Y_MAX  Z_MIN/Z_MAX"
#define MAGN_X_MIN ((float) -600)
#define MAGN_X_MAX ((float) 600)
#define MAGN_Y_MIN ((float) -600)
#define MAGN_Y_MAX ((float) 600)
#define MAGN_Z_MIN ((float) -600)
#define MAGN_Z_MAX ((float) 600)

// Gyroscope
// "gyro x,y,z (current/average) = .../OFFSET_X  .../OFFSET_Y  .../OFFSET_Z
#define GYRO_AVERAGE_OFFSET_X ((float) 0.0)
#define GYRO_AVERAGE_OFFSET_Y ((float) 0.0)
#define GYRO_AVERAGE_OFFSET_Z ((float) 0.0)

////////

// Sensor calibration scale and offset values
#define ACCEL_X_OFFSET ((ACCEL_X_MIN + ACCEL_X_MAX) / 2.0f)
#define ACCEL_Y_OFFSET ((ACCEL_Y_MIN + ACCEL_Y_MAX) / 2.0f)
#define ACCEL_Z_OFFSET ((ACCEL_Z_MIN + ACCEL_Z_MAX) / 2.0f)
#define ACCEL_X_SCALE (GRAVITY / (ACCEL_X_MAX - ACCEL_X_OFFSET))
#define ACCEL_Y_SCALE (GRAVITY / (ACCEL_Y_MAX - ACCEL_Y_OFFSET))
#define ACCEL_Z_SCALE (GRAVITY / (ACCEL_Z_MAX - ACCEL_Z_OFFSET))

#define MAGN_X_OFFSET ((MAGN_X_MIN + MAGN_X_MAX) / 2.0f)
#define MAGN_Y_OFFSET ((MAGN_Y_MIN + MAGN_Y_MAX) / 2.0f)
#define MAGN_Z_OFFSET ((MAGN_Z_MIN + MAGN_Z_MAX) / 2.0f)
#define MAGN_X_SCALE (100.0f / (MAGN_X_MAX - MAGN_X_OFFSET))
#define MAGN_Y_SCALE (100.0f / (MAGN_Y_MAX - MAGN_Y_OFFSET))
#define MAGN_Z_SCALE (100.0f / (MAGN_Z_MAX - MAGN_Z_OFFSET))

// Gain for gyroscope (ITG-3200)
#define GYRO_GAIN 0.06957 // Same gain on all axes
#define GYRO_SCALED_RAD(x) (x * TO_RAD(GYRO_GAIN)) // Calculate the scaled gyro readings in radians per second

// DCM parameters
#define Kp_ROLLPITCH 0.02f
#define Ki_ROLLPITCH 0.00002f
#define Kp_YAW 1.2f
#define Ki_YAW 0.00002f

// Stuff
#define GRAVITY 256.0f // "1G reference" used for DCM filter and accelerometer calibration
#define TO_RAD(x) (x * 0.01745329252f)  // *pi/180
#define TO_DEG(x) (x * 57.2957795131f)  // *180/pi

// Sensor variables
float accel[3];  // Actually stores the NEGATED acceleration (equals gravity, if board not moving).
float accel_min[3];
float accel_max[3];

float magnetom[3];
float magnetom_min[3];
float magnetom_max[3];
float magnetom_tmp[3];

float gyro[3];
float gyro_average[3];
int gyro_num_samples = 0;

// DCM variables
float MAG_Heading;
float Accel_Vector[3]= {
  0, 0, 0}; // Store the acceleration in a vector
float Gyro_Vector[3]= {
  0, 0, 0}; // Store the gyros turn rate in a vector
float Omega_Vector[3]= {
  0, 0, 0}; // Corrected Gyro_Vector data
float Omega_P[3]= {
  0, 0, 0}; // Omega Proportional correction
float Omega_I[3]= {
  0, 0, 0}; // Omega Integrator
float Omega[3]= {
  0, 0, 0};
float errorRollPitch[3] = {
  0, 0, 0};
float errorYaw[3] = {
  0, 0, 0};
float DCM_Matrix[3][3] = {
  {
    1, 0, 0  }
  , {
    0, 1, 0  }
  , {
    0, 0, 1  }
};
float Update_Matrix[3][3] = {
  {
    0, 1, 2  }
  , {
    3, 4, 5  }
  , {
    6, 7, 8  }
};
float Temporary_Matrix[3][3] = {
  {
    0, 0, 0  }
  , {
    0, 0, 0  }
  , {
    0, 0, 0  }
};

// Euler angles
float yaw;
float pitch;
float roll;

// DCM timing in the main loop
unsigned long timestamp;
unsigned long timestamp_old;
float G_Dt; // Integration time for DCM algorithm

Lemma lemma(PART_ID);

// More output-state variables
boolean output_stream_on;
int num_accel_errors = 0;
int num_magn_errors = 0;
int num_gyro_errors = 0;


/* Function Prototypes */
void printVector(const float a[3]);
void printMatrix(const float a[3][3]);

void Read_Gyro();
void Read_Accel();
void Read_Magn();

void Vector_Add(float out[3], const float v1[3], const float v2[3]);
float Vector_Dot_Product(const float v1[3], const float v2[3]);
void Vector_Scale(float out[3], const float v[3], float scale);
void Vector_Cross_Product(float out[3], const float v1[3], const float v2[3]);
void Compass_Heading();

void Normalize(void);
void Matrix_update(void);
void Matrix_Vector_Multiply(const float a[3][3], const float b[3], float out[3]);
void Matrix_Multiply(const float a[3][3], const float b[3][3], float out[3][3]);
void init_rotation_matrix(float m[3][3], float yaw, float pitch, float roll);

void Euler_angles(void);
void Drift_correction(void);

void I2C_Init();
void Accel_Init();
void Magn_Init();
void Gyro_Init();  

void output_angles();
void output_sensors_text(char raw_or_calibrated);
void output_sensors();



void read_sensors() {
  Read_Gyro(); // Read gyroscope
  Read_Accel(); // Read accelerometer
  Read_Magn(); // Read magnetometer
}

// Read every sensor and record a time stamp
// Init DCM with unfiltered orientation
// TODO re-init global vars?
void reset_sensor_fusion() {
  float temp1[3];
  float temp2[3];
  float xAxis[] = {
    1.0f, 0.0f, 0.0f  };

  read_sensors();
  timestamp = millis();

  // GET PITCH
  // Using y-z-plane-component/x-component of gravity vector
  pitch = -atan2(accel[0], sqrt(accel[1] * accel[1] + accel[2] * accel[2]));

  // GET ROLL
  // Compensate pitch of gravity vector 
  Vector_Cross_Product(temp1, accel, xAxis);
  Vector_Cross_Product(temp2, xAxis, temp1);
  roll = atan2(temp2[1], temp2[2]);

  // GET YAW
  Compass_Heading();
  yaw = MAG_Heading;

  // Init rotation matrix
  init_rotation_matrix(DCM_Matrix, yaw, pitch, roll);

  Serial.println("DCM_Matrix");
  printMatrix(DCM_Matrix);

}

// Apply calibration to raw sensor readings
void compensate_sensor_errors() {
  accel[0] = (accel[0] - ACCEL_X_OFFSET) * ACCEL_X_SCALE;
  accel[1] = (accel[1] - ACCEL_Y_OFFSET) * ACCEL_Y_SCALE;
  accel[2] = (accel[2] - ACCEL_Z_OFFSET) * ACCEL_Z_SCALE;

  magnetom[0] = (magnetom[0] - MAGN_X_OFFSET) * MAGN_X_SCALE;
  magnetom[1] = (magnetom[1] - MAGN_Y_OFFSET) * MAGN_Y_SCALE;
  magnetom[2] = (magnetom[2] - MAGN_Z_OFFSET) * MAGN_Z_SCALE;

  gyro[0] -= GYRO_AVERAGE_OFFSET_X;
  gyro[1] -= GYRO_AVERAGE_OFFSET_Y;
  gyro[2] -= GYRO_AVERAGE_OFFSET_Z;
}

void turn_output_stream_on()
{
  output_stream_on = true;
}

void turn_output_stream_off()
{
  output_stream_on = false;
}

// Blocks until another byte is available on serial port
char readChar()
{
  while (Serial.available() < 1) { 
  } // Block
  return Serial.read();
}

void dataTypeHandler(Event const & e){
  String dataString = e.stringValue;
  if( dataString == "angles") output_mode = OUTPUT__MODE_ANGLES;
  else if( dataString == "sensors") output_mode = OUTPUT__MODE_SENSORS;
}

void setup()
{
  Serial.begin(OUTPUT__BAUD_RATE);
  delay(5);
  I2C_Init();
  Accel_Init();
  Magn_Init();
  Gyro_Init();  
  delay(20);  // Give sensors enough time to collect data
  reset_sensor_fusion();
  // Init output
  turn_output_stream_on();

  // Wait 10 seconds
  //Delay(1000 * 10);
  
//  NOAM: 
    String hearString = "dataType ";
    hearString += PART_NUM;
    lemma.hear( "dataType---" , dataTypeHandler );
  lemma.begin( ); //TODO: ROOM_ID
}

// Main loop
void loop()
{
  // Read incoming control messages
  if (Serial.available() >= 2){
    if (Serial.read() == '#'){ // Start of new control message
      int command = Serial.read(); // Commands
      if (command == 'o'){ // Set _o_utput mode      
        char output_param = readChar();
        if (output_param == 's'){ // Output _s_ensor values
          output_mode = OUTPUT__MODE_SENSORS;
        }
        else if (output_param == 'a'){ //angles
          output_mode = OUTPUT__MODE_ANGLES; 
        }
        else if (output_param == '0'){ // Disable continuous streaming output
          turn_output_stream_off();
        }
        else if (output_param == '1'){ // Enable continuous streaming output
          turn_output_stream_on();
        }
        else if (output_param == 'e'){ // _e_rror output settings
          char error_param = readChar();
          if (error_param == '0') output_errors = false;
          else if (error_param == '1') output_errors = true;
          else if (error_param == 'c'){ // get error count          
            Serial.print("#AMG-ERR:");
            Serial.print(num_accel_errors); 
            Serial.print(",");
            Serial.print(num_magn_errors); 
            Serial.print(",");
            Serial.println(num_gyro_errors);
          }
        }
      }
    }
    else
    { 
    } // Skip character
  }
  
  lemma.run();

  // Time to read the sensors again?
  if((millis() - timestamp) >= OUTPUT__DATA_INTERVAL){
    timestamp_old = timestamp;
    timestamp = millis();
    if (timestamp > timestamp_old) G_Dt = (float) (timestamp - timestamp_old) / 1000.0f;
    else G_Dt = 0;
    // Update sensor readings
    read_sensors();    
    if (output_mode == OUTPUT__MODE_ANGLES){  // Output angles
      // Apply sensor calibration
      Serial.println("\n\n\n==============");
      compensate_sensor_errors();   
  
      // Run DCM algorithm
      Compass_Heading(); // Calculate magnetic heading
      Matrix_update();
      Normalize();
      Drift_correction();
      Euler_angles();

      //      NOAM:
      //      if(lemma.isConnectedToNoam){
      //float angleArray[3] = { 
      //  TO_DEG(yaw) , TO_DEG(pitch) , TO_DEG(roll)       };
      String topicID = "AnglesYPR ";
      topicID += PART_NUM;
      //        lemma.speak( topicID , angleArray );
      //      }
      //      /NOAM stuff

      if (output_stream_on ) output_angles();
    }
    else{  // Output sensor values
          compensate_sensor_errors();
      String topicID = "AccelXYZ ";
      topicID += PART_NUM;
      double accelAsDouble[3] = {(double)accel[0], (double)accel[1], (double)accel[2]};
      lemma.sendDoubleArray( "AccelXYZ" , accelAsDouble , 3);  //send float array

      topicID = "GyroXYZ ";
      topicID += PART_NUM;
      //        lemma.speak( topicID , gyro );  //send float array
            
      topicID = "MagXYZ ";
      topicID += PART_NUM;
      //        lemma.speak( topicID , magnetom );  //send float array
      //      }
      //      /NOAM stuff    
      if (output_stream_on ) output_sensors();
    }   
  }
}
///////////// DCM ////////////////////////////////////////////////////////////////////////
/* This file is part of the Razor AHRS Firmware */

// DCM algorithm

/**************************************************/
void Normalize(void)
{
  float error=0;
  float temporary[3][3];
  float renorm=0;

  error= -Vector_Dot_Product(&DCM_Matrix[0][0],&DCM_Matrix[1][0])*.5; //eq.19

  Vector_Scale(&temporary[0][0], &DCM_Matrix[1][0], error); //eq.19
  Vector_Scale(&temporary[1][0], &DCM_Matrix[0][0], error); //eq.19
  
  Vector_Add(&temporary[0][0], &temporary[0][0], &DCM_Matrix[0][0]);//eq.19
  Vector_Add(&temporary[1][0], &temporary[1][0], &DCM_Matrix[1][0]);//eq.19
  
  Vector_Cross_Product(&temporary[2][0],&temporary[0][0],&temporary[1][0]); // c= a x b //eq.20
  
  renorm= .5 *(3 - Vector_Dot_Product(&temporary[0][0],&temporary[0][0])); //eq.21
  Vector_Scale(&DCM_Matrix[0][0], &temporary[0][0], renorm);
  
  renorm= .5 *(3 - Vector_Dot_Product(&temporary[1][0],&temporary[1][0])); //eq.21
  Vector_Scale(&DCM_Matrix[1][0], &temporary[1][0], renorm);
  
  renorm= .5 *(3 - Vector_Dot_Product(&temporary[2][0],&temporary[2][0])); //eq.21
  Vector_Scale(&DCM_Matrix[2][0], &temporary[2][0], renorm);
}

/**************************************************/
void Drift_correction(void)
{
  float mag_heading_x;
  float mag_heading_y;
  float errorCourse;
  //Compensation the Roll, Pitch and Yaw drift. 
  static float Scaled_Omega_P[3];
  static float Scaled_Omega_I[3];
  float Accel_magnitude;
  float Accel_weight;
  
  // Serial.println("Drift_correction");

  // Serial.println("Accel_Vector");
  // printVector(Accel_Vector);
  //*****Roll and Pitch***************

  // Calculate the magnitude of the accelerometer vector
  Accel_magnitude = sqrt(Accel_Vector[0]*Accel_Vector[0] + Accel_Vector[1]*Accel_Vector[1] + Accel_Vector[2]*Accel_Vector[2]);
  Accel_magnitude = Accel_magnitude / GRAVITY; // Scale to gravity.
  // Dynamic weighting of accelerometer info (reliability filter)
  // Weight for accelerometer info (<0.5G = 0.0, 1G = 1.0 , >1.5G = 0.0)
  Accel_weight = constrain(1 - 2*abs(1 - Accel_magnitude),0,1);  //  
  // Serial.print("Accel_magnitude: ");
  // Serial.println(Accel_magnitude);

  // Serial.print("Accel_weight: ");
  // Serial.println(Accel_weight);  

  Vector_Cross_Product(&errorRollPitch[0],&Accel_Vector[0],&DCM_Matrix[2][0]); //adjust the ground of reference
  
  // Serial.print("errorRollPitch");
  // printVector(errorRollPitch);

  // Serial.print("Accel_Vector");
  // printVector(Accel_Vector);

  // Serial.print("DCM_Matrix");
  // printMatrix(DCM_Matrix);


  Vector_Scale(&Omega_P[0],&errorRollPitch[0],Kp_ROLLPITCH*Accel_weight);
  
  Vector_Scale(&Scaled_Omega_I[0],&errorRollPitch[0],Ki_ROLLPITCH*Accel_weight);
  Vector_Add(Omega_I,Omega_I,Scaled_Omega_I);     
  
  //*****YAW***************
  // We make the gyro YAW drift correction based on compass magnetic heading
 
  mag_heading_x = cos(MAG_Heading);
  mag_heading_y = sin(MAG_Heading);
  errorCourse=(DCM_Matrix[0][0]*mag_heading_y) - (DCM_Matrix[1][0]*mag_heading_x);  //Calculating YAW error
  Vector_Scale(errorYaw,&DCM_Matrix[2][0],errorCourse); //Applys the yaw correction to the XYZ rotation of the aircraft, depeding the position.
  
  Vector_Scale(&Scaled_Omega_P[0],&errorYaw[0],Kp_YAW);//.01proportional of YAW.
  Vector_Add(Omega_P,Omega_P,Scaled_Omega_P);//Adding  Proportional.
  
  Vector_Scale(&Scaled_Omega_I[0],&errorYaw[0],Ki_YAW);//.00001Integrator
  Vector_Add(Omega_I,Omega_I,Scaled_Omega_I);//adding integrator to the Omega_I

}

void Matrix_update(void)
{

  Gyro_Vector[0]=GYRO_SCALED_RAD(gyro[0]); //gyro x roll
  Gyro_Vector[1]=GYRO_SCALED_RAD(gyro[1]); //gyro y pitch
  Gyro_Vector[2]=GYRO_SCALED_RAD(gyro[2]); //gyro z yaw
  
  Accel_Vector[0]=accel[0];
  Accel_Vector[1]=accel[1];
  Accel_Vector[2]=accel[2];
  
  Serial.println("Gyro_Vector: ");
  printVector(Gyro_Vector);
  Serial.println("Omega_I: ");
  printVector(Omega_I);
  Serial.println("Omega: ");
  printVector(Omega);

  Vector_Add(&Omega[0], &Gyro_Vector[0], &Omega_I[0]);  //adding proportional term
  Vector_Add(&Omega_Vector[0], &Omega[0], &Omega_P[0]); //adding Integrator term

  Serial.println("Omega_Vector: ");
  printVector(Omega_Vector);

  Update_Matrix[0][0]=0;
  Update_Matrix[0][1]=-G_Dt*Omega_Vector[2];//-z
  Update_Matrix[0][2]=G_Dt*Omega_Vector[1];//y
  Update_Matrix[1][0]=G_Dt*Omega_Vector[2];//z
  Update_Matrix[1][1]=0;
  Update_Matrix[1][2]=-G_Dt*Omega_Vector[0];//-x
  Update_Matrix[2][0]=-G_Dt*Omega_Vector[1];//-y
  Update_Matrix[2][1]=G_Dt*Omega_Vector[0];//x
  Update_Matrix[2][2]=0;

  Serial.println("DCM_Matrix: ");
  printMatrix(DCM_Matrix);

  Matrix_Multiply(DCM_Matrix,Update_Matrix,Temporary_Matrix); //a*b=c

  Serial.println("Update_Matrix: ");
  printMatrix(Update_Matrix);

  for(int x=0; x<3; x++) //Matrix Addition (update)
  {
    for(int y=0; y<3; y++)
    {
      DCM_Matrix[x][y]+=Temporary_Matrix[x][y];
    } 
  }

  Serial.println("DCM_Matrix: ");
  printMatrix(DCM_Matrix);
}

void Euler_angles(void)
{
  pitch = -asin(DCM_Matrix[2][0]);
  roll = atan2(DCM_Matrix[2][1],DCM_Matrix[2][2]);
  yaw = atan2(DCM_Matrix[1][0],DCM_Matrix[0][0]);
}

///////////// COMPASS PARSING ////////////////////////////////////////////////////////////////////////
/* This file is part of the Razor AHRS Firmware */

void Compass_Heading()
{
  float mag_x;
  float mag_y;
  float cos_roll;
  float sin_roll;
  float cos_pitch;
  float sin_pitch;
  
  cos_roll = cos(roll);
  sin_roll = sin(roll);
  cos_pitch = cos(pitch);
  sin_pitch = sin(pitch);
  
  // Tilt compensated magnetic field X
  mag_x = magnetom[0] * cos_pitch + magnetom[1] * sin_roll * sin_pitch + magnetom[2] * cos_roll * sin_pitch;
  // Tilt compensated magnetic field Y
  mag_y = magnetom[1] * cos_roll - magnetom[2] * sin_roll;
  // Magnetic Heading
  MAG_Heading = atan2(-mag_y, mag_x);

}

///////////// MATH PARSING ////////////////////////////////////////////////////////////////////////
/* This file is part of the Razor AHRS Firmware */

// Computes the dot product of two vectors
float Vector_Dot_Product(const float v1[3], const float v2[3])
{
  float result = 0;
  
  for(int c = 0; c < 3; c++)
  {
    result += v1[c] * v2[c];
  }
  
  return result; 
}

// Computes the cross product of two vectors
// out has to different from v1 and v2 (no in-place)!
void Vector_Cross_Product(float out[3], const float v1[3], const float v2[3])
{
  out[0] = (v1[1] * v2[2]) - (v1[2] * v2[1]);
  out[1] = (v1[2] * v2[0]) - (v1[0] * v2[2]);
  out[2] = (v1[0] * v2[1]) - (v1[1] * v2[0]);
}

// Multiply the vector by a scalar
void Vector_Scale(float out[3], const float v[3], float scale)
{
  for(int c = 0; c < 3; c++)
  {
    out[c] = v[c] * scale; 
  }
}

// Adds two vectors
void Vector_Add(float out[3], const float v1[3], const float v2[3])
{
  for(int c = 0; c < 3; c++)
  {
    out[c] = v1[c] + v2[c];
  }
}

// Multiply two 3x3 matrices: out = a * b
// out has to different from a and b (no in-place)!
void Matrix_Multiply(const float a[3][3], const float b[3][3], float out[3][3])
{
  for(int x = 0; x < 3; x++)  // rows
  {
    for(int y = 0; y < 3; y++)  // columns
    {
      out[x][y] = a[x][0] * b[0][y] + a[x][1] * b[1][y] + a[x][2] * b[2][y];
    }
  }
}

// Multiply 3x3 matrix with vector: out = a * b
// out has to different from b (no in-place)!
void Matrix_Vector_Multiply(const float a[3][3], const float b[3], float out[3])
{
  for(int x = 0; x < 3; x++)
  {
    out[x] = a[x][0] * b[0] + a[x][1] * b[1] + a[x][2] * b[2];
  }
}

// Init rotation matrix using euler angles
void init_rotation_matrix(float m[3][3], float yaw, float pitch, float roll)
{
  float c1 = cos(roll);
  float s1 = sin(roll);
  float c2 = cos(pitch);
  float s2 = sin(pitch);
  float c3 = cos(yaw);
  float s3 = sin(yaw);

  // Euler angles, right-handed, intrinsic, XYZ convention
  // (which means: rotate around body axes Z, Y', X'') 
  m[0][0] = c2 * c3;
  m[0][1] = c3 * s1 * s2 - c1 * s3;
  m[0][2] = s1 * s3 + c1 * c3 * s2;

  m[1][0] = c2 * s3;
  m[1][1] = c1 * c3 + s1 * s2 * s3;
  m[1][2] = c1 * s2 * s3 - c3 * s1;

  m[2][0] = -s2;
  m[2][1] = c2 * s1;
  m[2][2] = c1 * c2;
}

///////////// OUTPUT ////////////////////////////////////////////////////////////////////////
/* This file is part of the Razor AHRS Firmware */

// Output angles: yaw, pitch, roll
void output_angles()
{
  Serial.print("#YPR=");
  Serial.print(TO_DEG(yaw)); 
  Serial.print(",");
  Serial.print(TO_DEG(pitch)); 
  Serial.print(",");
  Serial.print(TO_DEG(roll)); 
  Serial.println();  
}

void output_sensors_text(char raw_or_calibrated)
{
  Serial.print("#A-"); 
  Serial.print(raw_or_calibrated); 
  Serial.print('=');
  Serial.print(accel[0]); 
  Serial.print(",");
  Serial.print(accel[1]); 
  Serial.print(",");
  Serial.print(accel[2]); 
  Serial.print("   ");

  Serial.print("#M-"); 
  Serial.print(raw_or_calibrated); 
  Serial.print('=');
  Serial.print(magnetom[0]); 
  Serial.print(",");
  Serial.print(magnetom[1]); 
  Serial.print(",");
  Serial.print(magnetom[2]); 
  Serial.print("   ");

  Serial.print("#G-"); 
  Serial.print(raw_or_calibrated); 
  Serial.print('=');
  Serial.print(gyro[0]); 
  Serial.print(",");
  Serial.print(gyro[1]); 
  Serial.print(",");
  Serial.print(gyro[2]); 
  Serial.println();
}

void output_sensors()
{
  // Apply sensor calibration
  compensate_sensor_errors();
  output_sensors_text('C');
}

///////////// SENSORS ////////////////////////////////////////////////////////////////////////
// I2C code to read the sensors

// Sensor I2C addresses
#define ACCEL_ADDRESS ((int) 0x53) // 0x53 = 0xA6 / 2
#define MAGN_ADDRESS  ((int) 0x1E) // 0x1E = 0x3C / 2
#define GYRO_ADDRESS  ((int) 0x68) // 0x68 = 0xD0 / 2

// Arduino backward compatibility macros
#if (ARDUINO >= 100 || IS_SPARK_CORE) 
  #define WIRE_SEND(b) Wire.write((byte) b) 
  #define WIRE_RECEIVE() Wire.read() 
#else
  #define WIRE_SEND(b) Wire.send(b)
  #define WIRE_RECEIVE() Wire.receive() 
#endif


void I2C_Init()
{
  Wire.begin();
}

void Accel_Init()
{
  Wire.beginTransmission(ACCEL_ADDRESS);
  WIRE_SEND(0x2D);  // Power register
  WIRE_SEND(0x08);  // Measurement mode
  Wire.endTransmission();
  delay(5);
  Wire.beginTransmission(ACCEL_ADDRESS);
  WIRE_SEND(0x31);  // Data format register
  WIRE_SEND(0x08);  // Set to full resolution
  Wire.endTransmission();
  delay(5);
  
  // Because our main loop runs at 50Hz we adjust the output data rate to 50Hz (25Hz bandwidth)
  Wire.beginTransmission(ACCEL_ADDRESS);
  WIRE_SEND(0x2C);  // Rate
  WIRE_SEND(0x09);  // Set to 50Hz, normal operation
  Wire.endTransmission();
  delay(5);
}

// Reads x, y and z accelerometer registers
void Read_Accel()
{
  int i = 0;
  byte buff[6];
  
  Wire.beginTransmission(ACCEL_ADDRESS); 
  WIRE_SEND(0x32);  // Send address to read from
  Wire.endTransmission();
  
  Wire.beginTransmission(ACCEL_ADDRESS);
  Wire.requestFrom(ACCEL_ADDRESS, 6);  // Request 6 bytes
  while(Wire.available())  // ((Wire.available())&&(i<6))
  { 
    buff[i] = WIRE_RECEIVE();  // Read one byte
    i++;
  }
  Wire.endTransmission();
  
  if (i == 6)  // All bytes received?
  {
    // No multiply by -1 for coordinate system transformation here, because of double negation:
    // We want the gravity vector, which is negated acceleration vector.
    accel[0] = (((int) buff[3]) << 8) | buff[2];  // X axis (internal sensor y axis)
    accel[1] = (((int) buff[1]) << 8) | buff[0];  // Y axis (internal sensor x axis)
    accel[2] = (((int) buff[5]) << 8) | buff[4];  // Z axis (internal sensor z axis)
  }
  else
  {
    num_accel_errors++;
    if (output_errors) Serial.println("!ERR: reading accelerometer");
  }
}

void Magn_Init()
{
  Wire.beginTransmission(MAGN_ADDRESS);
  WIRE_SEND(0x02); 
  WIRE_SEND(0x00);  // Set continuous mode (default 10Hz)
  Wire.endTransmission();
  delay(5);

  Wire.beginTransmission(MAGN_ADDRESS);
  WIRE_SEND(0x00);
  WIRE_SEND(0b00011000);  // Set 50Hz
  Wire.endTransmission();
  delay(5);
}

void Read_Magn()
{
  int i = 0;
  byte buff[6];
 
  Wire.beginTransmission(MAGN_ADDRESS); 
  WIRE_SEND(0x03);  // Send address to read from
  Wire.endTransmission();
  
  Wire.beginTransmission(MAGN_ADDRESS); 
  Wire.requestFrom(MAGN_ADDRESS, 6);  // Request 6 bytes
  while(Wire.available())  // ((Wire.available())&&(i<6))
  { 
    buff[i] = WIRE_RECEIVE();  // Read one byte
    i++;
  }
  Wire.endTransmission();
  
  if (i == 6)  // All bytes received?
  {
    magnetom[0] = (((int) buff[0]) << 8) | buff[1];         // X axis (internal sensor x axis)
    magnetom[1] = -1 * ((((int) buff[4]) << 8) | buff[5]);  // Y axis (internal sensor -y axis)
    magnetom[2] = -1 * ((((int) buff[2]) << 8) | buff[3]);  // Z axis (internal sensor -z axis)
  }
  else
  {
    num_magn_errors++;
    if (output_errors) Serial.println("!ERR: reading magnetometer");
  }
}

void Gyro_Init()
{
  // Power up reset defaults
  Wire.beginTransmission(GYRO_ADDRESS);
  WIRE_SEND(0x3E);
  WIRE_SEND(0x80);
  Wire.endTransmission();
  delay(5);
  
  // Select full-scale range of the gyro sensors
  // Set LP filter bandwidth to 42Hz
  Wire.beginTransmission(GYRO_ADDRESS);
  WIRE_SEND(0x16);
  WIRE_SEND(0x1B);  // DLPF_CFG = 3, FS_SEL = 3
  Wire.endTransmission();
  delay(5);
  
  // Set sample rato to 50Hz
  Wire.beginTransmission(GYRO_ADDRESS);
  WIRE_SEND(0x15);
  WIRE_SEND(0x0A);  //  SMPLRT_DIV = 10 (50Hz)
  Wire.endTransmission();
  delay(5);

  // Set clock to PLL with z gyro reference
  Wire.beginTransmission(GYRO_ADDRESS);
  WIRE_SEND(0x3E);
  WIRE_SEND(0x00);
  Wire.endTransmission();
  delay(5);
}

// Reads x, y and z gyroscope registers
void Read_Gyro()
{
  int i = 0;
  byte buff[6];
  
  Wire.beginTransmission(GYRO_ADDRESS); 
  WIRE_SEND(0x1D);  // Sends address to read from
  Wire.endTransmission();
  
  Wire.beginTransmission(GYRO_ADDRESS);
  Wire.requestFrom(GYRO_ADDRESS, 6);  // Request 6 bytes
  while(Wire.available())  // ((Wire.available())&&(i<6))
  { 
    buff[i] = WIRE_RECEIVE();  // Read one byte
    i++;
  }
  Wire.endTransmission();
  
  if (i == 6)  // All bytes received?
  {
    gyro[0] = -1 * ((((int) buff[2]) << 8) | buff[3]);    // X axis (internal sensor -y axis)
    gyro[1] = -1 * ((((int) buff[0]) << 8) | buff[1]);    // Y axis (internal sensor -x axis)
    gyro[2] = -1 * ((((int) buff[4]) << 8) | buff[5]);    // Z axis (internal sensor -z axis)
  }
  else
  {
    num_gyro_errors++;
    if (output_errors) Serial.println("!ERR: reading gyroscope");
  }
}

void printVector(const float a[3]){
  Serial.print(a[0]);
  Serial.print(", ");
  Serial.print(a[1]);
  Serial.print(", ");
  Serial.println(a[2]);
}

void printMatrix(const float a[3][3]){
  printVector(a[0]);
  Serial.print("   ");
  printVector(a[1]);
  Serial.print("   ");
  printVector(a[2]);
}
