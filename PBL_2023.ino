#include <motor.h>
#include <motorPair.h>
#include <Sonar.h>
#include <PID_v1.h>

#include "config.h"

#include "utils.h"
#include "gyro.h"

//PID
double Setpoint, Input, Output;
double Kp=2,Ki=0.2, Kd=0.1;
//double Kp=0.8,Ki=0.02, Kd=0.3;
PID pid(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

Motor leftMotor(ENA, IN1, IN2);
Motor rightMotor(ENB, IN3, IN4);
MotorPair car(rightMotor, leftMotor);
Sonar sonar(TRIG, ECHO);

int yawoffset = 0;

data d;

void turn(int angle);
void sonarFunc();
void debugYaw();
void turnBack();

void setup() {
	Serial.begin(115200);
	car.setSpeed(0,0);
	sonar.init();
	pinMode(LED, OUTPUT);
	setupGyro();
	Setpoint = 0;
	pid.SetMode(AUTOMATIC);
	pid.SetOutputLimits(-50, 50);
}
//void loop() {
	//Input = getYaw();
	//pid.Compute();
	//car.arcadeDrive(50, -Output);
	//car.setSpeed(100,100);
	//sonarFunc();
//}

bool turned = false;

void turn(int angle) {
	bool done = false;
	int count = 0;
	car.setSpeed(0,0);
	setupGyro();
	while (!done) {
		delay(10);
		Setpoint = angle;
		Input = getYaw();
		//Input = angleDiff(Setpoint, getYaw()); 
		pid.Compute();
		debugYaw();
		car.arcadeDrive(0, -Output);
		if (Input >= angle - 5 && Input <= angle + 5) {
			count++;
		}
		if (count > 50) {
			done = true;
			Serial.println("done");
			car.setSpeed(0,0);
			turned = true;
		}
	}
}

void sonarFunc() {
	delay(50);
	int distance = sonar.getcm();
	if(distance <= 35) {
		Serial.println(distance);
		Serial.println("turning back");
		Serial.println("sonar");
		digitalWrite(LED, HIGH);
		turn(90);
		//turnBack();
	} else {
		Serial.print("forward :");
		digitalWrite(LED, LOW);
		if (turned) {
			car.setSpeed(0,0);
			setupGyro();
			turned = false;
		}
		
		Setpoint = 0;
		Input = getYaw();
		pid.Compute();
		car.arcadeDrive(50, -Output);
	}
}

void debugYaw() {
	Serial.print(Setpoint);
	Serial.print(" ");
	Serial.print(Output);
	Serial.print(" ");
	Serial.print(Input);
	Serial.print(" ");
	//Serial.print(sonar.getcm());
	Serial.println();
}

void turnBack() {
	Serial.println("turning back");
	turn(90);
	delay(10);
	turn(90);
}

void loop() {
	//debugYaw();
	sonarFunc();
}
