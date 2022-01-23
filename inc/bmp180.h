#define BMP180_ADDR 0xEE
#define BMP180_ID 0xD0
#define BMP180_CTRL 0xF4

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
