/*!
 * \file
 * \brief Implémentation de la classe CCAx12TeensyDriverTest.
 */

#include "CCAx12TeensyDriverTest.hpp"

#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <cerrno>
#include <cstdio>
#include <cstring>

#include "../../src/Driver-EV3/CCAx12Teensy.hpp"
#include "../../src/Driver-EV3/i2c-dev.h"

void test::CCAx12TeensyDriverTest::suite() {
    this->testSet();

}

void test::CCAx12TeensyDriverTest::testSet() {

    //int conn = CCAx12Teensy::instance().connect(0x08);

    //code chaff test
    int data;
    int addr = (0x08);
    int status = initI2C(addr);
    printf("Setup communication to I2C slave address %d\n", addr);

    if (status < 0) {
        printf("Can't setup the I2C device\n");
        //return -1;
    }
    int id = 2;
    int i;
    while (1) {
        setLedOn(5);
        usleep(500000);
        setLedOff(5);
        usleep(500000);
    }
    if (0) {
        for (i = 0; i < 10; i++) {
            printf("Flashing LED %d\n", i + 1);
            setLedOn(i + 1);
            usleep(500000);
            setLedOff(i + 1);
            usleep(20000);
        }
    }

    this->assert(true, "OK");
}

int test::CCAx12TeensyDriverTest::initI2C(uint8_t slaveAddress) {
    slave_addr_i2c = slaveAddress;
    fd_i2c = open("/dev/i2c-in2", O_RDWR);
    if (fd_i2c == -1) {
        fprintf(stderr, "initI2C: failed to open: %s\n", strerror(errno));
        return -1;
    }
    return 0;
}

uint8_t test::CCAx12TeensyDriverTest::readI2C(char cmd, char * buf, uint16_t bytesToRead) {
    struct i2c_msg messages[] = { { slave_addr_i2c, 0, 1, &cmd }, { slave_addr_i2c, I2C_M_RD, bytesToRead, buf }, };
    struct i2c_rdwr_ioctl_data ioctl_data = { messages, 2 };

    if (ioctl(fd_i2c, I2C_RDWR, &ioctl_data) != 0) {
        fprintf(stderr, "readI2C : failed to ioctl: %s\n", strerror(errno));
        return -1;
    }
    printf("readI2C : %d\n", bytesToRead);
    for (int i = 0; i < bytesToRead; i++) {
        printf("readI2C:%d : %d\n", i, buf[i]);
    }
    return 0;
}

int test::CCAx12TeensyDriverTest::writeI2C(char * data, uint16_t bytesToWrite) {

    struct i2c_msg message = { slave_addr_i2c, 0, bytesToWrite, data };
    struct i2c_rdwr_ioctl_data ioctl_data = { &message, 1 };

    if (ioctl(fd_i2c, I2C_RDWR, &ioctl_data) != 0) {
        fprintf(stderr, "writeI2C : failed to ioctl: %s\n", strerror(errno));
        return -1;
    }
    return 0;
}

