#include <cstdint>

#include "Ev3i2c.hpp"

#ifndef EV3_ASSERVDRIVER_AMS_AS5048B_H_
#define EV3_ASSERVDRIVER_AMS_AS5048B_H_

// Address depending on the two DIL switches
#define AS5048B_ADDR(a1,a2)  (uint8_t)(0x40 | ( !a1 ? 0x2 : 0 ) | ( !a2 ? 0x1 : 0 ))

// Default addresses for AS5048B
#define AS5048_ADDRESS 0x40 // 0b10000 + ( A1 & A2 to ON (GND))
#define AS5048B_PROG_REG 0x03
#define AS5048B_ADDR_REG 0x15
#define AS5048B_ZEROMSB_REG 0x16 //bits 0..7
#define AS5048B_ZEROLSB_REG 0x17 //bits 0..5
#define AS5048B_GAIN_REG 0xFA
#define AS5048B_DIAG_REG 0xFB
#define AS5048B_MAGNMSB_REG 0xFC //bits 0..7
#define AS5048B_MAGNLSB_REG 0xFD //bits 0..5
#define AS5048B_ANGLMSB_REG 0xFE //bits 0..7
#define AS5048B_ANGLLSB_REG 0xFF //bits 0..5
#define AS5048B_RESOLUTION 16384.0 //14 bits

// Moving Exponential Average on angle - beware heavy calculation for some Arduino boards
// This is a 1st order low pass filter
// Moving average is calculated on Sine et Cosine values of the angle to provide an extrapolated accurate angle value.
#define EXP_MOVAVG_N 5	//history length impact on moving average impact - keep in mind the moving average will be impacted by the measurement frequency too
#define EXP_MOVAVG_LOOP 1 //number of measurements before starting mobile Average - starting with a simple average - 1 allows a quick start. Value must be 1 minimum

//unit consts - just to make the units more readable
#define U_RAW 1
#define U_TRN 2
#define U_DEG 3
#define U_RAD 4
#define U_GRAD 5
#define U_MOA 6
#define U_SOA 7
#define U_MILNATO 8
#define U_MILSE 9
#define U_MILRU 10

class AMS_AS5048B {
public:
    AMS_AS5048B(uint8_t chipAddress, uint8_t i2cNum = 1);
    ~AMS_AS5048B() {
    }
    bool connected();

    int ping();

    void begin(void); // to init the object, must be called in the setup loop
    void reset(void);
    void toggleDebug(void); // start / stop debug through serial at anytime
    void setClockWise(bool cw = true); //set clockwise counting, default is false (native sensor)
    void progRegister(uint8_t regVal); //nothing so far - manipulate the OTP register
    void doProg(void); //progress programming slave address OTP
    void doProgZero(void); //progress programming zero position OTP
    void addressRegW(uint8_t regVal); //change the chip address
    uint8_t addressRegR(void); //read chip address
    void setZeroReg(void); //set Zero to current angle position
    void zeroRegW(uint16_t regVal); //write Zero register value
    uint16_t zeroRegR(void); //read Zero register value
    uint16_t angleRegR(void); //read raw value of the angle register
    //uint8_t diagR(void); //read diagnostic register
    uint16_t magnitudeR(void); //read current magnitude
    float angleR(int unit = U_RAW, bool newVal = true); //Read current angle or get last measure with unit conversion : RAW, TRN, DEG, RAD, GRAD, MOA, SOA, MILNATO, MILSE, MILRU
    uint8_t getAutoGain(void);
    uint8_t getDiagReg(void);

    void updateMovingAvgExp(void); //measure the current angle and feed the Exponential Moving Average calculation
    float getMovingAvgExp(int unit = U_RAW); //get Exponential Moving Average calculation
    void resetMovingAvgExp(void); //reset Exponential Moving Average calculation values

    int getAllData(uint8_t *agc, uint8_t *diag, uint16_t *mag, uint16_t *raw);
private:
    Ev3i2c i2c_;
    bool connected_;

    //variables
    bool _debugFlag;
    bool _clockWise;
    uint8_t _chipAddress;
    uint8_t _addressRegVal;
    uint16_t _zeroRegVal;
    float _lastAngleRaw;
    float _movingAvgExpAngle;
    float _movingAvgExpSin;
    float _movingAvgExpCos;
    float _movingAvgExpAlpha;
    int _movingAvgCountLoop;

    //methods
    uint8_t readReg8(uint8_t address);
    uint16_t readReg16(uint8_t address); //16 bit value got from 2x8bits registers (7..0 MSB + 5..0 LSB) => 14 bits value
    int readRegs(uint8_t address, uint8_t len, uint8_t* data);
    void writeReg(uint8_t address, uint8_t value);
    float convertAngle(int unit, float angle); //RAW, TRN, DEG, RAD, GRAD, MOA, SOA, MILNATO, MILSE, MILRU
    float getExpAvgRawAngle(void);
    void printDebug(void);
};

#endif
