/*

 This Arduino project is a demonstration of the Noam platform's Arduino Lemma Library (Ethernet).
 This project was developed on an Arduino Mega 2560 with Ethernet shield.
 When the Arduino is connected to a Noam network, pushing the buttong will send a signal that is echoed and causes the LED to light up while the button is pressed.

 When developing Arduino for Noam, it is important to use an event-centric paradigm.  Using software debounce ensures messages are only sent when the button state changes.
 Keeping delays out of the main loop will ensure fast message response, as new messages are polled in the loop.

 Author: Evan Shapiro [ IDEO ]

 */

#include "application.h"

#include "CIID_Comps/Buttons_Core.cpp"
 