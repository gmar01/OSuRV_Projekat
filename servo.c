#include "servo.h"

static servoData_ts servoData_as[SERVO_NUMBER] = { 0 };

static int openDriverFile()
{
	int servoDriverFile;
	servoDriverFile = open(SERVO_DRIVER, O_RDWR);
	
	if(servoDriverFile < 0){
		fprintf(stderr, "ERROR: \"%s\" not opened!\n", SERVO_DRIVER);
		fprintf(stderr, "fd = %d %s\n", servoDriverFile, strerror(-servoDriverFile));
	}

	return servoDriverFile;
}

void servo_init()
{
	int i = 0;
	for(i = 0; i < SERVO_NUMBER; i++)
	{
		servo_setServoDuty(i, SERVO_POS_MIN);
	}
}

void servo_loadServoData()
{
	
	int servoDriverFile = openDriverFile();
	
	uint16_t servoDuties[SERVO_NUMBER] = { 0 };
	int i = 0;
	int readFileOuput = 0;
	readFileOuput = read(servoDriverFile, (char*)&servoDuties, sizeof(servoDuties));
	if(readFileOuput != sizeof(servoDuties)){
			printf("%d bytes read", readFileOuput);
			fprintf(stderr, "ERROR: read went wrong!\n");
	}

	for(i = 0; i < SERVO_NUMBER; i++)
	{
		servoData_as[i].servoId_u8 = i;
		servoData_as[i].servoDuty_u16 = servoDuties[i];
	}
	
	close(servoDriverFile);
}

void servo_printServoStates()
{
	servo_loadServoData();
	
	int i = 0;
	for(i = 0; i < SERVO_NUMBER; i++)
	{
		printf("Servo %d = %d\n", servoData_as[i].servoId_u8, servoData_as[i].servoDuty_u16);
	}
}

void servo_setServoDuty(servoId_te servoId_u8, uint16_t servoDuty_u16)
{
	if(servoDuty_u16 <= SERVO_POS_MAX && servoDuty_u16 >= SERVO_POS_MIN)
	{
	
		servo_loadServoData();
		
		int servoDriverFile = openDriverFile();
		int writeFileOutput;
		int seekOutput;
		
		
		uint16_t duties[SERVO_NUMBER] = { 0 };
		
		int i = 0;
		for(i = 0; i < SERVO_NUMBER; i++)
		{
			duties[i] = servoData_as[i].servoDuty_u16;
		}
		
		duties[servoId_u8] = servoDuty_u16;
		
		(void)write(servoDriverFile, (char*)duties, sizeof(duties));
		
		close(servoDriverFile);
	}
	else
	{
		printf("Invalid range\n");
	}
}
