//drivers...OPO

#include "BeaconSensors.hpp"

#include <stdlib.h>
#include <unistd.h>
#include <cmath>

#include "../Log/Logger.hpp"

BeaconSensors::BeaconSensors(int bus, unsigned char i2c_addr) :
        i2c_BeaconSensors_(bus), connected_BeaconSensors_(false) //, shift_(0)
{
    logger().debug() << "BeaconSensors(" << reinterpret_cast<void*>(i2c_addr) << ") : BeaconSensors init" << logs::end;

    //logger().error() << "BeaconSensors(" << reinterpret_cast<void*>(i2c_addr) << ") : BeaconSensors testing...." << logs::end;

    i2c_address_ = i2c_addr;
    int err = i2c_BeaconSensors_.setSlaveAddr(i2c_addr);
    if (err >= 0)
       connected_BeaconSensors_ = true;
/*
    while(1)
    {
        int nbofbot = readRegByte(NUMOFBOTS_BeaconSensors);
            logger().error() << "BeaconSensors nbofbot=" << nbofbot << logs::end;
    }*/
    /*
    //read shift
    int nbofbot = readRegByte(NUMOFBOTS_BeaconSensors);
    logger().error() << "BeaconSensors nbofbot=" << nbofbot << logs::end;
    if (nbofbot >= 250) {
        logger().error() << "BeaconSensors(" << reinterpret_cast<void*>(i2c_addr) << ") : BeaconSensors NOT CONNECTED !" << logs::end;
        connected_BeaconSensors_ = false;
    }
    else if ((nbofbot >= 0) && (nbofbot <= 10))
    {
        connected_BeaconSensors_ = true;
        logger().debug() << "Beacon available" << logs::end;

    }*/
}


void BeaconSensors::display(int number) {
    uint8_t n = (uint8_t) number;
    int err = i2c_BeaconSensors_.writeReg(0x02, &n, 1);
    if (err < 0) {
        logger().error() << "BeaconSensors::display() : error writereg err=" << err << logs::end;
    }
}


Registers BeaconSensors::getData() {
    unsigned char buf[18] = { 0 };
    Registers regs;

    int err = readRegnBytes(DATA_BeaconSensors, buf, 18);

    if (err < 0) {
        logger().error() << "BeaconSensors : readRegnBytes ERROR !" << logs::end;
        regs.flags = 0xFF; //ERROR
        return regs;
    }

    regs.flags = buf[0];
    regs.nbDetectedBots = buf[1];
    regs.c1_mm = buf[2] | (buf[3] << 8); //front
    regs.c2_mm = buf[4] | (buf[5] << 8); //front
    regs.c3_mm = buf[6] | (buf[7] << 8); //front
    regs.c4_mm = buf[8] | (buf[9] << 8); //front
    regs.c5_mm = buf[10] | (buf[11] << 8); //back
    regs.c6_mm = buf[12] | (buf[13] << 8); //back
    regs.c7_mm = buf[14] | (buf[15] << 8); //back
    regs.c8_mm = buf[16] | (buf[17] << 8); //back

    logger().debug() << "data = FLAGS:"
            << (int) regs.flags
            << " NB:"
            << (int) regs.nbDetectedBots
            << " C1:"
            << regs.c1_mm
            << " "
            << regs.c2_mm
            << " "
            << regs.c3_mm
            << " "
            << regs.c4_mm
            << " "
            << regs.c5_mm
            << " "
            << regs.c6_mm
            << " "
            << regs.c7_mm
            << " C8:"
            << regs.c8_mm
            << logs::end;

    err = readRegnBytes(24, buf, 8);
    if (err < 0) {
        logger().error() << "BeaconSensors : readRegnBytes ERROR !" << logs::end;
        regs.flags = 0xFF; //ERROR
        return regs;
    }

    regs.x1_mm = buf[0] | (buf[1] << 8);
    regs.y1_mm = buf[2] | (buf[3] << 8);
    float2bytes_t a1;
    a1.bytes[0] = buf[4];
    a1.bytes[1] = buf[5];
    a1.bytes[2] = buf[6];
    a1.bytes[3] = buf[7];
    regs.a1_deg = a1.f;

    logger().debug() << " x1:" << regs.x1_mm << " " << regs.y1_mm << " " << regs.a1_deg << logs::end;

    err = readRegnBytes(32, buf, 8);
    if (err < 0) {
        logger().error() << "BeaconSensors : readRegnBytes ERROR !" << logs::end;
        regs.flags = 0xFF; //ERROR
        return regs;
    }
    regs.x2_mm = buf[0] | (buf[1] << 8);
    regs.y2_mm = buf[2] | (buf[3] << 8);
    float2bytes_t a2;
    a2.bytes[0] = buf[4];
    a2.bytes[1] = buf[5];
    a2.bytes[2] = buf[6];
    a2.bytes[3] = buf[7];
    regs.a2_deg = a2.f;

    logger().debug() << " x2:" << regs.x2_mm << " " << regs.y2_mm << " " << regs.a2_deg << logs::end;

    err = readRegnBytes(40, buf, 8);
    if (err < 0) {
        logger().error() << "BeaconSensors : readRegnBytes ERROR !" << logs::end;
        regs.flags = 0xFF; //ERROR
        return regs;
    }
    regs.x3_mm = buf[0] | (buf[1] << 8);
    regs.y3_mm = buf[2] | (buf[3] << 8);
    float2bytes_t a3;
    a3.bytes[0] = buf[4];
    a3.bytes[1] = buf[5];
    a3.bytes[2] = buf[6];
    a3.bytes[3] = buf[7];
    regs.a3_deg = a3.f;

    logger().debug() << " x3:" << regs.x3_mm << " " << regs.y3_mm << " " << regs.a3_deg << logs::end;

    err = readRegnBytes(48, buf, 8);
    if (err < 0) {
        logger().error() << "BeaconSensors : readRegnBytes ERROR !" << logs::end;
        regs.flags = 0xFF; //ERROR
        return regs;
    }
    regs.x4_mm = buf[0] | (buf[1] << 8);
    regs.y4_mm = buf[2] | (buf[3] << 8);
    float2bytes_t a4;
    a4.bytes[0] = buf[4];
    a4.bytes[1] = buf[5];
    a4.bytes[2] = buf[6];
    a4.bytes[3] = buf[7];
    regs.a4_deg = a4.f;

    logger().debug() << " x4:" << regs.x4_mm << " " << regs.y4_mm << " " << regs.a4_deg << logs::end;

    err = readRegnBytes(56, buf, 8);
    if (err < 0) {
        logger().error() << "BeaconSensors : readRegnBytes ERROR !" << logs::end;
        regs.flags = 0xFF; //ERROR
        return regs;
    }
    regs.d1_mm = buf[0] | (buf[1] << 8);
    regs.d2_mm = buf[2] | (buf[3] << 8);
    regs.d3_mm = buf[4] | (buf[5] << 8);
    regs.d4_mm = buf[6] | (buf[7] << 8);

    logger().debug() << " d1:" << regs.d1_mm << " " << regs.d2_mm << " " << regs.d3_mm << " " << regs.d4_mm << logs::end;

    return regs;
}

