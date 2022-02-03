#include <math.h>
#include <unistd.h>
#include <stdint.h>


/*     get_altitude 
 * Takes two uint16_t with pressure in Pa
 * Returns a height in m
 */
double
get_altitude(uint32_t measured_pressure, uint32_t reference_pressure)
{
	double altitude;
	double X1;
	double X2;
	double MP;
	double RP;

	// Convert units to hPa
	MP = measured_pressure / 100;
	RP = reference_pressure / 100;

	// Use barometric formula
	X1 = MP/RP;
	X2 = 1/5.255;
	X1 = pow(X1,X2);
	altitude = 44330 * (1 - X1);

	return altitude;
}
