/*
 * Timer 2 function implementations.
 *
 * @author Matas Kairaitis
 * @author Marko Lazic
 * @version 2016-12-12
 */

#include <pic32mx.h>
#include "timer.h"
#include "pid.h"

unsigned int timeElapsed = 0;
unsigned int throttleStart = 0;

/*
 * Timer 2 Interrupt Service Routine
 */
void user_isr(void) {

  IFS(0) = 0;
  timeElapsed += 5;

  if ((PORTE & 2) == 2 && throttleStart == 0) {
    // Start clounting pulse
    throttleStart = timeElapsed;
  } else if ((PORTE & 2) != 2 && throttleStart > 0) {
    // Pulse is over, set throttle to input pulse
    setThrottle(timeElapsed - throttleStart);
    throttleStart = 0;
  }

}

void timerBegin() {
  T2CON = 0x0;
  TMR2 = 0x0;
  PR2 = 80 * 5;
  T2CONSET = 0x8000;

  volatile int* iec = 0xbf881060;
  volatile int* ipc = 0xbf8810b0;
  *iec = *iec | 0x100;
  *ipc = *ipc | 0x1c;
  *ipc = *ipc | 0x3;
}

unsigned int getTime() {
  return timeElapsed;
}

void delayMicroseconds(unsigned int delay) {
  unsigned int startTime = timeElapsed;
  while (timeElapsed < startTime + delay);
}