//________________________________________________

int BeaconSensors::ScanBus() {
    logger().debug() << "I2C Scanner starting... " << logs::end;
    int nDevices;
    unsigned char error, address;
    nDevices = 0;
    for (address = 0; address < 127; address++) {
        error = readAddr(address);

        if (error == 0) {

            if (address < 16) logger().error() << "I2C device found at address 0x0" << (address << 1) << " !" << logs::end;
            else logger().error() << "I2C device found at address 0x" << (address << 1) << " !" << logs::end;

            nDevices++;
        }
        else if (error == 4) {

            if (address < 16) logger().error() << "Unknown error at address 0x0" << (address << 1) << " !" << logs::end;
            else logger().error() << "Unknown error at address 0x" << (address << 1) << " !" << logs::end;

        }
    }
    if (nDevices == 0) {
        logger().info() << "No I2C devices found!" << logs::end;
        return 0;
    }
    else {
        logger().info() << nDevices << " devices found. I2C Scanner done." << logs::end;
        return nDevices;
    }
}

int BeaconSensors::readRegnBytes(unsigned char command, unsigned char *aData, uint nb) {
    return (int) i2c_BeaconSensors_.readReg(command, aData, nb + 1);
}

unsigned char BeaconSensors::readAddr(unsigned char address) { //TODO a modifier
    return (unsigned char) i2c_BeaconSensors_.read(&address, 0);
}

unsigned char BeaconSensors::readRegByte(unsigned char command) {
    return (unsigned char) i2c_BeaconSensors_.readRegByte(command);
}
/*
 int BeaconSensors::readReg2Bytes(unsigned char command, unsigned char *aData)
 {
 return (int) i2c_BeaconSensors_.readReg(command, aData, 2);
 }
 int BeaconSensors::readReg3Bytes(unsigned char command, unsigned char *aData)
 {
 return (int) i2c_BeaconSensors_.readReg(command, aData, 3);
 }

 int BeaconSensors::writeRegByte(unsigned char command, unsigned char value)
 {
 return i2c_BeaconSensors_.writeRegByte(command, value);
 }
 int BeaconSensors::writeReg2Bytes(unsigned char command, unsigned char value1, unsigned char value2)
 {
 unsigned char data[2];
 data[0] = value1;
 data[1] = value2;
 return (int) i2c_BeaconSensors_.writeReg(command, data, 2);
 }
 */
