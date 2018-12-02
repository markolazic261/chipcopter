/*
 * Quadcopter flight controller software.
 *
 * @author Matas Kairaitis
 * @author Marko Lazic
 * @version 2016-12-12
 */

#include <stdio.h>
#include <pic32mx.h>
#include "timer.h"
#include "gyro.h"
#include "display.h"
#include "pid.h"

unsigned int calibTime = 0;

/*
 * Represents a motor.
 */
typedef struct {
  int pulse;
  int turnOff;
} motor;

/*
 * Initializes the required I/O pins.
 */
void pinsInit() {
  TRISECLR = 0xF0;
  TRISESET = 0x02;
  PORTECLR = 2;
}

/*
 * Sets output of all motor pins to HIGH.
 */
void setMotorOutputHigh() {
  PORTESET = 0xF0;
}

/*
 * Sets output of all motor pins to LOW.
 */
void setMotorOutputLow() {
  PORTECLR = 0xF0;
}

/*
 * Pulses all motors with the given pulse.
 */
void pulseAllMotors(int pulse) {
  setMotorOutputHigh();
  delayMicroseconds(pulse);
  setMotorOutputLow();
}

/*
 * Sorts motor structs in ascending order.
 */
void sort(motor list[], int n) {
  int c, d;
  motor t;
  for (c = 0 ; c < ( n - 1 ); c++) {
    for (d = 0 ; d < n - c - 1; d++) {
      if (list[d].pulse > list[d + 1].pulse) {
        t = list[d];
        list[d] = list[d + 1];
        list[d + 1] = t;
      }
    }
  }
}

/*
 * Main loop of the program.
 */
void loop() {
  unsigned int startTime = getTime();

  updateGyroValues();
  pidCalculate(getGyroX() - getXCalib(), getGyroY() - getYCalib(), getGyroZ() - getZCalib());

  char buf1[32];
  char buf2[32];
  char buf3[32];
  char buf4[32];

  itoa(pidGetPulse1(), buf1, 10);
  itoa(pidGetPulse2(), buf2, 10);
  itoa(pidGetPulse3(), buf3, 10);
  itoa(pidGetPulse4(), buf4, 10);

  /*
  itoa(getGyroX(), buf1, 10);
  itoa(getGyroY(), buf2, 10);
  itoa(getGyroZ(), buf3, 10);
  itoa(getGyroX(), buf4, 10);

  */
  display_string(0, buf1);
  display_string(1, buf2);
  display_string(2, buf3);
  display_string(3, buf4);
  display_update();

  motor motor1 = {pidGetPulse1(), 1 << 4};
  motor motor2 = {pidGetPulse2(), 1 << 5};
  motor motor3 = {pidGetPulse3(), 1 << 6};
  motor motor4 = {pidGetPulse4(), 1 << 7};

  motor motors[4];
  motors[0] = motor1;
  motors[1] = motor2;
  motors[2] = motor3;
  motors[3] = motor4;

  sort(motors, 4);

  // Pulse motors
  if (getTime() < 5000000 + calibTime) {
    pulseAllMotors(2000);
  } else if (getTime() < 7000000 + calibTime) {
    pulseAllMotors(1000);
  } else {
    setMotorOutputHigh();
    delayMicroseconds(motors[0].pulse);
    PORTECLR = motors[0].turnOff;
    delayMicroseconds(motors[1].pulse - motors[0].pulse);
    PORTECLR = motors[1].turnOff;
    delayMicroseconds(motors[2].pulse - motors[1].pulse);
    PORTECLR = motors[2].turnOff;
    delayMicroseconds(motors[3].pulse - motors[2].pulse);
    PORTECLR = motors[3].turnOff;
  }

  unsigned int calculationTime = getTime() - startTime;
  delayMicroseconds(40000 - calculationTime);
}

/*
 * Entry point of the program.
 */
int main() {
  gyroBegin();
  pinsInit();
  timerBegin();
  display_init();
  asm volatile("ei");

  unsigned int calibStart = getTime();
  gyroCalibration();
  display_string(0, "START");
  display_update();
  calibTime = getTime() - calibStart;

  while (1) {
    loop();
  }
}
