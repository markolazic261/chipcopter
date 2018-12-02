/*
 * PID calculation function definitions.
 *
 * @author Matas Kairaitis
 * @author Marko Lazic
 * @version 2016-12-12
 */

#ifndef HEADER_PID
#define HEADER_PID

/*
 * Calculates the pulses for all motors
 * based on the given gyro values.
 */
void pidCalculate(double gyroX, double gyroY, double gyroZ);

/*
 * Sets the gyro input values.
 */
void pidSetGyroValues(double gyroX, double gyroY, double gyroZ);

/*
 * Calculates the roll using PID.
 */
void pidCalculateRoll();

/*
 * Calculates the yaw using PID.
 */
void pidCalculateYaw();

/*
 * Calculates the pitch using PID.
 */
void pidCalculatePitch();

/*
 * Calculates the pulses for all of the motors.
 */
void pidCalculatePulses();

/*
 * Getter for the calculated pulse of motor 1.
 */
int pidGetPulse1();

/*
 * Getter for the calculated pulse of motor 2.
 */
int pidGetPulse2();

/*
 * Getter for the calculated pulse of motor 3.
 */
int pidGetPulse3();

/*
 * Getter for the calculated pulse of motor 4.
 */
int pidGetPulse4();

/*
 * Setter for the base throttle value.
 */
void setThrottle(int th);

/*
 * Getter for the base throttle value.
 */
int getThrottle();

#endif