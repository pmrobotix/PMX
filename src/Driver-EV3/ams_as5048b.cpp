/**************************************************************************/
/*!
 @file     ams_as5048b.cpp
 @author   SOSAndroid (E. Ha.)
 @license  BSD

 Library to interface the AS5048B magnetic rotary encoder from AMS over the I2C bus

 @section  HISTORY

 v1.0.0 - First release
 v1.0.1 - Typo to allow compiling on Codebender.cc (Math.h vs math.h)
 v1.0.2 - setZeroReg() issue raised by @MechatronicsWorkman
 v1.0.3 - Small bug fix and improvement by @DavidHowlett
 +Modifications PMX to use Ev3dev i2c-other features
 */
/**************************************************************************/

#include "ams_as5048b.hpp"

#include <sys/types.h>
#include <unistd.h>
#include <cmath>
#include <cstdio>
#include <iostream>

#include "../Log/Logger.hpp"

//extern BaseSequentialStream *outputStream;

/*========================================================================*/
/*                            CONSTRUCTORS                                */
/*========================================================================*/

/**************************************************************************/
/*!
 Constructor
 */
/**************************************************************************/

AMS_AS5048B::AMS_AS5048B(uint8_t chipAddress, uint8_t i2cNum) :
        i2c_(i2cNum, true)
{
    //printf("\naddr 0x%02x 0x%02x 0x%02x 0x%02x curent_addr=0x%02x\n", AS5048B_ADDR(0, 0), AS5048B_ADDR(0, 1), AS5048B_ADDR(1, 0), AS5048B_ADDR(1, 1), chipAddress);

    connected_ = false;
    _chipAddress = chipAddress;
    _debugFlag = false;
    _zeroRegVal = 0;
    _addressRegVal = 0;
    _clockWise = false;
    _movingAvgCountLoop = 0;
    _movingAvgExpCos = 0.0;
    _movingAvgExpSin = 0.0;
    _movingAvgExpAngle = 0.0;
    _movingAvgExpAlpha = 0.0;
    _lastAngleRaw = 0.0;
}

/*========================================================================*/
/*                           PUBLIC FUNCTIONS                             */
/*========================================================================*/

bool AMS_AS5048B::connected() {
    return connected_;
}

/**************************************************************************/
/*!
 @brief  ping

 @params
 none
 @returns
 0
 */
/**************************************************************************/
int AMS_AS5048B::ping() {
    return i2c_.ping();
}

bool AMS_AS5048B::connect(void) {
    if (connected_)
        return connected_;

    _clockWise = false;
    int err = i2c_.begin(_chipAddress);

    if (err >= 0) connected_ = true;
    else {
        connected_ = false;
        return connected_;
    }
    usleep(10000);
    reset();
    return connected_;
}

/**************************************************************************/
/*!
 @brief  reset values and overall behaviors for AS5948B use

 @params
 none
 @returns
 none
 */
/**************************************************************************/
void AMS_AS5048B::reset(void) {
    _lastAngleRaw = 0.0;
    _zeroRegVal = AMS_AS5048B::zeroRegR();
    _addressRegVal = AMS_AS5048B::addressRegR();
    AMS_AS5048B::resetMovingAvgExp();
}
/**************************************************************************/
/*!
 *
 *
 @brief  Toggle debug output to serial

 @params
 none
 @returns
 none
 */
/**************************************************************************/
void AMS_AS5048B::toggleDebug(void) {
    _debugFlag = !_debugFlag;
    return;
}

/**************************************************************************/
/*!
 @brief  Set / unset clock wise counting - sensor counts CCW natively

 @params[in]
 bool cw - true: CW, false: CCW
 @returns
 none
 */
/**************************************************************************/
void AMS_AS5048B::setClockWise(bool cw) {
    _clockWise = cw;
    _lastAngleRaw = 0.0;
    AMS_AS5048B::resetMovingAvgExp();
    return;
}

/**************************************************************************/
/*!
 @brief  writes OTP control register

 @params[in]
 unit8_t register value
 @returns
 none
 */
/**************************************************************************/
void AMS_AS5048B::progRegister(uint8_t regVal) {
    AMS_AS5048B::writeReg(AS5048B_PROG_REG, regVal);
    return;
}

/**************************************************************************/
/*!
 @brief  Burn values to the slave address OTP register

 @params[in]
 none
 @returns
 none
 */
