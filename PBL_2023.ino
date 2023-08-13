#include <motor.h>
#include <motorPair.h>
#include <Sonar.h>
#include <PID_v1.h>

#include "config.h"

#include "utils.h"
#include "gyro.h"
#include "pixy.h"
#include "stepMotor.h"

//PID
double Setpoint, Input, Output;
PID pid(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

Motor leftMotor(ENA, IN1, IN2);
Motor rightMotor(ENB, IN3, IN4);
MotorPair car(rightMotor, leftMotor);
Sonar sonar(TRIG, ECHO);

data d;

void turn(int angle);
void sonarFunc();
void debugYaw();
void turnToRedball();

void setup() {
	Serial.begin(115200);
	
	car.setSpeed(0,0);
	sonar.init();
	pinMode(LED, OUTPUT);
	initPixy();
	setupGyro();
	Setpoint = 0;
	pid.SetMode(AUTOMATIC);
	pid.SetOutputLimits(-25, 25);
}

void loop() {
	debugYaw();
	//sonarFunc();
	turnToRedball();	
}

void turnToRedball() {
	if (getBlocks()) {
		Setpoint = 0;
		Input = xDiff(); 
		pid.Compute();
		car.arcadeDrive(25, Output);
	} else {
		car.setSpeed(0,0);
		Input = 0;
		pid.Compute();
	}
}

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
	if(distance <= 35 && distance > 1) {
		Serial.println();
		Serial.println(distance);
		Serial.println("turning");
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

