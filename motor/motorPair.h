#ifndef _MOTORPAIR_H_
#define _MOTORPAIR_H_

#include "motor.h"

class MotorPair {
	public:
		MotorPair(Motor leftMotor, Motor rightMotor);
		void setSpeed(int speedLeft, int speedRight);
		void arcadeDrive(int speed, int rotation);
	private:
		Motor _leftMotor;
		Motor _rightMotor;
};

#endif
