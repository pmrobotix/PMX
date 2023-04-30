/*
 * CCAx12Teensy.cpp
 *
 *  Created on: Apr 17, 2022
 *      Author: pmx
 */

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#include "CCAx12Teensy.hpp"

CCAx12Teensy::CCAx12Teensy() : //TODO Une SEULE CARTE est possible a mettre en paramettre adresse + port mais attention à l'instance unique
//Connexion i2C 5V
        i2c_CCAx12Teensy_(1), connected_(false)
{
    i2c_aAddr_ = 0;
    i2cHandle_ = 0;
}

bool CCAx12Teensy::connect(short int i2c_aAddr) {
    i2c_aAddr_ = i2c_aAddr;

    int err = i2c_CCAx12Teensy_.setSlaveAddr(i2c_aAddr);
    if (err >= 0) {
        connected_ = true;
    }
    else {
        connected_ = false;
        logger().error() << "CCAx12Teensy::connect() : NOT CONNECTED!" << logs::end;
    }

    return connected_;
}

int CCAx12Teensy::convertToVoltage(int adc_value) {

    double millivolts = adc_value * 3300.0 / 4095.0; // 3V3

    return (int) millivolts;

}

/**
 * Read bytes
 * cmd : command
 * buf : where to store data read from slave
 * bytesToRead : how many bytes to read
 * Returns negative errno if error
 */
uint8_t CCAx12Teensy::readI2C(uint8_t cmd, uint8_t * data, uint16_t bytesToRead) {

    int err = i2c_CCAx12Teensy_.readReg(cmd, data, bytesToRead);
    //DEBUG
//    for (int i = 0; i < bytesToRead; i++) {
//        printf("readI2C: cmd=%d i=%d %d\n", cmd, i, data[i]);
//    }
    return err;

    /*
     // I2C-Create a Read message.
     struct i2c_msg messages[] = { { i2c_aAddr_, 0, 1, &cmd }, //Set register address.
     { i2c_aAddr_, I2C_M_RD, bytesToRead, data } //Read length bytes into data.
     };
     struct i2c_rdwr_ioctl_data ioctl_data = { messages, 2 };
     int e = ioctl(i2cHandle_, I2C_RDWR, &ioctl_data);
     for (int i = 0; i < bytesToRead; i++) {
     printf("readI2C: cmd=%d i=%d %d\n", cmd, i, data[i]);
     }

     // I2C-Read.
     if (e != 0) {
     fprintf(stderr, "readI2C : %d failed to ioctl: %s \n", e, strerror(errno));
     return -1;
     }
     return 0;
     */
}

///**VERSION CHAFF INO
// * Read bytes
// * cmd : command
// * buf : where to store data read from slave
// * bytesToRead : how many bytes to read
// * Returns negative errno if error
// */
//uint8_t readI2C(uint8_t cmd, uint8_t * buf, uint16_t bytesToRead) {
//    /* I2C-Create a Read message. */
//    struct i2c_msg messages[] = {
//        { slave_addr_i2c, 0, 1, &cmd },         /*Set register address. */
//        { slave_addr_i2c, I2C_M_RD, bytesToRead, buf },  /*Read length bytes into data. */
//    };
//    struct i2c_rdwr_ioctl_data ioctl_data = { messages, 2 };
//
//    /* I2C-Read. */
//    if (ioctl(fd_i2c, I2C_RDWR, &ioctl_data) != 2) {
//        fprintf(stderr, "readI2C : failed to ioctl: %s\n", strerror(errno));
//        return -1;
//    }
//    return 0;
//}

/**
 * Write bytes
 * cmd : command
 * data : bytes to send to slave
 * bytesToWrite : how many bytes to write
 * Returns negative errno if error
 */
