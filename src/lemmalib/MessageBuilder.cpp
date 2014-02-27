#include "DebugUtil.h"
#include "MessageBuilder.h"
#include "spark_wiring_string.h"
#include <nJSON.h>


char * lemmaID;


#ifdef __cplusplus
extern "C"
{
#endif


/* sprintf() and sscanf() functions do not support floating point format conversion in Arduino,
 * we need a way to convert floating point numbers to strings and back
 */


void fmtDouble(double val, byte precision, char *buf, unsigned bufLen = 0xffff);
unsigned fmtUnsigned(unsigned long val, char *buf, unsigned bufLen = 0xffff, byte width = 0);

// http://forum.arduino.cc/index.php/topic,44216.0.html#11
// by: Don Kinzer
// http://forum.arduino.cc/index.php?PHPSESSID=d5o728468ik95ettg59m5utc56&action=profile;u=7003
// 
// Produce a formatted string in a buffer corresponding to the value provided.
// If the 'width' parameter is non-zero, the value will be padded with leading
// zeroes to achieve the specified width.  The number of characters added to
// the buffer (not including the null termination) is returned.
//
unsigned
fmtUnsigned(unsigned long val, char *buf, unsigned bufLen, byte width)
{
  if (!buf || !bufLen)
    return(0);

  // produce the digit string (backwards in the digit buffer)
  char dbuf[10];
  unsigned idx = 0;
  while (idx < sizeof(dbuf))
  {
    dbuf[idx++] = (val % 10) + '0';
    if ((val /= 10) == 0)
      break;
  }

  // copy the optional leading zeroes and digits to the target buffer
  unsigned len = 0;
  byte padding = (width > idx) ? width - idx : 0;
  char c = '0';
  while ((--bufLen > 0) && (idx || padding))
  {
    if (padding)
      padding--;
    else
      c = dbuf[--idx];
    *buf++ = c;
    len++;
  }

  // add the null termination
  *buf = '\0';
  return(len);
}

// http://forum.arduino.cc/index.php/topic,44216.0.html#11
// by: Don Kinzer
// http://forum.arduino.cc/index.php?PHPSESSID=d5o728468ik95ettg59m5utc56&action=profile;u=7003
//
// Format a floating point value with number of decimal places.
// The 'precision' parameter is a number from 0 to 6 indicating the desired decimal places.
// The 'buf' parameter points to a buffer to receive the formatted string.  This must be
// sufficiently large to contain the resulting string.  The buffer's length may be
// optionally specified.  If it is given, the maximum length of the generated string
// will be one less than the specified value.
//
// example: fmtDouble(3.1415, 2, buf); // produces 3.14 (two decimal places)
//
void
fmtDouble(double val, byte precision, char *buf, unsigned bufLen)
{
  if (!buf || !bufLen)
    return;

  // limit the precision to the maximum allowed value
  const byte maxPrecision = 6;
  if (precision > maxPrecision)
    precision = maxPrecision;

  if (--bufLen > 0)
  {
    // check for a negative value
    if (val < 0.0)
    {
      val = -val;
      *buf = '-';
      bufLen--;
    }

    // compute the rounding factor and fractional multiplier
    double roundingFactor = 0.5;
    unsigned long mult = 1;
    for (byte i = 0; i < precision; i++)
    {
      roundingFactor /= 10.0;
      mult *= 10;
    }

    if (bufLen > 0)
    {
      // apply the rounding factor
      val += roundingFactor;

      // add the integral portion to the buffer
      unsigned len = fmtUnsigned((unsigned long)val, buf, bufLen);
      buf += len;
      bufLen -= len;
    }

    // handle the fractional portion
    if ((precision > 0) && (bufLen > 0))
    {
      *buf++ = '.';
      if (--bufLen > 0)
        buf += fmtUnsigned((unsigned long)((val - (unsigned long)val) * mult), buf, bufLen, precision);
    }
  }

  // null-terminate the string
  *buf = '\0';
}


/* encodes JSON-like non-nesting string based on type of value */
char * buildEvent( char const * name, int type, void const * value )
{
  int intValue = 0;
  float floatValue = 0.0;
  double doubleValue = 0.0;
  char *stringValue = NULL;
  unsigned int boolValue = 0;

  char buf[256];
  memset(buf, 0, 256);

  char temp[256];
  memset(temp, 0, 256);

  switch (type) {
    case nJSON_INT:
      intValue = *((int *)value);
      sprintf(buf, "[\"event\", \"%s\", \"%s\", %d]", lemmaID, name, intValue);
    break;
    case nJSON_FLOAT:
      floatValue = *((float *)value);
      sprintf(buf, "[\"event\", \"%s\", \"%s\", %f]", lemmaID, name, floatValue);
    break;
    case nJSON_DOUBLE:
      doubleValue = *((double *)value);
      fmtDouble(doubleValue, 6, temp, 256);
      sprintf(buf, "[\"event\", \"%s\", \"%s\", %s]", lemmaID, name, temp);
    break;
    case nJSON_STRING:
      stringValue = (char *)value;
      sprintf(buf, "[\"event\", \"%s\", \"%s\", \"%s\"]", lemmaID, name, stringValue);
    break;
    case nJSON_BOOL:
      boolValue = *((unsigned int *)value);
      sprintf(buf, "[\"event\", \"%s\", \"%s\", %d]", lemmaID, name, 0 == boolValue ? 0 : 1);
    break;
    default:
    break;
  }

  /* [TODO] memory management */
  return strdup(buf);
}

char * buildArrayEvent( char const * name, void * array, int size, int elemType )
{
#define ARRAY_BUILDER_BUFSIZE 128

  int *intArray = NULL;
  double *doubleArray = NULL;
  char **stringArray = NULL;
  char arrayString[ARRAY_BUILDER_BUFSIZE];
  char buf[ARRAY_BUILDER_BUFSIZE];
  int i;

  memset(arrayString, 0, ARRAY_BUILDER_BUFSIZE);
  memset(buf, 0, ARRAY_BUILDER_BUFSIZE);

  strcat(arrayString, "[");

  switch (elemType) {
    case nJSON_INT:
    case nJSON_BOOL:
      intArray = (int *)array;
      for (i = 0; i < size; i++) {
        sprintf(buf, "%d", intArray[i]);
        strcat(arrayString, buf);
        if (i < size-1) {
          strcat(arrayString, ", ");
        }
      }
    break;
    case nJSON_FLOAT:
    case nJSON_DOUBLE:
      doubleArray = (double *)array;
      for (i = 0; i < size; i++) {
        memset(buf, 0, ARRAY_BUILDER_BUFSIZE);
        fmtDouble(doubleArray[i], 6, buf, 256);
        strcat(arrayString, buf);
        if (i < size-1) {
          strcat(arrayString, ", ");
        }
      }
    break;
    case nJSON_STRING:
      stringArray = (char **)(array);
      for (i = 0; i < size; i++) {
        memset(buf, 0, ARRAY_BUILDER_BUFSIZE);
        sprintf(buf, "\"%s\"", stringArray[i]);
        strcat(arrayString, buf);
        if (i < size-1) {
          strcat(arrayString, ", ");
        }
      }
    break;
    default:
    break;
  }

  memset(buf, 0, ARRAY_BUILDER_BUFSIZE);
  sprintf(buf, "[\"event\", \"%s\", \"%s\",  %s]]", lemmaID, name, arrayString);

  return strdup(buf);
}


#ifdef __cplusplus
}
#endif


