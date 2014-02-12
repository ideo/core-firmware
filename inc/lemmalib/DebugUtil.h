#ifndef DEBUG_UTIL_H
#define DEBUG_UTIL_H


#include "spark_wiring.h"
#include "spark_wiring_usbserial.h"


//#define DEBUG


#ifdef DEBUG

#define PRINT(MSG, ...)         Serial.print(MSG, ##__VA_ARGS__)
#define PRINTLN(MSG, ...)       Serial.println(MSG, ##__VA_ARGS__)

#define PRINT_FUNCTION_PREFIX   { \
  PRINT("[");                     \
  PRINT(__PRETTY_FUNCTION__);     \
  PRINT("] ");                    \
}

#else

#define PRINT(MSG, ...)         {}
#define PRINTLN(MSG, ...)       {}
#define PRINT_FUNCTION_PREFIX   {}

#endif


#endif


