#include <stdint.h> // uint16_t and family
#include <stdio.h> // printf and family
#include <stdlib.h>
#include <string.h> // strerror()
#include <unistd.h> // file ops
#include <fcntl.h> // open() flags
#include "servo.h" //driver control functions
#include <pthread.h>

#define SERVO_STEPS	 				20	//steps from min to max
#define DELAY_BETWEEN_STEPS			1	//time between each motor step	
#define PHASE_SHIFT_BETWEEN_SERVOS	5	//phase shift between motors defined in steps when each step is 5% of the PWM signal	

static pthread_t servoCtrlThread[SERVO_NUMBER];
static pthread_mutex_t servoStepMutex;	//mutex to sync every servo step

static uint8_t motorPosition_au8[SERVO_NUMBER] = { 0 };
static uint16_t duties[SERVO_NUMBER] = { 0 };

static void stepServo(servoId_te serviId_e);

void servoCtrlFunction(servoId_te servoId_e)
{
	uint8_t servoStep_u8 = 0;
	
	switch(servoId_e)
	{
		case servo0:
		{
			for(servoStep_u8 = 0; servoStep_u8 < SERVO_STEPS; servoStep_u8++)
			{
				stepServo(servoId_e);
			}
			break;
		}
		case servo1:
		case servo2:
		case servo3:
		{
			while(motorPosition_au8[servoId_e - 1] < PHASE_SHIFT_BETWEEN_SERVOS) //wait for phase condiiton to be true
			{
				//wait here
			}
			
			for(servoStep_u8 = 0; servoStep_u8 < SERVO_STEPS; servoStep_u8++)
			{
				stepServo(servoId_e);
			}
			break;
		}
		default:
		{
			// should not occur
		}
	}
}

int main()
{
	
		int i = 0;
		
		/* Initialize servo duty cycles to minimal value */
		servo_init();
		
		servo_printServoStates();
		
		servoId_te servo_ae[SERVO_NUMBER] = { servo0, servo1, servo2, servo3 };
		
		pthread_mutex_init(&servoStepMutex, NULL);
		for(i = 0; i < SERVO_NUMBER; i++)
		{
			int threadCreateOutput = pthread_create(&servoCtrlThread[i], NULL, (void *)servoCtrlFunction, (int *)servo_ae[i]);
			if (threadCreateOutput != 0)
			{
				printf("Error in thread creation.\n");
				exit(1);
			}
		}
		
		getchar();
		pthread_mutex_destroy(&servoStepMutex);
	
	return 0;
}

static void stepServo(servoId_te servoId_e)
{
	uint8_t servoDeltaStep_u8 = (SERVO_POS_MAX - SERVO_POS_MIN) / SERVO_STEPS;
	
	pthread_mutex_lock(&servoStepMutex);
	//enter critical section
	servo_setServoDuty(servoId_e, SERVO_POS_MIN + servoDeltaStep_u8 * motorPosition_au8[servoId_e]);
	motorPosition_au8[servoId_e] += 1;
	//exit critical section
	pthread_mutex_unlock(&servoStepMutex);
	sleep(DELAY_BETWEEN_STEPS);
	
	printf("Servo %d set to %d\n", servoId_e, motorPosition_au8[servoId_e] - 1);
	
}




