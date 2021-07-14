//Code to make a megapi controllable by bluetooth
#include <Arduino.h>
#include "MeMegaPiPro.h"

#define MOTOR_SPEED 256

MeMegaPiDCMotor motor1A(PORT1A);
MeMegaPiDCMotor motor1B(PORT1B);
MeMegaPiDCMotor motor2A(PORT2A);
MeMegaPiDCMotor motor2B(PORT2B);
char instruct;

void setup() {
	Serial3.begin(9600);
	Serial.print("All systems green!\n");
}

void loop() {
	//Execute commands from bluetooth
	//Read input from bluetooth
	if (Serial3.available()) {
		instruct = Serial3.read();
		Serial.print(instruct + "\n");
		Serial.print("Test\n");
	}
	//Match up char to action

	//Forward
	if (instruct == 'f') {
		motor1A.run(MOTOR_SPEED);
		motor1B.run(MOTOR_SPEED);
		motor2A.run(-MOTOR_SPEED);
		motor2B.run(-MOTOR_SPEED);
	}
	//Reverse
	else if (instruct == 'b') {
		motor1A.run(-MOTOR_SPEED);
		motor1B.run(-MOTOR_SPEED);
		motor2A.run(MOTOR_SPEED);
		motor2B.run(MOTOR_SPEED);
	}
	//right
	else if (instruct == 'r') {
		motor1A.run(MOTOR_SPEED);
		motor1B.run(MOTOR_SPEED);
		motor2A.run(MOTOR_SPEED);
		motor2B.run(MOTOR_SPEED);
	}
	//left
	else if (instruct == 'l') {
		motor1A.run(-MOTOR_SPEED);
		motor1B.run(-MOTOR_SPEED);
		motor2A.run(-MOTOR_SPEED);
		motor2B.run(-MOTOR_SPEED);
	}
	//Stop
	else if (instruct == 's') {
		motor1A.run(0);
		motor1B.run(0);
		motor2A.run(0);
		motor2B.run(0);
	}

}