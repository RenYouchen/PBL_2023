#ifndef CONFIG_H
#define CONFIG_H

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

#define LED 31

typedef struct {
	int8_t lspeed;
	int8_t rspeed;
} data;

#endif