/**************************************************************************/
void AMS_AS5048B::doProg(void) {
    //enable special programming mode
    AMS_AS5048B::progRegister(0xFD);
    //chThdSleepMilliseconds(10);
    usleep(10000);

    //set the burn bit: enables automatic programming procedure
    AMS_AS5048B::progRegister(0x08);
    //chThdSleepMilliseconds(10);
    usleep(10000);

    //disable special programming mode
    AMS_AS5048B::progRegister(0x00);
    //chThdSleepMilliseconds(10);
    usleep(10000);

    return;
}

/**************************************************************************/
/*!
 @brief  Burn values to the zero position OTP register

 @params[in]
 none
 @returns
 none
 */
/**************************************************************************/
void AMS_AS5048B::doProgZero(void) {
    //this will burn the zero position OTP register like described in the datasheet
    //enable programming mode
    AMS_AS5048B::progRegister(0x01);
    usleep(10000);
    //chThdSleepMilliseconds(10);

    //set the burn bit: enables automatic programming procedure
    AMS_AS5048B::progRegister(0x08);
    usleep(10000);    //chThdSleepMilliseconds(10);

    //read angle information (equals to 0)
    uint16_t data = 0;
    AMS_AS5048B::readReg16(AS5048B_ANGLMSB_REG, &data);
    usleep(10000);    //chThdSleepMilliseconds(10);

    //enable verification
    AMS_AS5048B::progRegister(0x40);
    usleep(10000);    //chThdSleepMilliseconds(10);

    //read angle information (equals to 0)
    data = 0;
    AMS_AS5048B::readReg16(AS5048B_ANGLMSB_REG, &data);
    usleep(10000);    //chThdSleepMilliseconds(10);

    return;
}

/**************************************************************************/
/*!
 @brief  write I2C address value (5 bits) into the address register

 @params[in]
 unit8_t register value
 @returns
 none
 */
/**************************************************************************/
void AMS_AS5048B::addressRegW(uint8_t regVal) {
    // write the new chip address to the register
    AMS_AS5048B::writeReg(AS5048B_ADDR_REG, regVal);

    // update our chip address with our 5 programmable bits
    // the MSB is internally inverted, so we flip the leftmost bit
    _chipAddress = ((regVal << 2) | (_chipAddress & 0b11)) ^ (1 << 6);
    return;
}

/**************************************************************************/
/*!
 @brief  reads I2C address register value

 @params[in]
 none
 @returns
 uint8_t register value
 */
/**************************************************************************/
uint8_t AMS_AS5048B::addressRegR(void) {
    uint8_t data = 0;
    int err = AMS_AS5048B::readReg8(AS5048B_ADDR_REG, &data);
    if (err < 0) printf("\nError AMS_AS5048B::addressRegR() readReg8 err=%d\n", err);
    return data;
}

/**************************************************************************/
/*!
 @brief  sets current angle as the zero position

 @params[in]
 none
 @returns
 none
 */
/**************************************************************************/
void AMS_AS5048B::setZeroReg(void) {
    AMS_AS5048B::zeroRegW((uint16_t) 0x00); //Issue closed by @MechatronicsWorkman and @oilXander. The last sequence avoids any offset for the new Zero position
    uint16_t newZero = 0;
    int err = AMS_AS5048B::readReg16(AS5048B_ANGLMSB_REG, &newZero);
    if (err < 0) printf("\nError AMS_AS5048B::setZeroReg() readReg16 err=%d\n", err);
    AMS_AS5048B::zeroRegW(newZero);
    return;
}

/**************************************************************************/
/*!
 @brief  writes the 2 bytes Zero position register value

 @params[in]
 unit16_t register value
 @returns
 none
 */
/**************************************************************************/
void AMS_AS5048B::zeroRegW(uint16_t regVal) {
    AMS_AS5048B::writeReg(AS5048B_ZEROMSB_REG, (uint8_t) (regVal >> 6));
    AMS_AS5048B::writeReg(AS5048B_ZEROLSB_REG, (uint8_t) (regVal & 0x3F));
    return;
}

/**************************************************************************/
/*!
 @brief  reads the 2 bytes Zero position register value

 @params[in]
 none
 @returns
 uint16_t register value trimmed on 14 bits
 */
/**************************************************************************/
uint16_t AMS_AS5048B::zeroRegR(void) {
    uint16_t data = 0;
    int err = AMS_AS5048B::readReg16(AS5048B_ZEROMSB_REG, &data);
    if (err < 0) printf("\nError AMS_AS5048B::zeroRegR() readReg16 err=%d\n", err);
    return data;
}