int CCAx12Teensy::writeI2C(uint8_t * data, uint16_t bytesToWrite) {
    //DEBUG
//    for (int i = 0; i < bytesToWrite; i++) {
//        printf("writeI2C: i=%d %d\n", i, data[i]);
//    }
    uint8_t cmd = data[0];

    uint8_t* buffer = (uint8_t*) malloc(bytesToWrite - 1);
    memcpy(buffer, &data[1], bytesToWrite - 1); //Set data after the second byte.
    int err = i2c_CCAx12Teensy_.writeReg(cmd, buffer, bytesToWrite - 1);
    free(buffer);
    return err;

    //printf("writeI2C: address=%d i2cHandle_=%d \n", i2c_aAddr_, i2cHandle_);
    /*
     // I2C-Prepare a buffer for Write.
     char* buffer = (char*) malloc(bytesToWrite + 1);
     if (buffer == NULL) {
     fprintf(stderr, "write2C : failed to memory allocate\n");
     return -1;
     }
     buffer[0] = cmd; //Set the register address in the 1st byte.
     memcpy(&buffer[1], data, bytesToWrite); //Set data after the second byte.

     // I2C-Create a Write message.
     struct i2c_msg message = { i2c_aAddr_, 0, bytesToWrite + 1, buffer };
     struct i2c_rdwr_ioctl_data ioctl_data = { &message, 1 };

     // I2C-Write.
     int e = ioctl(i2cHandle_, I2C_RDWR, &ioctl_data);
     if (e != 0) {
     fprintf(stderr, "writeI2C :%d failed to ioctl: %s\n", e, strerror(errno));
     free(buffer);
     return -1;
     }

     free(buffer);
     return 0;
     */
}

/** VERSION CHAFF
 * Write bytes
 * data : bytes to send to slave
 * bytesToWrite : how many bytes to write
 * Returns negative errno if error
 */
//int writeI2C(uint8_t * data, uint16_t bytesToWrite) {
//    /* I2C-Create a Write message. */
//    struct i2c_msg message = { slave_addr_i2c, 0, bytesToWrite, data };
//    struct i2c_rdwr_ioctl_data ioctl_data = { &message, 1 };
//
//    /* I2C-Write. */
//    if (ioctl(fd_i2c, I2C_RDWR, &ioctl_data) != 1) {
//        fprintf(stderr, "writeI2C : failed to ioctl: %s\n", strerror(errno));
//        return -1;
//    }
//    return 0;
//}

int CCAx12Teensy::getAddressSize(int address) {
    switch (address) {
        case P_MODEL_NUMBER:
        case P_CW_ANGLE_LIMIT:
        case P_CCW_ANGLE_LIMIT:
        case P_MAX_TORQUE:
        case P_DOWN_CALIBRATION:
        case P_UP_CALIBRATION:
        case P_GOAL_POSITION:
        case P_GOAL_SPEED:
        case P_TORQUE_LIMIT:
        case P_PRESENT_POSITION:
        case P_PRESENT_SPEED:
        case P_PRESENT_LOAD:
        case P_PUNCH:
            return 2;
        default:
            return 1;
    }
}

uint8_t CCAx12Teensy::computeCRC(uint8_t *buf, uint16_t length) {
    uint8_t result = 5;
    for (int i = 0; i < length; i++) {
        result = result * 3 + buf[i];
    }
    return result;
}

int CCAx12Teensy::checkCRC(uint8_t* buf, uint16_t length) {
    uint8_t crc = buf[length - 1];
    uint8_t computed = computeCRC(buf, length - 1);
    if (crc == computed) {
        return 0;
    }
    //printf("checkCRC failed : %d != %d\n", crc, computed);
    logger().error() << "checkCRC() failed crc:" << crc << " != computed:" << computed << logs::end;
    return -1;
}

int CCAx12Teensy::setLed(uint8_t led, bool on) {
    int cmd = AXT_CMD_SET_LED_OFF;
    if (on) {
        cmd = AXT_CMD_SET_LED_ON;
    }

    // Send message
    uint8_t buffer[3];
    buffer[0] = cmd;
    buffer[1] = led;
    buffer[2] = computeCRC(buffer, 2);
    int err_write = writeI2C(buffer, 3);
    if (err_write < 0) {
        return AXT_ERROR_SEND_CMD;
    }
    // Read response
    uint8_t status[2];
    int err_read = readI2C(AXT_STATUS, status, 2);
    if (err_read < 0) {
        return AXT_ERROR_READ_STATUS;
    }
    if (checkCRC(status, 2) != 0) {
        return AXT_ERROR_CRC;
    }
    return status[0];
}

