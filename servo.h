#ifndef SERVO_H
#define SERVO_H

#include <stdint.h>
#include <stdint.h> // uint16_t and family
#include <stdio.h> // printf and family
#include <string.h> // strerror()
#include <unistd.h> // file ops
#include <fcntl.h> // open() flags
#include <errno.h>

#define SERVO_NUMBER 4
#define SERVO_DRIVER "/dev/servo_ctrl"

#define SERVO_POS_MAX 125
#define SERVO_POS_MIN 25

typedef struct servoData
{
	uint8_t servoId_u8;
	uint16_t servoDuty_u16;
} servoData_ts;

typedef enum servoId
{
	servo0,
	servo1,
	servo2,
	servo3
} servoId_te;

extern void servo_init();
extern void servo_loadServoData();
extern void servo_printServoStates();
extern void servo_setServoDuty(servoId_te servoId_u8, uint16_t servoDuty_u16);

#endif
