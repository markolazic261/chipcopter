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

#include <pic32mx.h>
#include <stdint.h>
#include <stdbool.h>
#include "gyro.h"

/* I2C Gyro Registers */
#define GYRO_ADDRESS 0x68
#define GYRO_CONFIG 0x1B
#define GYRO_POWER 0x6B
#define GYRO_XOUT_H 0x43


/* Gyro values */
double dx, dy, dz;

/* Gyro calibration */
double x_calib = 0;
double y_calib = 0;
double z_calib = 0;

void i2cIdle() {
	while (I2C1CON & 0x1F || I2C1STAT & (1 << 14));
}

bool i2cSend(uint8_t data) {
	i2cIdle();
	I2C1TRN = data;
	i2cIdle();
	return !(I2C1STAT & (1 << 15));
}

uint8_t i2cRecv() {
	i2cIdle();
	I2C1CONSET = 1 << 3;
	i2cIdle();
	I2C1STATCLR = 1 << 6;
	return I2C1RCV;
}

void i2cAck() {
	i2cIdle();
	I2C1CONCLR = 1 << 5;
	I2C1CONSET = 1 << 4;
}

void i2cNack() {
	i2cIdle();
	I2C1CONSET = 1 << 5;
	I2C1CONSET = 1 << 4;
}

void i2cStart() {
	i2cIdle();
	I2C1CONSET = 1 << 0;
	i2cIdle();
}

void i2cRestart() {
	i2cIdle();
	I2C1CONSET = 1 << 1;
	i2cIdle();
}

void i2cStop() {
	i2cIdle();
	I2C1CONSET = 1 << 2;
	i2cIdle();
}

void gyroBegin() {
	uint16_t temp;
	OSCCON = ~0x180000;
	OSCCON |= 0x080000;
	SPI2CON = 0;
	SPI2BRG = 4;
	SPI2STATCLR &= ~0x40;
	SPI2CON |= 0x60;
	SPI2CONSET = 0x8000;

	I2C1CON = 0x0;
	I2C1BRG = 0x0C2;
	I2C1STAT = 0x0;
	I2C1CONSET = 1 << 13;
	I2C1CONSET = 1 << 15;
	temp = I2C1RCV;

	/* Establishes comunication with the gyro in write mode (lowest bit 0), and spams it
	 * until it responds with an acknowledge condition.
	 */
	do {
		i2cStart();
	} while (!i2cSend(GYRO_ADDRESS << 1));
	i2cSend(GYRO_POWER);
	i2cSend(0x0);
	i2cStop();

	do {
		i2cStart();
	} while (!i2cSend(GYRO_ADDRESS << 1));
	i2cSend(GYRO_CONFIG);
	i2cSend(0x8); // Set angle range to +- 500dps
	i2cStop();
}

void gyroCalibration() {
	int i;
	for (i = 0; i < 2000; i++) {
		updateGyroValues();
		x_calib += dx;
		y_calib += dy;
		z_calib += dz;
	}
	x_calib /= 2000;
	y_calib /= 2000;
	z_calib /= 2000;
}

void updateGyroValues() {
	uint16_t x;
	uint16_t y;
	uint16_t z;

	do {
		i2cStart();
	} while (!i2cSend(GYRO_ADDRESS << 1));
	/* First register in a row of eight that we want to access */
	i2cSend(GYRO_XOUT_H);

	/* Switch to read mode (lowest bit 1) */
	do {
		i2cStart();
	} while (!i2cSend((GYRO_ADDRESS << 1) | 1));

	/* Get high/low x values */
	x = i2cRecv() << 8;
	i2cAck();
	x |= i2cRecv();
	i2cAck();

	/* Get high/low y values */
	y = i2cRecv() << 8;
	i2cAck();
	y |= i2cRecv();
	i2cAck();

	/* Get high/low z values */
	z = i2cRecv() << 8;
	i2cAck();
	z |= i2cRecv();
	i2cNack();

	/* Stop communication */
	i2cStop();

	int sx = (int16_t) x;
	int sy = (int16_t) y;
	int sz = (int16_t) z;

	/* Divide by constant to get degrees per second */
	double divConst = 65.5;
	dx = ((double) sx) / divConst;
	dy = ((double) sy) / divConst * -1;
	dz = ((double) sz) / divConst * -1;
}

double getXCalib() {
	return x_calib;
}

double getYCalib() {
	return y_calib;
}

double getZCalib() {
	return z_calib;
}

double getGyroX() {
	return dx;
}

double getGyroY() {
	return dy;
}

double getGyroZ() {
	return dz;
}