// Set the led on
// @param led :  1 - 10
int CCAx12Teensy::setLedOn(uint8_t led) {
    return setLed(led, true);
}

// Set the led off
// @param led :  1 - 10
int CCAx12Teensy::setLedOff(uint8_t led) {
    return setLed(led, false);
}

// Get the ADC value (12bits)
// @param ADC :  0 - 9
// @returns value : 0 - 4095
int CCAx12Teensy::getADC(uint8_t adc) {
    // Send message
    uint8_t buffer[3];
    buffer[0] = AXT_CMD_GET_ADC;
    buffer[1] = adc;
    buffer[2] = computeCRC(buffer, 2);
    int err_write = writeI2C(buffer, 3);
    if (err_write < 0) {
        return AXT_ERROR_SEND_CMD;
    }
    // Read response
    uint8_t status[4];
    int err_read = readI2C(AXT_STATUS, status, 4);
    if (err_read < 0) {
        return AXT_ERROR_READ_STATUS;
    }
    if (checkCRC(status, 4) != 0) {
        return AXT_ERROR_CRC;
    }
    return status[1] + 256 * status[2];

}

/**
 * Ping a Dynamixel Servo
 * @param port : the port (1-4)
 * @param id : the id of the dynamixel
 * @returns AXT_OK is servo is found, AXT_AX_MISSING if not found
 * (on error, result is <0)
 */
int CCAx12Teensy::pingAX(uint8_t port, uint8_t id) {
    // Send message
    uint8_t buffer[4];
    buffer[0] = AXT_CMD_PING_AX;
    buffer[1] = port;
    buffer[2] = id;
    buffer[3] = computeCRC(buffer, 3);
    int err_write = writeI2C(buffer, 4);
    if (err_write < 0) {
        return AXT_ERROR_SEND_CMD;
    }
    // Read response
    uint8_t status[2];
    int err_read = readI2C(AXT_STATUS, status, 2);
    if (err_read < 0) {
        return AXT_ERROR_READ_STATUS;
    }
    if (checkCRC(status, 2) != 0) {
        return AXT_ERROR_CRC;
    }
    int result = status[0];
    return result;
}

/**
 * Read data at a specified address
 * @param port : the port (1-4)
 * @param id : the id of the dynamixel
 * @param address : address of the data
 * @returns the data
 */
int CCAx12Teensy::readAXData(uint8_t port, uint8_t id, uint8_t address) {
    // Send message
    uint8_t buffer[5];
    buffer[0] = AXT_CMD_READ_AX;
    buffer[1] = port;
    buffer[2] = id;
    buffer[3] = address;
    buffer[4] = computeCRC(buffer, 4);
    int err_write = writeI2C(buffer, 5);
    if (err_write < 0) {
        return AXT_ERROR_SEND_CMD;
    }
    // Read response
    uint8_t status[4];
    int err_read = readI2C(AXT_STATUS, status, 4);
    if (err_read < 0) {
        return AXT_ERROR_READ_STATUS;
    }
    if (checkCRC(status, 4) != 0) {
        return AXT_ERROR_CRC;
    }
    return status[1] + 256 * status[2];
}

/**
 * Write data at a specified address
 * @param port : the port (1-4)
 * @param id : the id of the dynamixel
 * @param address : address of the data
 */
int CCAx12Teensy::writeAXData(uint8_t port, uint8_t id, uint8_t address, uint16_t data) {
    uint8_t xlow = data & 0xff;
    uint8_t xhigh = (data >> 8);

    uint8_t buffer[7];
    buffer[0] = AXT_CMD_WRITE_AX;
    buffer[1] = port;
    buffer[2] = id;
    buffer[3] = address;
    buffer[4] = xlow;
    buffer[5] = xhigh;
    buffer[6] = computeCRC(buffer, 6);
    int err_write = writeI2C(buffer, 7);
    if (err_write < 0) {
        return AXT_ERROR_SEND_CMD;
    }

    // Read response
    uint8_t status[2];
    int err_read = readI2C(AXT_STATUS, status, 2);
    if (err_read < 0) {
        return AXT_ERROR_READ_STATUS;
    }
    if (checkCRC(status, 2) != 0) {
        return AXT_ERROR_CRC;
    }

    return 0;
}