int test::CCAx12TeensyDriverTest::getAddressSize(int address) {
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

uint8_t test::CCAx12TeensyDriverTest::computeCRC(char *buf, uint16_t length) {
    uint8_t result = 5;
    for (int i = 0; i < length; i++) {
        result = result * 3 + buf[i];
    }
    return result;
}

int test::CCAx12TeensyDriverTest::checkCRC(char* buf, uint16_t length) {
    uint8_t crc = buf[length - 1];
    uint8_t computed = computeCRC(buf, length - 1);
    if (crc == computed) {
        return 0;
    }
    printf("checkCRC failed : %d != %d\n", crc, computed);
    return -1;
}

int test::CCAx12TeensyDriverTest::setLed(uint8_t led, bool on) {
    int cmd = AXT_CMD_SET_LED_OFF;
    if (on) {
        cmd = AXT_CMD_SET_LED_ON;
    }

    // Send message
    char buffer[3];
    buffer[0] = cmd;
    buffer[1] = led;
    buffer[2] = computeCRC(buffer, 2);
    int err_write = writeI2C(buffer, 3);
    if (err_write < 0) {
        return AXT_ERROR_SEND_CMD;
    }
    // Read response
    char status[20];
    for(int i=0;i<20;i++){
        status[i]=66;
    }
    int err_read = readI2C(AXT_STATUS, status, 2);
   // readI2C(AXT_STATUS, status, 1);
   // readI2C(AXT_STATUS, status, 1);
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
int test::CCAx12TeensyDriverTest::setLedOn(uint8_t led) {
    return setLed(led, true);
}

// Set the led off
// @param led :  1 - 10
int test::CCAx12TeensyDriverTest::setLedOff(uint8_t led) {
    return setLed(led, false);
}

// Get the ADC value (12bits)
// @param ADC :  0 - 9
// @returns value : 0 - 4095
int test::CCAx12TeensyDriverTest::getADC(uint8_t adc) {
    // Send message
    char buffer[3];
    buffer[0] = AXT_CMD_GET_ADC;
    buffer[1] = adc;
    buffer[2] = computeCRC(buffer, 2);
    int err_write = writeI2C(buffer, 3);
    if (err_write < 0) {
        return AXT_ERROR_SEND_CMD;
    }
    // Read response
    char status[4];
    int err_read = readI2C(AXT_STATUS, status, 4);
    if (err_read < 0) {
        return AXT_ERROR_READ_STATUS;
    }
    if (checkCRC(status, 4) != 0) {
        return AXT_ERROR_CRC;
    }
    return status[1] + 256 * status[2];

}

int test::CCAx12TeensyDriverTest::pingAX(uint8_t port, uint8_t id) {
    // Send message
    char buffer[4];
    buffer[0] = AXT_CMD_PING_AX;
    buffer[1] = port;
    buffer[2] = id;
    buffer[3] = computeCRC(buffer, 3);
    int err_write = writeI2C(buffer, 4);
    if (err_write < 0) {
        return AXT_ERROR_SEND_CMD;
    }
    // Read response
    char status[2];
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

int test::CCAx12TeensyDriverTest::readAXData(uint8_t port, uint8_t id, uint8_t address) {
    // Send message
    char buffer[5];
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
    char status[4];
    int err_read = readI2C(AXT_STATUS, status, 4);
    if (err_read < 0) {
        return AXT_ERROR_READ_STATUS;
    }
    if (checkCRC(status, 4) != 0) {
        return AXT_ERROR_CRC;
    }
    return status[1] + 256 * status[2];
}

int test::CCAx12TeensyDriverTest::writeAXData(uint8_t port, uint8_t id, uint8_t address, uint16_t data) {
    uint8_t xlow = data & 0xff;
    uint8_t xhigh = (data >> 8);

    char buffer[7];
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
    char status[2];
    int err_read = readI2C(AXT_STATUS, status, 2);
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
 int status=initI2C(addr);
 printf("Setup communication to I2C slave address %d\n", addr);

 if (status < 0) {
 printf("Can't setup the I2C device\n");
 return -1;
 }
 int id = 2;
 int i;
 while(1) {
 setLedOn(5);
 usleep(500000);
 }
 if (0) {
 for (i = 0; i < 10; i++) {
 printf("Flashing LED %d\n", i + 1);
 setLedOn(i + 1);
 usleep(50000);
 setLedOff(i + 1);
 usleep(2000);
 }
 }

 int port=1;
 if(0) {
 for (i = 1; i < 5; i++) {
 int r=pingAX(1,i);
 if(r==0) {
 printf("Ping AX %d : AX FOUND\n", i);
 } else if(r==AXT_AX_MISSING) {
 printf("Ping AX %d : AX NOT FOUND\n", i);
 } else {
 printf("Ping AX %d : error %d\n", i,r);
 }
 }
 }
 if(1) {
 printf("Testing AX on port %d, id %d\n",port,id);
 printf("read AX %d MODEL_NUMBER returned %d\n", id, readAXData(port,id, P_MODEL_NUMBER));
 printf("read AX %d MODEL_VERSION returned %d\n", id, readAXData(port,id, P_VERSION));
 printf("read AX %d ID returned %d\n", id, readAXData(port,id, P_ID));
 printf("read AX %d BAUD_RATE returned %d\n", id, readAXData(port,id, P_BAUD_RATE));
 }
 if(1) {
 printf("read AX %d RETURN_DELAY_TIME returned %d\n", id, readAXData(port,id, P_RETURN_DELAY_TIME));
 printf("read AX %d CW_ANGLE_LIMIT returned %d\n", id, readAXData(port,id, P_CW_ANGLE_LIMIT));
 printf("read AX %d CCW_ANGLE_LIMIT returned %d\n", id, readAXData(port,id, P_CCW_ANGLE_LIMIT));
 printf("read AX %d SYSTEM_DATA2 returned %d\n", id, readAXData(port,id, P_SYSTEM_DATA2));
 printf("read AX %d LIMIT_TEMPERATURE returned %d\n", id, readAXData(port,id, P_LIMIT_TEMPERATURE));
 printf("read AX %d DOWN_LIMIT_VOLTAGE returned %d\n", id, readAXData(port,id, P_DOWN_LIMIT_VOLTAGE));
 printf("read AX %d UP_LIMIT_VOLTAGE returned %d\n", id, readAXData(port,id, P_UP_LIMIT_VOLTAGE));
 printf("read AX %d MAX_TORQUE returned %d\n", id, readAXData(port,id, P_MAX_TORQUE));
 printf("read AX %d RETURN_LEVEL returned %d\n", id, readAXData(port,id, P_RETURN_LEVEL));
 printf("read AX %d ALARM_LED returned %d\n", id, readAXData(port,id, P_ALARM_LED));
 printf("read AX %d ALARM_SHUTDOWN returned %d\n", id, readAXData(port,id, P_ALARM_SHUTDOWN));
 printf("read AX %d OPERATING_MODE returned %d\n", id, readAXData(port,id, P_OPERATING_MODE));
 printf("read AX %d DOWN_CALIBRATION returned %d\n", id, readAXData(port,id, P_DOWN_CALIBRATION));
 printf("read AX %d UP_CALIBRATION returned %d\n", id, readAXData(port,id, P_UP_CALIBRATION));
 printf("read AX %d TORQUE_ENABLE returned %d\n", id, readAXData(port,id, P_TORQUE_ENABLE));
 printf("read AX %d LED returned %d\n", id, readAXData(port,id, P_LED));
 printf("read AX %d CW_COMPLIANCE_MARGIN returned %d\n", id, readAXData(port,id, P_CW_COMPLIANCE_MARGIN));
 printf("read AX %d CCW_COMPLIANCE_MARGIN returned %d\n", id, readAXData(port,id, P_CCW_COMPLIANCE_MARGIN));
 printf("read AX %d CW_COMPLIANCE_SLOPE returned %d\n", id, readAXData(port,id, P_CW_COMPLIANCE_SLOPE));
 printf("read AX %d CCW_COMPLIANCE_SLOPE returned %d\n", id, readAXData(port,id, P_CCW_COMPLIANCE_SLOPE));
 printf("read AX %d GOAL_POSITION returned %d\n", id, readAXData(port,id, P_GOAL_POSITION));
 printf("read AX %d GOAL_SPEED returned %d\n", id, readAXData(port,id, P_GOAL_SPEED));
 printf("read AX %d TORQUE_LIMIT returned %d\n", id, readAXData(port,id, P_TORQUE_LIMIT));
 printf("read AX %d PRESENT_POSITION returned %d\n", id, readAXData(port,id, P_PRESENT_POSITION));
 printf("read AX %d PRESENT_SPEED returned %d\n", id, readAXData(port,id, P_PRESENT_SPEED));
 printf("read AX %d PRESENT_LOAD returned %d\n", id, readAXData(port,id, P_PRESENT_LOAD));
 printf("read AX %d PRESENT_VOLTAGE returned %d\n", id, readAXData(port,id, P_PRESENT_VOLTAGE));
 printf("read AX %d PRESENT_TEMPERATURE returned %d\n", id, readAXData(port,id, P_PRESENT_TEMPERATURE));
 printf("read AX %d REGISTERED_INSTRUCTION returned %d\n", id, readAXData(port,id, P_REGISTERED_INSTRUCTION));
 printf("read AX %d PAUSE_TIME returned %d\n", id, readAXData(port,id, P_PAUSE_TIME));
 printf("read AX %d MOVING returned %d\n", id, readAXData(port,id, P_MOVING));
 printf("read AX %d LOCK returned %d\n", id, readAXData(port,id, P_LOCK));
 printf("read AX %d PUNCH returned %d\n", id, readAXData(port,id, P_PUNCH));



 int val = 150;
 printf("read AX %d GOAL_POSITION %d returned %d\n", id, val, writeAXData(port,id, P_GOAL_POSITION, val));
 }

 if(1) {
 // Move to 10
 printf("AX %d : set GOAL position to 10\n",id);
 int r=writeAXData(port,id,P_GOAL_POSITION,10);
 if(r!=AXT_OK) {
 printf("AX %d : set GOAL position error : %d\n",id,r);
 }
 sleep(2);
 printf("read AX %d PRESENT_POSITION returned %d\n", id, readAXData(port,id, P_PRESENT_POSITION));
 // Move to 600
 printf("AX %d : set GOAL position to 600\n",id);
 r=writeAXData(port,id,P_GOAL_POSITION,600);
 if(r!=AXT_OK) {
 printf("AX %d : set GOAL position error : %d\n",id,r);
 }
 sleep(2);
 printf("read AX %d PRESENT"
 "_POSITION returned %d\n", id, readAXData(port,id, P_PRESENT_POSITION));
 }

 if(0) {
 // Test ADC
 for (i = 0; i < 2; i++) {
 int adc = 1;
 int val=getADC(adc);
 printf("getADC %d returned %d\n", adc, val);
 }
 }
 printf("Exiting\n");
 return 0;
 }
 */

