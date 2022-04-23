/*!
 * \file
 * \brief Implémentation de la classe CCAx12Adc.
 */

#include "CCAx12Adc.hpp"

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstdint>

#include "../Log/Logger.hpp"

CCAx12Adc::CCAx12Adc() : //int i2c_bus_num, uint i2c_aAddr) ://TODO Une SEULE CARTE est possible a mettre en paramettre adresse + port
        i2c_CCAx12Adc_(1, false), connected_(false)
{
}

bool CCAx12Adc::connect(uint i2c_aAddr) {

    if (connected_)
        return connected_;

    //open i2c and setslave
    int err = i2c_CCAx12Adc_.begin(i2c_aAddr);
    if (err >= 0) connected_ = true;
    else {
        connected_ = false;
        logger().error() << "CCAx12Adc::connect() : NOT CONNECTED!" << logs::end;
    }


//    while(1)
//    {
//        setLedOn(2);
//        int present = pingAX(3);
//        int getV = readAXData(3, P_PRESENT_VOLTAGE);
//        logger().error() << "p3 =" << present
//                <<" V=" << getV
//                << logs::end;
//        usleep(200000);
//        setLedOff(2);
//        usleep(200000);
//    }


        //i2c_CCAx12Adc_.setSlaveAddr(AX12ADC_ADDR);
//        setLedOn(1);
//        while (1) {
//            usleep(1000 * 1000);
//            int pingg = pingAX(53);
//            int r = writeAXData(53, P_GOAL_POSITION, 512);
//            int getV5 = readAXData(53, P_PRESENT_LOAD)-1024;
//            logger().error() << "position =" << getV5 << logs::end;
//
//        }

//        int present = pingAX(51);
//        if (present == 0) {
//            setLedOn(2);
//            connected_ = true;
//        } else {
//            logger().error() << "CCAx12Adc::begin() AX 51 NOT present !! " << present << logs::end;
//            setLedOff(2);
//            connected_ = false;
//        }
//        connected_ = true;
        /*
         int i = 0;
         logger().error() << "TEST" << logs::end;
         while (1) {
         //   int r = writeAXData(182, P_GOAL_POSITION, i);
         int getV = readAXData(51, P_PRESENT_VOLTAGE);
         logger().error() << "p =" << getV << logs::end;
         //i++;
         int present = pingAX(51);
         logger().error() << "present51 =" << present << logs::end;

         int getV5 = readAXData(5, P_PRESENT_VOLTAGE);
         logger().error() << "p =" << getV5 << logs::end;
         //i++;
         int present5 = pingAX(5);
         logger().error() << "present5 =" << present5 << logs::end;

         int getV102 = readAXData(102, P_PRESENT_VOLTAGE);
         logger().error() << "p =" << getV102 << logs::end;
         //i++;
         int present102 = pingAX(102);
         logger().error() << "present102 =" << present102 << logs::end;



         int voltage_9 = getADC(9);
         int voltage_8 = getADC(8);
         logger().error() << "volt9 = " << voltage_9 << "    volt8 = " << voltage_8 << logs::end;
         usleep(200000);
         }
         */
        /*
         //test read ax
         writeAXData(51, P_GOAL_SPEED, 10);
         int r = writeAXData(51, P_GOAL_POSITION, 512);
         while (1)
         {
         int moving = readAXData(51, P_MOVING);
         logger().error() << "moving =" << moving << logs::end;
         //usleep(100000);
         }
         */
        /*
         int moving = 0;

         writeAXData(5, P_GOAL_SPEED, 40);

         int r = writeAXData(5, P_GOAL_POSITION, 512);
         logger().error() << "setPosition P_GOAL_POSITION =" << r << logs::end;

         moving = readAXData(5, P_MOVING);
         while (moving >= 1)
         {
         moving = readAXData(5, P_MOVING);
         logger().error() << "moving =" << moving << logs::end;
         //usleep(100000);
         }
         logger().error() << "end moving =" << moving << logs::end;
         writeAXData(5, P_TORQUE_ENABLE, 0);
         */
   //}
    return connected_;
}

