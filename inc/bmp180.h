#define BMP180_ADDR 0xEE
#define BMP180_ID 0xD0
#define BMP180_ID_SIZE 1
#define BMP180_RDTEMP 0x2E
#define BMP180_CTRL 0xF4
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

struct BMP180_CALIBRATION {
	short AC1;
	short AC2;
	short AC3;
	unsigned short AC4;
	unsigned short AC5;
	unsigned short AC6;
	short B1;
	short B2;
	short B5;
	short MB;
	short MC;
	short MD;
};

struct BMP180_CONF {
	short oss;
	struct BMP180_CALIBRATION *calib;
};
