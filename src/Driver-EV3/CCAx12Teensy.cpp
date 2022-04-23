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

#include "i2c-dev.h"
#include "CCAx12Teensy.hpp"

CCAx12Teensy::CCAx12Teensy() : //TODO Une SEULE CARTE est possible a mettre en paramettre adresse + port mais attention à l'instance unique
        i2c_CCAx12Teensy_(1, false), connected_(false)
{
}

bool CCAx12Teensy::connect(uint i2c_aAddr) {
    if (connected_) return connected_;
/*
    while(1)
    {
    //open i2c and setslave
    int err = i2c_CCAx12Teensy_.begin(i2c_aAddr);
    if (err >= 0)
        {
        connected_ = true;

        logger().error() << "setLedOn" << logs::end;
        setLedOn(5);
        int port = 1;
        int id = 8;
        int p = pingAX(port, id);

        printf("Testing AX on port %d, id %d, ping : \n", port, id, p);
         printf("read AX %d MODEL_NUMBER returned %d\n", id, readAXData(port, id, P_MODEL_NUMBER));
         printf("read AX %d MODEL_VERSION returned %d\n", id, readAXData(port, id, P_VERSION));
         printf("read AX %d ID returned %d\n", id, readAXData(port, id, P_ID));
         printf("read AX %d BAUD_RATE returned %d\n", id, readAXData(port, id, P_BAUD_RATE));
        usleep(100000);
        setLedOff(5);
        }
    else {
        connected_ = false;
        logger().error() << "CCAx12Teensy::connect() : NOT CONNECTED!" << logs::end;
    }
    usleep(300000);
    }*/

    return connected_;
}

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

uint8_t CCAx12Teensy::computeCRC(uint8_t c, uint8_t *buf, uint16_t length) {
    uint8_t result = c;
    for (int i = 0; i < length; i++) {
        result = result * 3 + buf[i];
    }
    return result;
}

int CCAx12Teensy::checkCRC(uint8_t* buf, uint16_t length) {
    uint8_t crc = buf[length - 1];
    uint8_t computed = computeCRC(buf[0], buf, length - 1);
    if (crc == computed) {
        return 0;
    }
    logger().error() << "checkCRC failed : " << std::dec << crc << "!=" << std::dec << computed << logs::end;
    printf("checkCRC failed : %d != %d\n", crc, computed);
    return -1;
}

int CCAx12Teensy::setLed(uint8_t led, bool on) {
    if (!connected_) {
        return -999;
    }

    int cmd = AXT_CMD_SET_LED_OFF;
    if (on) {
        cmd = AXT_CMD_SET_LED_ON;
    }

// Send message
    uint8_t buffer[2];
    buffer[0] = led;
    buffer[1] = computeCRC(cmd, buffer, 1);

    mutex_.lock();
    int err_write = writeI2C(cmd, buffer, 2);
    if (err_write < 0) {
        mutex_.unlock();
        return AXT_ERROR_SEND_CMD;
    }
// Read response
    uint8_t status[2];
    int err_read = readI2C(AXT_STATUS, status, 2);
    if (err_read < 0) {
        mutex_.unlock();
        return AXT_ERROR_READ_STATUS;
    }
    mutex_.unlock();
    if (checkCRC(status, 2) != 0) {
        return AXT_ERROR_CRC;
    }
    return status[0];
}

// Set the led on
// @param led :  1 - 9
int CCAx12Teensy::setLedOn(uint8_t led) {
    return setLed(led, true);
}

// Set the led off
// @param led :  1 - 9
int CCAx12Teensy::setLedOff(uint8_t led) {
    return setLed(led, false);
}

//// I2C communication
//int32_t fd_i2c;
//uint8_t slave_addr_i2c;
//
//int initI2C(uint8_t slaveAddress) {
//    slave_addr_i2c=slaveAddress;
//    fd_i2c = open("/dev/i2c-1", O_RDWR);
//    if (fd_i2c == -1) {
//        fprintf(stderr, "initI2C: failed to open: %s\n", strerror(errno));
//        return -1;
//    }
//    return 0;
//}

/**
 * Read bytes
 * cmd : command
 * buf : where to store data read from slave
 * bytesToRead : how many bytes to read
 * Returns negative errno if error
 */
