#include <unistd.h>

#define BMP180_ADDR 0xEE
#define BMP180_ID 0xD0
#define BMP180_ID_SIZE 1
#define BMP180_RDTEMP 0x2E
#define BMP180_CTRL 0xF4
#define BMP180_CAL_START_ADDR 0xAA
#define BMP180_AC1H 0xAA
#define BMP180_AC1L 0xAB
#define BMP180_AC2H 0xAC
#define BMP180_AC2L 0xAD
#define BMP180_AC3H 0xAE
#define BMP180_AC3L 0xAF
#define BMP180_AC4H 0xB0
#define BMP180_AC4L 0xB1
#define BMP180_AC5H 0xB2
#define BMP180_AC5L 0xB3
#define BMP180_AC6H 0xB4
#define BMP180_AC6L 0xB5
#define BMP180_B1H 0xB6
#define BMP180_B1L 0xB7
#define BMP180_B2H 0xB8
#define BMP180_B2L 0xB9
#define BMP180_MBH 0xBA
#define BMP180_MBL 0xBB
#define BMP180_MCH 0xBC
#define BMP180_MCL 0xBD
#define BMP180_MDH 0xBE
#define BMP180_MDL 0xBF
#define BMP180_CAL_MAX 22 

#pragma pack(1)
struct BMP180_CALIBRATION {
	int16_t AC1;
	int16_t AC2;
	int16_t AC3;
	uint16_t AC4;
	uint16_t AC5;
	uint16_t AC6;
	int16_t B1;
	int16_t B2;
	int16_t MB;
	int16_t MC;
	int16_t MD;
	int16_t B5;
};
#pragma options align=reset

struct BMP180_CONF {
	int16_t oss;
	struct BMP180_CALIBRATION *calib;
};
