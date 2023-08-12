#ifndef _MOTOR_H_
#define _MOTOR_H_
#include "Arduino.h"
class Motor {
	public:
		Motor();
		Motor(int INA, int INB);
		Motor(int ENX, int INA, int INB);
		void forward();
		void backward();
		void stop();
		void setSpeed(int speed); // 0~100
	private:
		int ENX;
		int INA;
		int INB;
		int speed;
		void writeSpeed(int _speed); //speed to pwm
};

#endif
