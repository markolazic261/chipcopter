/*
 * Timer 2 function definitions.
 *
 * @author Matas Kairaitis
 * @author Marko Lazic
 * @version 2016-12-12
 */

#ifndef HEADER_TIMER
#define HEADER_TIMER

/*
 * Initializes timer 2 registers and starts the timer.
 */
void timerBegin();

/*
 * Getter for the current time elapsed value since
 * power on in microseconds.
 */
unsigned int getTime();

/*
 * Pauses execution for the given number of microseconds.
 */
void delayMicroseconds(unsigned int delay);

#endif