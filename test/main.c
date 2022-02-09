#include <stdio.h>

#include "../inc/bmp180.h"
#include "../inc/temperature.h"
#include "../inc/pressure.h"

static struct BMP180_CALIBRATION calibration;

int
main(int argc, char *argv[])
{
	int errorcount;
	long UT;
	long T;
	long UP;
	long P;
	short oss;

	printf("Testing calculations\n");
	errorcount = 0;

	// Use calibration data from datasheet
	calibration.AC1 = 408;
	calibration.AC2 = -72;
	calibration.AC3 = -14383;
	calibration.AC4 = 32741;
	calibration.AC5 = 32757;
	calibration.AC6 = 23153;
	calibration.B1 = 6190;
	calibration.B2 = 4;
	calibration.MB = -32768;
	calibration.MC = -8711;
	calibration.MD = 2868;

	// Test temperature tools

	UT = 27898;

	T = bmp180_get_temperature(&calibration,UT);
	
	if ( T == 150 )
		printf("Temperature is correct: %ld\n",T);
	else
	{
		printf("Temperature is incorrect: %ld, difference of: %ld \n",T, T-150);
		errorcount++;
	}

	// Test Pressure tools
	
	UP = 23843;
	oss = 0;

	P = bmp180_calpressure(&calibration, UP,oss);

	if ( P == 69964 )
		printf("Pressure is correct: %ld\n",P);
	else
	{
		printf("Pressure is incorrect: %ld, difference of: %ld \n",P, P-69964);
		errorcount++;
	}

	return errorcount;
}