uint8_t CCAx12Teensy::readI2C(uint8_t cmd, uint8_t * buf, uint16_t bytesToRead) {

    int err = i2c_CCAx12Teensy_.readReg(cmd, buf, bytesToRead);
    return err;
    /*
     // I2C-Create a Read message.
     struct i2c_msg messages[] = { { slave_addr_i2c, 0, 1, &cmd }, //Set register address.
     { slave_addr_i2c, I2C_M_RD, bytesToRead, buf }, //Read length bytes into data.
     };
     struct i2c_rdwr_ioctl_data ioctl_data = { messages, 2 };

     // I2C-Read.
     if (ioctl(fd_i2c, I2C_RDWR, &ioctl_data) != 2) {
     fprintf(stderr, "readI2C : failed to ioctl: %s\n", strerror(errno));
     return -1;
     }
     return 0;
     */
}

/**
 * Write bytes
 * cmd : command
 * data : bytes to send to slave
 * bytesToWrite : how many bytes to write
 * Returns negative errno if error
 */
int CCAx12Teensy::writeI2C(uint8_t cmd, uint8_t * data, uint16_t bytesToWrite) {

    int err = i2c_CCAx12Teensy_.writeReg(cmd, data, bytesToWrite);
    return err;

    /*
     // I2C-Prepare a buffer for Write.
     uint8_t* buffer = (uint8_t*) malloc(bytesToWrite + 1);
     if (buffer == NULL) {
     fprintf(stderr, "write2C : failed to memory allocate\n");
     return -1;
     }
     buffer[0] = cmd; //Set the register address in the 1st byte.
     memcpy(&buffer[1], data, bytesToWrite); //Set data after the second byte.

     // I2C-Create a Write message.
     struct i2c_msg message = { slave_addr_i2c, 0, bytesToWrite + 1, buffer };
     struct i2c_rdwr_ioctl_data ioctl_data = { &message, 1 };

     // I2C-Write.
     if (ioctl(fd_i2c, I2C_RDWR, &ioctl_data) != 1) {
     fprintf(stderr, "writeI2C : failed to ioctl: %s\n", strerror(errno));
     free(buffer);
     return -1;
     }

     free(buffer);
     return 0;
     */
}

//

// Get the ADC value (12bits)
// @param ADC :  0 - 9
// @returns value : 0 - 4095
int CCAx12Teensy::getADC(uint8_t adc) {
// Send message
    uint8_t buffer[2];
    buffer[0] = adc;
    buffer[1] = computeCRC(AXT_CMD_GET_ADC, buffer, 1);
    mutex_.lock();
    int err_write = writeI2C(AXT_CMD_GET_ADC, buffer, 2);
    if (err_write < 0) {
        mutex_.unlock();
        return AXT_ERROR_SEND_CMD;
    }
// Read response
    uint8_t status[4];
    int err_read = readI2C(AXT_STATUS, status, 4);
    mutex_.unlock();
    for (int i = 0; i < 4; i++) {
        printf("%d\n", status[i]);
    }
    if (err_read < 0) {
        return AXT_ERROR_READ_STATUS;
    }
    if (checkCRC(status, 4) != 0) {
        return AXT_ERROR_CRC;
    }
    return status[1] + 256 * status[2];

}