MessageBuilder::MessageBuilder( char const * id) :
  spallaId(id)
{
  /* for C code to read Lemma ID */
  lemmaID = (char *)id;
}


char * MessageBuilder::buildRegister( int port, char const** hears, int hearsSize, char const** plays, int playsSize, int heartbeat_period_sec) const
{
  char buf[1024];
  char *string;
  memset(buf, 0, 1024);

  sprintf(buf, "[\"register\", \"%s\", %d, ", lemmaID, port);

  strcat(buf, "[");
  for (int i = 0; i < hearsSize; i++) {
    strcat(buf, "\"");
    strcat(buf, hears[i]);
    strcat(buf, i < hearsSize-1 ? "\", " : "\"");
  }
  strcat(buf, "], ");

  strcat(buf, "[");
  for (int i = 0; i < playsSize; i++) {
    strcat(buf, "\"");
    strcat(buf, plays[i]);
    strcat(buf, i < playsSize-1 ? "\", " : "\"");
  }
  strcat(buf, "], ");

  strcat(buf, "\"spark\", ");
  strcat(buf, "\"0.2\"");
  if(heartbeat_period_sec > 0){
    strcat(buf, ", {\"heartbeat\":");
    char heartbeat_str[16];
    sprintf(heartbeat_str, "%d", heartbeat_period_sec);
    strcat(buf, heartbeat_str);
    strcat(buf, "}");
  }
  strcat(buf, "]");
  /* [TODO] memory management */
  string = strdup(buf);

  return string;
}

