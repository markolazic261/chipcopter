/*
 * PID calculation function implementations.
 *
 * @author Matas Kairaitis
 * @author Marko Lazic
 * @version 2016-12-12
 */

#include "pid.h"

float pid_p_gain_roll = 1.3;
float pid_i_gain_roll = 0.05;
float pid_d_gain_roll = 15.0;
int pid_max_roll = 400;

float pid_p_gain_pitch = 1.3;
float pid_i_gain_pitch = 0.05;
float pid_d_gain_pitch = 15.0;
int pid_max_pitch = 400;

float pid_p_gain_yaw = 4.0;
float pid_i_gain_yaw = 0.02;
float pid_d_gain_yaw = 0.0;
int pid_max_yaw = 400;

float gyro_roll_input = 0;
float gyro_yaw_input = 0;
float gyro_pitch_input = 0;

float pid_error_temp;
float pid_i_mem_roll, pid_roll_setpoint, pid_output_roll, pid_last_roll_d_error;
float pid_i_mem_pitch, pid_pitch_setpoint, pid_output_pitch, pid_last_pitch_d_error;
float pid_i_mem_yaw, pid_yaw_setpoint, pid_output_yaw, pid_last_yaw_d_error;

int motorPulse1, motorPulse2, motorPulse3, motorPulse4;

int throttle = 1000;
int centerPos = 0;

void pidCalculate(double gyroX, double gyroY, double gyroZ) {
	pidSetGyroValues(gyroX, gyroY, gyroZ);
	pidCalculateRoll();
	pidCalculateYaw();
	pidCalculatePitch();
	pidCalculatePulses();
}

void pidSetGyroValues(double gyroX, double gyroY, double gyroZ) {
	gyro_roll_input =  (gyro_roll_input * 0.7)  +  (gyroX * 0.3);
	gyro_yaw_input =  (gyro_yaw_input * 0.7)  +  (gyroZ * 0.3);
	gyro_pitch_input =  (gyro_pitch_input * 0.7) +  (gyroY * 0.3);
}

void pidCalculateRoll() {
	pid_error_temp = gyro_roll_input - centerPos; // PID PE
	pid_i_mem_roll += pid_i_gain_roll * pid_error_temp; // I, cumulative error

	// Reduce the severity of the correction (avoid flips)
	if (pid_i_mem_roll > pid_max_roll) {
		pid_i_mem_roll = pid_max_roll;
	} else if (pid_i_mem_roll < pid_max_roll * -1) {
		pid_i_mem_roll = pid_max_roll * -1;
	}

	int p = pid_error_temp * pid_p_gain_roll; // Calculate P component
	int i = pid_i_mem_roll; // Calculate I component
	int d = pid_d_gain_roll * (pid_error_temp - pid_last_roll_d_error); // Calculate D component
	pid_output_roll = p + i + d;

	// Limit the roll to the max/min value
	if (pid_output_roll > pid_max_roll) {
		pid_output_roll = pid_max_roll;
	} else if (pid_output_roll < pid_max_roll * -1) {
		pid_output_roll = pid_max_roll * -1;
	}

	pid_last_roll_d_error = pid_error_temp;
}

void pidCalculateYaw() {
	pid_error_temp = gyro_yaw_input - centerPos; // PID PE
	pid_i_mem_yaw += pid_i_gain_yaw * pid_error_temp; // I, cumulative error

	// Reduce the severity of the correction (avoid spins)
	if (pid_i_mem_yaw > pid_max_yaw) {
		pid_i_mem_yaw = pid_max_yaw;
	} else if (pid_i_mem_yaw < pid_max_yaw * -1) {
		pid_i_mem_yaw = pid_max_yaw * -1;
	}

	int p = pid_error_temp * pid_p_gain_yaw; // Calculate P component
	int i = pid_i_mem_yaw; // Calculate I component
	int d = pid_d_gain_yaw * (pid_error_temp - pid_last_yaw_d_error); // Calculate D component
	pid_output_yaw = (p + i + d);

	// Limit the yaw to the max/min value
	if (pid_output_yaw > pid_max_yaw) {
		pid_output_yaw = pid_max_yaw;
	} else if (pid_output_yaw < pid_max_yaw * -1) {
		pid_output_yaw = pid_max_yaw * -1;
	}

	pid_last_yaw_d_error = pid_error_temp;
}

void pidCalculatePitch() {
	pid_error_temp = gyro_pitch_input - centerPos; // PID PE
	pid_i_mem_pitch += pid_i_gain_pitch * pid_error_temp; // I, cumulative error

	// Reduce the severity of the correction (avoid spins)
	if (pid_i_mem_pitch > pid_max_pitch) {
		pid_i_mem_pitch = pid_max_pitch;
	} else if (pid_i_mem_pitch < pid_max_pitch * -1) {
		pid_i_mem_pitch = pid_max_pitch * -1;
	}

	int p = pid_error_temp * pid_p_gain_pitch; // Calculate P component
	int i = pid_i_mem_pitch; // Calculate I component
	int d = pid_d_gain_pitch * (pid_error_temp - pid_last_pitch_d_error); // Calculate D component
	pid_output_pitch = (p + i + d);

	// Limit the yaw to the max/min value
	if (pid_output_pitch > pid_max_pitch) {
		pid_output_pitch = pid_max_pitch;
	} else if (pid_output_pitch < pid_max_pitch * -1) {
		pid_output_pitch = pid_max_pitch * -1;
	}

	pid_last_pitch_d_error = pid_error_temp;
}

void pidCalculatePulses() {
	motorPulse1 = throttle - pid_output_pitch + pid_output_roll - pid_output_yaw; // Front right
	motorPulse2 = throttle + pid_output_pitch + pid_output_roll + pid_output_yaw; // Rear right
	motorPulse3 = throttle + pid_output_pitch - pid_output_roll - pid_output_yaw; // Rear Left
	motorPulse4 = throttle - pid_output_pitch - pid_output_roll + pid_output_yaw; // Front left

	if (motorPulse1 < 1100) motorPulse1 = 1100;
	if (motorPulse2 < 1100) motorPulse2 = 1100;
	if (motorPulse3 < 1100) motorPulse3 = 1100;
	if (motorPulse4 < 1100) motorPulse4 = 1100;

	if (motorPulse1 > 2000) motorPulse1 = 2000;
	if (motorPulse2 > 2000) motorPulse2 = 2000;
	if (motorPulse3 > 2000) motorPulse3 = 2000;
	if (motorPulse4 > 2000) motorPulse4 = 2000;

	if (throttle < 1050) {
		motorPulse1 = 1000;
		motorPulse2 = 1000;
		motorPulse3 = 1000;
		motorPulse4 = 1000;
	}
}

void setThrottle(int th) {
	throttle = th;
}

int getThrottle() {
	return throttle;
}

int pidGetPulse1() {
	return motorPulse1;
}

int pidGetPulse2() {
	return motorPulse2;
}

int pidGetPulse3() {
	return motorPulse3;
}

int pidGetPulse4() {
	return motorPulse4;
}