int CCAx12Teensy::convertToVoltage(int adc_value) {

    double millivolts = adc_value * 3300.0 / 4095.0; //5V ou 3V3?

    return (int) millivolts;

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
    uint8_t buffer[3];
    buffer[0] = port;
    buffer[1] = id;
    buffer[2] = computeCRC(AXT_CMD_PING_AX, buffer, 2);
    mutex_.lock();
    int err_write = writeI2C(AXT_CMD_PING_AX, buffer, 3);
    if (err_write < 0) {
        mutex_.unlock();
        return AXT_ERROR_SEND_CMD;
    }
// Read response
    uint8_t status[2];
    int err_read = readI2C(AXT_STATUS, status, 2);
    mutex_.unlock();
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
    uint8_t buffer[4];
    buffer[0] = port;
    buffer[1] = id;
    buffer[2] = address;
    buffer[3] = computeCRC(AXT_CMD_READ_AX, buffer, 3);
    mutex_.lock();
    int err_write = writeI2C(AXT_CMD_READ_AX, buffer, 4);
    if (err_write < 0) {
        mutex_.unlock();
        return AXT_ERROR_SEND_CMD;
    }
// Read response
    uint8_t status[4];
    int err_read = readI2C(AXT_STATUS, status, 4);
    mutex_.unlock();
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

    uint8_t buffer[6];
    buffer[0] = port;
    buffer[1] = id;
    buffer[2] = address;
    buffer[3] = xlow;
    buffer[4] = xhigh;
    buffer[5] = computeCRC(AXT_CMD_WRITE_AX, buffer, 5);
    mutex_.lock();
    int err_write = writeI2C(AXT_CMD_WRITE_AX, buffer, 6);
    if (err_write < 0) {
        mutex_.unlock();
        return AXT_ERROR_SEND_CMD;
    }

// Read response
    uint8_t status[2];
    int err_read = readI2C(AXT_STATUS, status, 2);
    mutex_.unlock();
    if (err_read < 0) {
        return AXT_ERROR_READ_STATUS;
    }
    if (checkCRC(status, 2) != 0) {
        return AXT_ERROR_CRC;
    }
    return 0;
}
/*
 int main(int argc, char *argv[]) {
 int data;
 int addr = (0x08);
 int status = initI2C(addr);
 printf("Setup communication to I2C slave address %d\n", addr);

 if (status < 0) {
 printf("Can't setup the I2C device\n");
 return -1;
 }
 int id = 2;
 int i;
 if (0) {
 for (i = 0; i < 10; i++) {
 printf("Flashing LED %d\n", i + 1);
 setLedOn(i + 1);
 usleep(50000);
 setLedOff(i + 1);
 usleep(2000);
 }
 }

 int port = 1;
 if (0) {
 for (i = 1; i < 5; i++) {
 int r = pingAX(1, i);
 if (r == 0) {
 printf("Ping AX %d : AX FOUND\n", i);
 }
 else if (r == AXT_AX_MISSING) {
 printf("Ping AX %d : AX NOT FOUND\n", i);
 }
 else {
 printf("Ping AX %d : error %d\n", i, r);
 }
 }
 }
 if (1) {
 printf("Testing AX on port %d, id %d\n", port, id);
 printf("read AX %d MODEL_NUMBER returned %d\n", id, readAXData(port, id, P_MODEL_NUMBER));
 printf("read AX %d MODEL_VERSION returned %d\n", id, readAXData(port, id, P_VERSION));
 printf("read AX %d ID returned %d\n", id, readAXData(port, id, P_ID));
 printf("read AX %d BAUD_RATE returned %d\n", id, readAXData(port, id, P_BAUD_RATE));
 }
 if (1) {
 printf("read AX %d RETURN_DELAY_TIME returned %d\n", id, readAXData(port, id, P_RETURN_DELAY_TIME));
 printf("read AX %d CW_ANGLE_LIMIT returned %d\n", id, readAXData(port, id, P_CW_ANGLE_LIMIT));
 printf("read AX %d CCW_ANGLE_LIMIT returned %d\n", id, readAXData(port, id, P_CCW_ANGLE_LIMIT));
 printf("read AX %d SYSTEM_DATA2 returned %d\n", id, readAXData(port, id, P_SYSTEM_DATA2));
 printf("read AX %d LIMIT_TEMPERATURE returned %d\n", id, readAXData(port, id, P_LIMIT_TEMPERATURE));
 printf("read AX %d DOWN_LIMIT_VOLTAGE returned %d\n", id, readAXData(port, id, P_DOWN_LIMIT_VOLTAGE));
 printf("read AX %d UP_LIMIT_VOLTAGE returned %d\n", id, readAXData(port, id, P_UP_LIMIT_VOLTAGE));
 printf("read AX %d MAX_TORQUE returned %d\n", id, readAXData(port, id, P_MAX_TORQUE));
 printf("read AX %d RETURN_LEVEL returned %d\n", id, readAXData(port, id, P_RETURN_LEVEL));
 printf("read AX %d ALARM_LED returned %d\n", id, readAXData(port, id, P_ALARM_LED));
 printf("read AX %d ALARM_SHUTDOWN returned %d\n", id, readAXData(port, id, P_ALARM_SHUTDOWN));
 printf("read AX %d OPERATING_MODE returned %d\n", id, readAXData(port, id, P_OPERATING_MODE));
 printf("read AX %d DOWN_CALIBRATION returned %d\n", id, readAXData(port, id, P_DOWN_CALIBRATION));
 printf("read AX %d UP_CALIBRATION returned %d\n", id, readAXData(port, id, P_UP_CALIBRATION));
 printf("read AX %d TORQUE_ENABLE returned %d\n", id, readAXData(port, id, P_TORQUE_ENABLE));
 printf("read AX %d LED returned %d\n", id, readAXData(port, id, P_LED));
 printf("read AX %d CW_COMPLIANCE_MARGIN returned %d\n", id, readAXData(port, id, P_CW_COMPLIANCE_MARGIN));
 printf("read AX %d CCW_COMPLIANCE_MARGIN returned %d\n", id, readAXData(port, id, P_CCW_COMPLIANCE_MARGIN));
 printf("read AX %d CW_COMPLIANCE_SLOPE returned %d\n", id, readAXData(port, id, P_CW_COMPLIANCE_SLOPE));
 printf("read AX %d CCW_COMPLIANCE_SLOPE returned %d\n", id, readAXData(port, id, P_CCW_COMPLIANCE_SLOPE));
 printf("read AX %d GOAL_POSITION returned %d\n", id, readAXData(port, id, P_GOAL_POSITION));
 printf("read AX %d GOAL_SPEED returned %d\n", id, readAXData(port, id, P_GOAL_SPEED));
 printf("read AX %d TORQUE_LIMIT returned %d\n", id, readAXData(port, id, P_TORQUE_LIMIT));
 printf("read AX %d PRESENT_POSITION returned %d\n", id, readAXData(port, id, P_PRESENT_POSITION));
 printf("read AX %d PRESENT_SPEED returned %d\n", id, readAXData(port, id, P_PRESENT_SPEED));
 printf("read AX %d PRESENT_LOAD returned %d\n", id, readAXData(port, id, P_PRESENT_LOAD));
 printf("read AX %d PRESENT_VOLTAGE returned %d\n", id, readAXData(port, id, P_PRESENT_VOLTAGE));
 printf("read AX %d PRESENT_TEMPERATURE returned %d\n", id, readAXData(port, id, P_PRESENT_TEMPERATURE));
 printf("read AX %d REGISTERED_INSTRUCTION returned %d\n", id, readAXData(port, id, P_REGISTERED_INSTRUCTION));
 printf("read AX %d PAUSE_TIME returned %d\n", id, readAXData(port, id, P_PAUSE_TIME));
 printf("read AX %d MOVING returned %d\n", id, readAXData(port, id, P_MOVING));
 printf("read AX %d LOCK returned %d\n", id, readAXData(port, id, P_LOCK));
 printf("read AX %d PUNCH returned %d\n", id, readAXData(port, id, P_PUNCH));

 int val = 150;
 printf("read AX %d GOAL_POSITION %d returned %d\n", id, val, writeAXData(port, id, P_GOAL_POSITION, val));
 }

 if (1) {
 // Move to 10
 printf("AX %d : set GOAL position to 10\n", id);
 int r = writeAXData(port, id, P_GOAL_POSITION, 10);
 if (r != AXT_OK) {
 printf("AX %d : set GOAL position error : %d\n", id, r);
 }
 sleep(2);
 printf("read AX %d PRESENT_POSITION returned %d\n", id, readAXData(port, id, P_PRESENT_POSITION));
 // Move to 600
 printf("AX %d : set GOAL position to 600\n", id);
 r = writeAXData(port, id, P_GOAL_POSITION, 600);
 if (r != AXT_OK) {
 printf("AX %d : set GOAL position error : %d\n", id, r);
 }
 sleep(2);
 printf("read AX %d PRESENT_POSITION returned %d\n", id, readAXData(port, id, P_PRESENT_POSITION));
 }

 if (0) {
 // Test ADC
 for (i = 0; i < 2; i++) {
 int adc = 1;
 int val = getADC(adc);
 printf("getADC %d returned %d\n", adc, val);
 }
 }
 printf("Exiting\n");
 return 0;
 }
 */
