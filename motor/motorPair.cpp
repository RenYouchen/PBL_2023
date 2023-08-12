#include "motorPair.h"

MotorPair::MotorPair(Motor leftMotor, Motor rightMotor) {
	this->_leftMotor = leftMotor;
	this->_rightMotor = rightMotor;
};

void MotorPair::setSpeed(int speedLeft, int speedRight) {
	this->_leftMotor.setSpeed(speedLeft);
	this->_rightMotor.setSpeed(speedRight);
}

void MotorPair::arcadeDrive(int speed, int rotation) {
	this->_leftMotor.setSpeed(speed - rotation);
	this->_rightMotor.setSpeed(speed + rotation);
}