/**************************************************************************/
/*!
 @brief  reads the 2 bytes magnitude register value

 @params[in]
 none
 @returns
 uint16_t register value trimmed on 14 bits
 */
/**************************************************************************/
uint16_t AMS_AS5048B::magnitudeR(void) {
    uint16_t data = 0;
    int err = AMS_AS5048B::readReg16(AS5048B_MAGNMSB_REG, &data);
    if (err < 0) printf("\nError AMS_AS5048B::magnitudeR() readReg16 err=%d\n", err);
    return data;
}

uint16_t AMS_AS5048B::angleRegR(void) {
    uint16_t data = 0;
    int err = AMS_AS5048B::readReg16(AS5048B_ANGLMSB_REG, &data);
    if (err < 0) printf("\nError AMS_AS5048B::angleRegR() readReg16 err=%d\n", err);
    return data;
}

/**************************************************************************/
/*!
 @brief  reads the 1 bytes auto gain register value

 @params[in]
 none
 @returns
 uint8_t register value
 */
/**************************************************************************/
uint8_t AMS_AS5048B::getAutoGain(void) {
    uint8_t data = 0;
    int err = AMS_AS5048B::readReg8(AS5048B_GAIN_REG, &data);
    if (err < 0) printf("\nError AMS_AS5048B::getAutoGain() readReg8 err=%d\n", err);
    return data;
}

/**************************************************************************/
/*!
 @brief  reads the 1 bytes diagnostic register value

 @params[in]
 none
 @returns
 uint8_t register value
 */
/**************************************************************************/
uint8_t AMS_AS5048B::getDiagReg(void) {
    uint8_t data = 0;
    int err = AMS_AS5048B::readReg8(AS5048B_DIAG_REG, &data);
    if (err < 0) printf("\nError AMS_AS5048B::getDiagReg() readReg8 err=%d\n", err);
    return data;
}

/**************************************************************************/
/*!
 @brief  reads current angle value and converts it into the desired unit

 @params[in]
 String unit : string expressing the unit of the angle. Sensor raw value as default
 @params[in]
 Bool newVal : have a new measurement or use the last read one. True as default
 @returns
 float angle value converted into the desired unit
 */
/**************************************************************************/
float AMS_AS5048B::angleR(int unit, bool newVal) {
    float angleRaw = 0;
    uint16_t data = 0;

    if (newVal) {
        int err = AMS_AS5048B::readReg16(AS5048B_ANGLMSB_REG, &data);
        if (err == 0) {
            if (_clockWise) {
                angleRaw = (float) (0b11111111111111 - data);
            }
            else {
                angleRaw = (float) data;
            }

            _lastAngleRaw = angleRaw;
        }
        else printf("\nError AMS_AS5048B::angleR() readReg16 err=%d\n", err);
    }
    else {
        angleRaw = _lastAngleRaw;
    }

    return AMS_AS5048B::convertAngle(unit, angleRaw);
}

/**************************************************************************/
/*!
 @brief  Performs an exponential moving average on the angle.
 Works on Sine and Cosine of the angle to avoid issues 0°/360° discontinuity

 @params[in]
 none
 @returns
 none
 */
/**************************************************************************/
void AMS_AS5048B::updateMovingAvgExp(void) {
    //sine and cosine calculation on angles in radian
    float angle = AMS_AS5048B::angleR(U_RAD, true);

    if (_movingAvgCountLoop < EXP_MOVAVG_LOOP) {
        _movingAvgExpSin += sin(angle);
        _movingAvgExpCos += cos(angle);
        if (_movingAvgCountLoop == (EXP_MOVAVG_LOOP - 1)) {
            _movingAvgExpSin = _movingAvgExpSin / EXP_MOVAVG_LOOP;
            _movingAvgExpCos = _movingAvgExpCos / EXP_MOVAVG_LOOP;
        }
        _movingAvgCountLoop++;
    }
    else {
        float movavgexpsin = _movingAvgExpSin + _movingAvgExpAlpha * (sin(angle) - _movingAvgExpSin);
        float movavgexpcos = _movingAvgExpCos + _movingAvgExpAlpha * (cos(angle) - _movingAvgExpCos);
        _movingAvgExpSin = movavgexpsin;
        _movingAvgExpCos = movavgexpcos;
        _movingAvgExpAngle = getExpAvgRawAngle();
    }

    return;
}

/**************************************************************************/
/*!
 @brief  sent back the exponential moving averaged angle in the desired unit

 @params[in]
 String unit : string expressing the unit of the angle. Sensor raw value as default
 @returns
 float exponential moving averaged angle value
 */