int CCAx12Adc::getAddressSize(int address) {
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

// Set the led on
// @param led :  1 - 10
void CCAx12Adc::setLedOn(int led) {
    if (!connected_) {
        logger().error() << "CCAx12Adc::setLedOn() : BOARD NOT CONNECTED !" << logs::end;
        return;
    }
    mutex_.lock();

    int w1 = write(CMD_SET_LED_ON);
    if (w1 < 0) {
        mutex_.unlock();
        logger().error() << "setLedOn (led:"<< led <<") I2C bus error (first write), check I2C cables" << logs::end;
        //printf("setLedOn (led:%d) I2C bus error (first write), check I2C cables\n", led);
        return;
    }
    int w2 = write(led);
    if (w2 < 0) {
        mutex_.unlock();
        logger().error() << "setLedOn (led:"<< led <<") I2C bus error (second write), check I2C cables" << logs::end;
        //printf("setLedOn (led:%d) I2C bus error (second write), check I2C cables\n", led);
        return;
    }
    mutex_.unlock();
}

// Set the led off
// @param led :  1 - 10
void CCAx12Adc::setLedOff(int led) {
    if (!connected_) {
        logger().error() << "CCAx12Adc::setLedOff() : BOARD NOT CONNECTED !" << logs::end;
        return;
    }
    mutex_.lock();
    int w1 = write(CMD_SET_LED_OFF);
    if (w1 < 0) {
        mutex_.unlock();
        logger().error() << "setLedOn (led:"<< led <<") I2C bus error (first write), check I2C cables" << logs::end;
        //printf("setLedOff (led:%d) I2C bus error (first write), check I2C cables\n", led);
        return;
    }
    int w2 = write(led);
    if (w2 < 0) {
        mutex_.unlock();
        logger().error() << "setLedOn (led:"<< led <<") I2C bus error (second write), check I2C cables" << logs::end;
        //printf("setLedOff (led:%d) I2C bus error (second write), check I2C cables\n", led);
        return;
    }
    mutex_.unlock();
}

// Get the ADC value (12bits)
// @param ADC :  0 - 9
// @returns value : 0 - 4095
int CCAx12Adc::getADC(int adc) {
    if (!connected_) {
        logger().error() << "CCAx12Adc::getADC() : BOARD NOT CONNECTED !" << logs::end;
        return -1;
    }
    if (adc >= 10 || adc < 0) {
        logger().error() << "CCAx12Adc::getADC() : bad number adc !" << logs::end;
        return -2;
    }
    int err = 0;
    mutex_.lock();
    // TODO check apres chaque read/write
    err |= write(CMD_GET_ADC);
    err |= write(adc);
    if (err < 0) {
        mutex_.unlock();
        logger().error() << "getADC (id:"<< adc <<") I2C bus error (write err=" << err << "), check I2C cables" << logs::end;
        //printf("getADC (id:%d) I2C bus error (write err=%d), check I2C cables\n", adc, err);
        return -10;
    }

    int low = read();
    if (low < 0) {
        mutex_.unlock();
        logger().error() << "getADC (id:"<< adc <<") I2C bus error (read low byte=" << low << "), check I2C cables" << logs::end;
        //printf("getADC (id:%d) I2C bus error (read low byte=%d), check I2C cables\n", adc, low);
        return -3;
    }
    int high = read();
    if (high < 0) {
        mutex_.unlock();
        logger().error() << "getADC (id:"<< adc <<") I2C bus error (read high byte=" << high << "), check I2C cables" << logs::end;
        //printf("getADC (id:%d) I2C bus error (read high byte=%d), check I2C cables\n", adc, high);
        return -4;
    }
    mutex_.unlock();

    //printf("low:%d, high:%d \n", low, high);
    return convertToVoltage(high * 256 + low);
}

int CCAx12Adc::convertToVoltage(int adc_value) {

    double millivolts = adc_value * 5000.0 / 4095.0;

    return (int) millivolts;
}

// Ping a Dynamixel Servo
// @param id : the id of the dynamixel
// @returns 0 is servo is found
int CCAx12Adc::pingAX(int id) {

    int err = 0;
    mutex_.lock(); // TODO check apres chaque read/write
    err |= write(CMD_PING_AX);
    err |= write(id);

    if (err == -1) {
        mutex_.unlock();
        printf("\npingAX AX (id:%d) I2C bus error (writes)\n", id);
        return -1;
    }
    int d = read();
    if (d < 0) {
        mutex_.unlock();
        printf("\npingAX AX (id:%d) I2C bus error (d:%d)\n", id, d);
        return -11;
    }

    int status = read();
    if (status < 0) {
        mutex_.unlock();
        printf("\npingAX AX (id:%d) I2C bus error (status:%d)\n", id, status);
        return -22;
    }
    mutex_.unlock();
    //printf("\n--pingAX AX (id:%d) I2C bus error (status:%d)\n", id, status);

//    if (err == 253) {
//        printf("pingAX AX (id:%d) AX bus error (CRC error), check AX cables\n", id);
//        return -1; //checksum error
//    }
    return status;

    /*
     mutex_.lock(); // TODO check apres chaque read/write
     int w1 = write(CMD_PING_AX);
     int w2 = write(id);
     int err = read();
     mutex_.unlock();

     if (w1 < 0) {
     printf("pingAX AX (id:%d) I2C bus error (first write), check I2C cables\n", id);
     return -10;
     }
     if (w2 < 0) {
     printf("pingAX AX (id:%d) I2C bus error (second write), check I2C cables\n", id);
     return -20;
     }
     if (err == 253) {
     printf("pingAX AX (id:%d) AX bus error (CRC error), check AX cables\n", id);
     return -1; //checksum error
     }
     return err;
     */
}

// Read data (8bits or 16bits) at a specified address
// @param id : the id of the dynamixel
// @param address : address of the data
// @returns the data (negative value if error)
int CCAx12Adc::readAXData(int id, int address) {
    if (!connected_) {
        logger().error() << "CCAx12Adc::readAXData() : BOARD NOT CONNECTED !" << logs::end;
        return -1;
    } // TODO check apres chaque read/write
    int size = getAddressSize(address);

    int err = 0;
    mutex_.lock();
    err |= write(CMD_READ_AX);
    err |= write(id);
    err |= write(address);
    if (err == -1) {
        mutex_.unlock();
        printf("readAXData AX (id:%d) I2C bus error (writes), check I2C cables\n", id);
        return -1;
    }

    int low = read();
    if (low < 0) {
        mutex_.unlock();
        printf("readAXData AX (id:%d) I2C bus error (second read), check I2C cables\n", id);
        return -11;
    }
    if (size > 1) {
        int high = read();
        if (high < 0) {
            mutex_.unlock();
            printf("readAXData AX (id:%d) I2C bus error (third read), check I2C cables\n", id);
            return -12;
        }
        int status = read();
        if (status < 0) {
            mutex_.unlock();
            printf("readAXData AX (id:%d)(2 bytes) I2C bus error (AX status read), check I2C cables\n", id);
            return -22;
        }
        mutex_.unlock();

        if (status == 0) {
            //  printf("readAXData AX (id:%d) (2 bytes) (status %d) : %d (L:%x H:%x)\n", id, status, high * 256 + low, low, high);

            return high * 256 + low;
        }
        else {
            printf("readAXData ERROR AX (id:%d) (2 bytes) (status %d) : %d (L:%x H:%x)\n", id, status, high * 256 + low, low, high);
            return -42;
        }
    }
    else {
        int status = read();
        if (status < 0) {
            mutex_.unlock();
            printf("readAXData AX (id:%d)(1 byte)  I2C bus error (AX status read), check I2C cables\n", id);
            return -23;
        }

        mutex_.unlock();
        if (status == 0) {
            //printf("readAXData AX (id:%d) (1 byte) (status %d): %d \n", id, status, low);
            return low;
        }
        else {
            printf("readAXData ERROR AX (id:%d) (1 byte) (status %d): %d \n", id, status, low);
            return -43;
        }
    }
}

// Write data (8bits or 16bits) at a specified address
// @param id : the id of the dynamixel
// @param address : address of the data
// retourne 0 si ok, une erreur (negative si pb)
int CCAx12Adc::writeAXData(int id, int address, int data) {
    if (!connected_) {
        logger().error() << "CCAx12Adc::writeAXData() : BOARD NOT CONNECTED !" << logs::end;
        return -1;
    }

    int size = getAddressSize(address);

    int err = 0;

    mutex_.lock();
    err |= write(CMD_WRITE_AX);
    err |= write(id);
    err |= write(address);
    if (err == -1) {
        mutex_.unlock();
        printf("writeAXData AX (id:%d) I2C bus error (writes), check I2C cables\n", id);
        return -1;
    }
    uint8_t xlow = data & 0xff;
    err |= write(xlow);
    if (err == -1) {
        mutex_.unlock();
        printf("writeAXData AX (id:%d) I2C bus error (write low), check I2C cables\n", id);
        return -2;
    }
    if (size == 2) {
        uint8_t xhigh = (data >> 8);
        err |= write(xhigh);
        if (err == -1) {
            mutex_.unlock();
            printf("writeAXData AX (id:%d) I2C bus error (write high), check I2C cables\n", id);
            return -3;
        }
    }

    int error = read();
    mutex_.unlock();
    if (error < 0) {
        printf("writeAXData AX (id:%d) I2C bus error (read error status), check I2C cables\n", id);
        return -10;
    }

    if (error != 0) {
        printf("writeAXData AX (id:%d) AX bus error (error %d), check AX cables\n", id, error);
        return -error;
    }
    return 0;
}

//pas utilisé ?
//int CCAx12Adc::write_i2c(unsigned char command, unsigned char value) {
//    //return i2c_CCAx12Adc_.writeReg(command, value, 1);
//}

int CCAx12Adc::write(unsigned char command) {

    if (i2c_CCAx12Adc_.write(command)) {
        //printf("write > cmd=%d > error!\n", command);
        return -1;
    }
    return 0;
}

int CCAx12Adc::read() {

    unsigned char a[1];
    a[0] = 0;
    if (i2c_CCAx12Adc_.read(a) < 0) {
        //printf("write_readI2c  > read  error!\n");
        return -1;
    }

    return a[0];
}



int CCAx12Adc::readRegs(uint8_t reg_address, uint8_t* data,uint8_t len) {
    int err = i2c_CCAx12Adc_.readReg(reg_address, data, len);
    return err;
}

int CCAx12Adc::writeRegs(uint8_t reg_address, uint8_t* values) {
    //uint8_t values[] = { value };
    int err = i2c_CCAx12Adc_.writeReg(reg_address, values, sizeof(values));
    return err;
}

//int CCAx12Adc::writeReg(uint8_t reg_address, uint8_t value) {
//    uint8_t values[] = { value };
//    int err = i2c_CCAx12Adc_.writeReg(reg_address, values, sizeof(values));
//    return err;
//}

