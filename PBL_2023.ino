#include <motor.h>
#include <motorPair.h>
#include <Sonar.h>
#include <PID_v1.h>
#include <Servo.h>

#include "config.h"

#include "utils.h"
#include "gyro.h"
#include "pixy.h"
#include "stepMotor.h"

//PID
double Setpoint, Input, Output;
double ySetpoint, yInput, yOutput;
PID pid(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);
PID turnPid(&Input, &Output, &Setpoint, tKp, tKi, tKd, DIRECT);
PID goPid(&yInput, &yOutput, &ySetpoint, gKp, gKi, gKd, DIRECT); 

//obj define
Motor leftMotor(ENA, IN1, IN2);
Motor rightMotor(ENB, IN3, IN4);
MotorPair car(rightMotor, leftMotor);
Sonar sonar(TRIG, ECHO);
Servo servo;

//speed (maybe useless)
data di;

//function define
void catchBall();
void turn(int angle);
void sonarFunc();
int sonarDistance();
void debugYaw();
void turnToRedball();
int hitTheWall();

void setup() {
	Serial.begin(115200);
	car.setSpeed(0,0);
	sonar.init();
	pinMode(LED, OUTPUT);
	pinMode(SW1,INPUT_PULLUP);
	pinMode(SW2,INPUT_PULLUP);
	initPixy();
	setupGyro();
	Setpoint = 0;
	pid.SetMode(AUTOMATIC);
	pid.SetOutputLimits(-25, 25);
	turnPid.SetMode(AUTOMATIC);
	turnPid.SetOutputLimits(-25, 25);
	goPid.SetMode(AUTOMATIC);
	goPid.SetOutputLimits(-25, 25);
	//setup servo
	servo.attach(SERVO_PIN);
	servo.write(0);
	delay(1000);
	//led will be high when ready
	digitalWrite(LED, HIGH);
}

void loop() {
	//catchBall();
	//debugYaw();
	//Setpoint = 0;
	//Input = getYaw();
	//pid.Compute();
	//car.arcadeDrive(25, Output);
	//Serial.println(getYaw());
	//Serial.println(sonarDistance());
	turnToRedball();	
}

void catchBall() {
	int angle = 130;
	int dtime = 3000;
	//will do for 6s
	servo.write(angle);
	delay(dtime);
	servo.write(50);
	delay(dtime);
}

void throwBall() {
	servo.write(0);
	delay(3000);
}

int goStraight = 0;
int ballNotInRangeX = 1;
int count = 0;

void turnToRedball() {
	if (getBlocks() && ballNotInRangeX && 0) {
		Serial.println("adjust x");
		debugYaw();
		goStraight = 0;
		Setpoint = 0;
		Input = xDiff();
		if (Input >= -10 && Input <= 10) {
			count++;
			if (count > 25) {
				ballNotInRangeX = 0;
				count = 0;
			}
		} else {
			count = 0;
		}
		turnPid.Compute();
		car.arcadeDrive(0, Output);
	} else if (!ballNotInRangeX) { 
		Serial.println("gotoball");
		Serial.println("adjust y");
		if (!goStraight) {
			car.setSpeed(0,0);
			setupGyro();
			goStraight = 1;
		}

		if (yInput >= -5 && yInput <= 5) {
			count++;
			if (count > 25) {
				Serial.println("catch");
				car.setSpeed(0,0);
				count = 0;
				if (!(xDiff() >= -10 && xDiff() <= 10)) {
					ballNotInRangeX = 1;
				} else {
					catchBall();
					delay(1000);
					while (true) {
						while (1) {
							
						}
						car.setSpeed(-25,-25);
						if (hitTheWall()) {
							car.setSpeed(0,0);
							throwBall();
							break;
						}
					}
				}
			}
		}

		ySetpoint = 0;
		yInput = yDiff();
		goPid.Compute();

		Setpoint = 0;
		Input = getYaw();
		pid.Compute();
		
		debugYaw();

		car.arcadeDrive(yOutput, Output);
	} else {
		ballNotInRangeX = 1;
		Serial.println("not found");
		if (!goStraight) {
			car.setSpeed(0,0);
			setupGyro();
			goStraight = 1;
		}
		while (sonarDistance() < 20) {
			Serial.println("turning");
			car.arcadeDrive(0, -25);
			goStraight = 0;
		}
		Setpoint = 0;
		Input = getYaw();
		pid.Compute();
		car.arcadeDrive(50, -Output);
		//car.setSpeed(0,0);
	}
}

bool turned = false;

void turn(int angle) {
	bool done = false;
	int count = 0;
	car.setSpeed(0,0);
	setupGyro();
	while (!done) {
		Setpoint = angle;
		Input = getYaw();
		//Input = angleDiff(Setpoint, getYaw()); 
		//turnPid.Compute();
		pid.Compute();
		debugYaw();
		car.arcadeDrive(0, Output);
		if (Input >= angle - 5 && Input <= angle + 5) {
			count++;
		} else {
			count = 0;
		}
		if (count > 30) {
			done = true;
			Serial.println("done");
			car.setSpeed(0,0);
			turned = true;
			break;
		}
	}
}

void sonarFunc() {
	delay(50);
	int distance = sonar.getcm();
	if(distance <= 35 && distance > 1) {
		
	} else {

	}
}

void debugYaw() {
	Serial.print(Setpoint);
	Serial.print(" ");
	Serial.print(Output);
	Serial.print(" ");
	Serial.print(Input);
	Serial.print("    ");
	Serial.print(ySetpoint);
	Serial.print(" ");
	Serial.print(yOutput);
	Serial.print(" ");
	Serial.print(yInput);
	Serial.println();
}

int hitTheWall() {
	if ((digitalRead(SW1) == LOW) || (digitalRead(SW2) == LOW)) {
		return 1;
	} else {
		return 0;
	}
}

int sonarDistance() {
	int a = sonar.getcm();
	if (a < 1 || a > 21) {
		return 21;
	} else {
		return a;
	}
}
