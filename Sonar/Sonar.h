#ifndef _SONAR_H_ 
#define _SONAR_H_
#include "Arduino.h"

class Sonar {
	public:
		Sonar(int trigPin, int echoPin);
		void init();
		int getRaw();
		int getcm();
	private:
		int trigPin;
		int echoPin;
};
#endif
