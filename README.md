# Chipcopter

This is the source code of the flight controller software for a ChipKit Uno32, that we used in our drone project for the IS1500 Computer Science & Components course at the Royal Institute of Technology in Stockholm, Sweden.

![Quadcopter](https://github.com/matys18/chipcopter/blob/master/quad.jpg)

### Details

The drone receives input from the gyro (see [gyro.c](https://github.com/matys18/chipcopter/blob/master/gyro.c)) and the controller (see [timer.c](https://github.com/matys18/chipcopter/blob/master/timer.c)) and uses the PID control algorithm to balance the quadcopter (see [pid.c](https://github.com/matys18/chipcopter/blob/master/pid.c)).

If you have any questions, open an issue or email us at matas.kairaitis@gmail.com 

### Structure

The code is based on the [project-template](https://github.com/is1200-example-projects/project-template). The template contains the following source files:

main.c:
	Contains the entry point main()

vectors.S:
	Contains the interrupt handler and the exception handler

stubs.c:
	Contains stubs for functions run during micro controller
	initialization
