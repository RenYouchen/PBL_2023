#ifndef CONFIG_H
#define CONFIG_H

#define S1 4
#define S2 5
#define S3 6
#define S4 7

#define SW1 36
#define SW2 37

#define Kp 1.5
#define Ki 0.001
#define Kd 1.1

#define tKp 1
#define tKi 0.05
#define tKd 0

#define gKp 1.5
#define gKi 0
#define gKd 0

#define ENA 13
#define ENB 8
#define IN1 12
#define IN2 11
#define IN3 10
#define IN4 9

#define INTERRUPT_PIN 2

#define TRIG 46
#define ECHO 47
#define MAX_DISTANCE 200

#define SERVO_PIN 4

#define LED 31

typedef struct {
	int8_t lspeed;
	int8_t rspeed;
} data;

#endif
