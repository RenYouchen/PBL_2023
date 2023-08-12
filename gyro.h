#include <I2Cdev.h>
#include <Wire.h>
#include <MPU6050_6Axis_MotionApps20.h>

#include "config.h"

MPU6050 mpu;

bool blinkState = false;

Quaternion q;
VectorFloat gravity;
float ypr[3];

uint16_t packetSize;
uint8_t fifoBuffer[64];
bool mpuInterrupt;
int mpuIntStatus;

int dmpReady = false;

void dmpDataReady() {
  mpuInterrupt = true;
  dmpReady = true;
}

void setupGyro() {
  Wire.begin();
  Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties

  mpu.initialize();

  // verify connection
  Serial.println("Testing device connections...");
  Serial.println(mpu.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

  // load and configure the DMP
  Serial.println("Initializing DMP...");
  uint8_t devStatus = mpu.dmpInitialize();

  // supply your own gyro offsets here, scaled for min sensitivity
  mpu.setXGyroOffset(38);
  mpu.setYGyroOffset(3);
  mpu.setZGyroOffset(-11);
  mpu.setZAccelOffset(1205); // 1688 factory default for my test chip

  // make sure it worked (returns 0 if so)
  if (devStatus == 0) {
	mpu.CalibrateAccel(6);
	mpu.CalibrateGyro(6);
	mpu.PrintActiveOffsets();

	// turn on the DMP, now that it's ready
	Serial.println("Enabling DMP...");
	mpu.setDMPEnabled(true);

	// enable Arduino interrupt detection
	Serial.println("Enabling interrupt detection (Arduino external interrupt 2)...");
	attachInterrupt(INTERRUPT_PIN, dmpDataReady, RISING);
	mpuIntStatus = mpu.getIntStatus();

	// set our DMP Ready flag so the main loop() function knows it's okay to use it
	Serial.println("DMP ready! Waiting for first interrupt...");
	dmpReady = true;

	// get expected DMP packet size for later comparison
	packetSize = mpu.dmpGetFIFOPacketSize();
  } else {
	// ERROR!
	// 1 = initial memory load failed
	// 2 = DMP configuration updates failed
	// (if it's going to break, usually the code will be 1)
	Serial.print("DMP Initialization failed (code ");
	Serial.print(devStatus);
	Serial.println(")");
  }
}

int getYaw() {
	if (!dmpReady) return 0;
	if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer)) {
		mpu.dmpGetQuaternion(&q, fifoBuffer);
		mpu.dmpGetGravity(&gravity, &q);
		mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
		return ypr[0] * 180/M_PI;
	}
}
