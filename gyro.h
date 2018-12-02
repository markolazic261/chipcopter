/*
 * Gyro functionality via I2C.
 *
 * Heavily based on hello-temperature by h4xxel:
 * https://github.com/is1200-example-projects/hello-temperature
 *
 * @author Matas Kairaitis
 * @author Marko Lazic
 * @version 2016-12-12
 */

#ifndef HEADER_GYRO
#define HEADER_GYRO

#include <stdint.h>
#include <stdbool.h>

/*
 * Wait for I2C bus to become idle
 */
void i2cIdle();

/*
 * Send one byte on I2C bus, return ack/nack
 * status of transaction.
 */
bool i2cSend(uint8_t data);

/*
 * Receive one byte from I2C bus.
 */
uint8_t i2cRecv();

/*
 * Send acknowledge conditon on the bus.
 */
void i2cAck();

/*
 * Send not-acknowledge conditon on the bus.
 */
void i2cNack();

/*
 * Send start conditon on the bus.
 */
void i2cStart();

/*
 * Send restart conditon on the bus.
 */
void i2cRestart();

/*
 * Send stop conditon on the bus.
 */
void i2cStop();

/*
 * Initialize gyro registers and begin communication
 * via I2C.
 */
void gyroBegin();

/*
 * Calibrates the gyro for smoother inputs.
 */
void gyroCalibration();

/*
 * Updates the gyro x/y/z values in burst mode.
 */
void updateGyroValues();

/*
 * Getter for the gyro calibration X value.
 */
double getXCalib();

/*
 * Getter for the gyro calibration Y value.
 */
double getYCalib();

/*
 * Getter for the gyro calibration Z value.
 */
double getZCalib();

/*
 * Getter for the current X axis value.
 */
double getGyroX();

/*
 * Getter for the current Y axis value.
 */
double getGyroY();

/*
 * Getter for the current Z axis value.
 */
double getGyroZ();

#endif
