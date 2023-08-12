#include "Sonar.h"

Sonar::Sonar(int trigPin, int echoPin) {
	this->trigPin = trigPin;
	this->echoPin = echoPin;
}

void Sonar::init() {
	pinMode(trigPin, OUTPUT);
	pinMode(echoPin, INPUT);
}

int Sonar::getRaw() {
	digitalWrite(trigPin, LOW);
	delayMicroseconds(2);
	digitalWrite(trigPin, HIGH);
	delayMicroseconds(10);
	digitalWrite(trigPin, LOW);
	return pulseIn(echoPin, HIGH);
}

int Sonar::getcm() {
	return getRaw() / 2 / 29.1;
}