/**************************************************************************/
float AMS_AS5048B::getMovingAvgExp(int unit) {
    return AMS_AS5048B::convertAngle(unit, _movingAvgExpAngle);
}

void AMS_AS5048B::resetMovingAvgExp(void) {
    _movingAvgExpAngle = 0.0;
    _movingAvgCountLoop = 0;
    _movingAvgExpAlpha = 2.0 / (EXP_MOVAVG_N + 1.0);
    return;
}

int AMS_AS5048B::getAllData(uint8_t *agc, uint8_t *diag, uint16_t *mag, uint16_t *raw) {
    uint8_t data[6] = { 0 };
    int err = readRegs(AS5048B_GAIN_REG, 6, data);
    //printf("%02X:%02X:%02X:%02X:%02X:%02X\n", data[0], data[1], data[2], data[3], data[4], data[5]);
    if (err < 0) return err;
    *agc = data[0];
    *diag = data[1];
    *mag = ((uint16_t) (data[2]) << 6) + (data[3] & 0x3F);
    *raw = ((uint16_t) (data[4]) << 6) + (data[5] & 0x3F);
    return err;
}

/*========================================================================*/
/*                           PRIVATE FUNCTIONS                            */
/*========================================================================*/

int AMS_AS5048B::readReg8(uint8_t reg_address, uint8_t* readValue) {
    if (!connected_) return -99;
    int err = readRegs(reg_address, 1, readValue);
    return err;
}

int AMS_AS5048B::readReg16(uint8_t reg_address, uint16_t* readValue) {
    if (!connected_) return -99;
    //16 bit value got from 2 8bits registers (7..0 MSB + 5..0 LSB) => 14 bits value
    uint8_t readArray[2] = { 0, 0 };
    int err = readRegs(reg_address, 2, readArray);
    if (err >= 0) {
        *readValue = (((uint16_t) readArray[0]) << 6);
        *readValue += (readArray[1] & 0x3F);
    }

    return err;
}

int AMS_AS5048B::readRegs(uint8_t reg_address, uint8_t len, uint8_t* data) {
    if (!connected_) return -99;
    int err = i2c_.readReg(reg_address, data, len);
    return err;
}

int AMS_AS5048B::writeReg(uint8_t reg_address, uint8_t value) {
    if (!connected_) return -99;
    uint8_t values[] = { value };
    int err = i2c_.writeReg(reg_address, values, sizeof(values));
    return err;
}

float AMS_AS5048B::convertAngle(int unit, float angle) {
    // convert raw sensor reading into angle unit
    float angleConv;

    switch (unit) {
        case U_RAW:
            //Sensor raw measurement
            angleConv = angle;
            break;
        case U_TRN:
            //full turn ratio
            angleConv = (angle / AS5048B_RESOLUTION);
            break;
        case U_DEG:
            //degree
            angleConv = (angle / AS5048B_RESOLUTION) * 360.0;
            break;
        case U_RAD:
            //Radian
            angleConv = (angle / AS5048B_RESOLUTION) * 2 * M_PI;
            break;
        case U_MOA:
            //minute of arc
            angleConv = (angle / AS5048B_RESOLUTION) * 60.0 * 360.0;
            break;
        case U_SOA:
            //second of arc
            angleConv = (angle / AS5048B_RESOLUTION) * 60.0 * 60.0 * 360.0;
            break;
        case U_GRAD:
            //grade
            angleConv = (angle / AS5048B_RESOLUTION) * 400.0;
            break;
        case U_MILNATO:
            //NATO MIL
            angleConv = (angle / AS5048B_RESOLUTION) * 6400.0;
            break;
        case U_MILSE:
            //Swedish MIL
            angleConv = (angle / AS5048B_RESOLUTION) * 6300.0;
            break;
        case U_MILRU:
            //Russian MIL
            angleConv = (angle / AS5048B_RESOLUTION) * 6000.0;
            break;
        default:
            //no conversion => raw angle
            angleConv = angle;
            break;
    }
    return angleConv;
}

float AMS_AS5048B::getExpAvgRawAngle(void) {
    float angle;
    float twopi = 2 * M_PI;

    if (_movingAvgExpSin < 0.0) {
        angle = twopi - acos(_movingAvgExpCos);
    }
    else {
        angle = acos(_movingAvgExpCos);
    }

    angle = (angle / twopi) * AS5048B_RESOLUTION;

    return angle;
}

void AMS_AS5048B::printDebug(void) {
    return;
}
