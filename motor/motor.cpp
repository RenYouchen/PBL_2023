#include "motor.h"

Motor::Motor() {}

Motor::Motor(int INA, int INB) {
	this->INA = INA;
	this->INB = INB;
	pinMode(INA, OUTPUT);
	pinMode(INB, OUTPUT);
}

Motor::Motor(int ENX, int INA, int INB) {
	this->ENX = ENX;
	this->INA = INA;
	this->INB = INB;
	pinMode(ENX, OUTPUT);
	pinMode(INA, OUTPUT);
	pinMode(INB, OUTPUT);
}

void Motor::setSpeed(int speed) {
	this->speed = speed;
	writeSpeed(speed);
}

void Motor::forward() {
	digitalWrite(INA, HIGH);
	digitalWrite(INB, LOW);
}

void Motor::backward() {
	digitalWrite(INA, LOW);
	digitalWrite(INB, HIGH);
}

void Motor::writeSpeed(int _speed) {
	int flag = 1;
	if (_speed < 0) { 
		flag = 0; 
		_speed = -_speed;
	}
	_speed = _speed * 255 / 100;
	analogWrite(ENX, _speed);
	flag == 1 ? forward() : backward();
}
