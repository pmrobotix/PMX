//drivers...OPO
//http://www.sharp-world.com/products/device/lineup/data/pdf/datasheet/gp2y0e02_03_appl_e.pdf

#include "Gp2y0e02b.hpp"

#include <stdlib.h>
#include <unistd.h>
#include <cmath>

#include "../Log/Logger.hpp"

Gp2y0e02b::Gp2y0e02b(int bus, unsigned char addr) :
        i2c_gp2y0e02b_(bus), connected_gp2y0e02b_(false), shift_(0)
{
    logger().debug() << "Gp2y0e02b(" << reinterpret_cast<void*>(addr) << ") : gp2y0e02b CREATE" << logs::end;

    address_ = addr;
    i2c_gp2y0e02b_.setSlaveAddr(addr);

    //read shift
    shift_ = readRegByte(SHIFT_ADDR);

    if (shift_ <= 0) {
        logger().error() << "Gp2y0e02b(" << reinterpret_cast<void*>(addr) << ") : gp2y0e02b NOT CONNECTED !" << logs::end;
        connected_gp2y0e02b_ = false;
    } else
        connected_gp2y0e02b_ = true;
    logger().info() << "CREATE ok" << logs::end;
}

int Gp2y0e02b::getDistanceMm()
{
    if (!connected_gp2y0e02b_)
        return -99;
    unsigned char data[2];
    int r = readReg2Bytes(DISTANCE_ADDR, data);
    if (r < 0)
        logger().error() << "ERROR getDistanceMm() readReg2Bytes r=" << r << logs::end;
    unsigned char high = data[0];
    unsigned char low = data[1];

    logger().debug() << "high: " << (int) high << " \tlow: " << (int) low << logs::end;
    int distance_mm = ((high * 16 + low) / 16) * 10 / (int) std::pow(2, shift_); // Calculate the range in CM
    logger().debug() << "distance: " << address_ << " mm = " << distance_mm << logs::end;

    if (distance_mm >= 637)
        return -1;
    return distance_mm;
}

void Gp2y0e02b::getInfoData(unsigned int *spotSize, unsigned int *spotSym)
{
    if (!connected_gp2y0e02b_)
        return;
    unsigned char data[3]; //c + a + b
    int r = readReg3Bytes(RIGHT_EDGE_ADDR, data);
    if (r < 0)
        logger().error() << "ERROR getInfoData() readReg3Bytes" << logs::end;

    *spotSize = (int) (data[0] - data[1]); //c - a;
    *spotSym = (int) (std::abs((int) (data[0] - data[1]) - 2 * data[2]));

}

void Gp2y0e02b::EFuseSlaveID(unsigned char desiredAddress, bool pin_activated)
{
    if (!connected_gp2y0e02b_) {
        logger().error() << "ERROR Gp2y0e02b Not connected" << logs::end;
        return;
    }

    int r = 0;
    /* ----- Stage 1 ----- */
    logger().info() << "Stage 1 started." << logs::end;
    //writeSequence(byte(ADDRESS), byte(0xEC), byte(0xFF));
    r = writeRegByte(0xEC, 0xFF);
    logger().info() << "Data = 0xFF is set in Address = 0xEC" << logs::end;

    if (pin_activated) {
//    digitalWrite(4, HIGH);
//    Serial.println();
        logger().info() << "3.3V is applied in the Vpp terminal" << logs::end;
    }

    /* ----- Stage 2 ------ */
    logger().info() << "Stage 2 started." << logs::end;
    r = writeRegByte(0xC8, 0x00);
    logger().info() << "Data = 0x00 is set in Address = 0xC8" << logs::end;
//    Serial.println("Stage 2 started.");
//    writeSequence(byte(ADDRESS), byte(0xC8), byte(0x00));
//    Serial.println();

    /* ----- Stage 3 ------ */
    logger().info() << "Stage 3 started." << logs::end;
    r = writeRegByte(0xC9, 0x45);
    logger().info() << "Data = 0x45 is set in Address = 0xC9" << logs::end;
//    Serial.println("Stage 3 started.");
//    writeSequence(byte(ADDRESS), byte(0xC9), byte(0x45));
//    Serial.println();

    /* ----- Stage 4 ------ */
    /* THIS IS WHERE THE ADDRESS WILL BE SET! */
    logger().info() << "Stage 4 started." << logs::end;
    r = writeRegByte(0xCD, (desiredAddress >> 4));
    logger().info() << "Data = SETADDR >> 4 is set in Address = 0xCD" << logs::end;
//      Serial.println("Stage 4 started.");
//      writeSequence(byte(ADDRESS), byte(0xCD), byte(newID >> 4));
//      Serial.println("Data = SETADDR >> 4 is set in Address = 0xCD");

    /* ----- Stage 5 ------ */
    logger().info() << "Stage 5 started." << logs::end;
    r = writeRegByte(0xCA, 0x01);
    logger().info() << "Data = 0x01 is set in Address = 0xCA" << logs::end;
    logger().info() << "Wait for 500 us" << logs::end;
    usleep(500);
//    Serial.println("Stage 5 started.");
//    writeSequence(byte(ADDRESS), byte(0xCA), byte(0x01));
//    Serial.println("Data = 0x01 is set in Address = 0xCA");
//    Serial.println("Wait for 500 us");
//    delayMicroseconds(500);

    /* ----- Stage 6 ------ */
    logger().info() << "Stage 6 started." << logs::end;
    r = writeRegByte(0xCA, 0x00);
    logger().info() << "Data = 0x00 is set in Address = 0xCA" << logs::end;
//    Serial.println("Stage 6 started.");
//    writeSequence(byte(ADDRESS), byte(0xCA), byte(0x00));
//    Serial.println("Data = 0x00 is set in Address = 0xCA");
    if (pin_activated) {
        //    digitalWrite(4, LOW);
        //    Serial.println();
        logger().info() << "Vpp terminal grounded." << logs::end;
    }

    /* ----- Stage 7 ------ */
    logger().info() << "Stage 7 started." << logs::end;
    r = writeRegByte(0xEF, 0x00);
    logger().info() << "Data = 0x00 is set in Address = 0xEF" << logs::end;
//    Serial.println("Stage 7 started.");
//    writeSequence(byte(ADDRESS), byte(0xEF), byte(0x00));
//    Serial.println();

    r = writeRegByte(0xC8, 0x40);
    logger().info() << "Data = 0x40 is set in Address = 0xC8" << logs::end;
//    writeSequence(byte(ADDRESS), byte(0xC8), byte(0x40));
//    Serial.println();

    r = writeRegByte(0xC8, 0x00);
    logger().info() << "Data = 0x00 is set in Address = 0xC8" << logs::end;
//    writeSequence(byte(ADDRESS), byte(0xC8), byte(0x00));
//    Serial.println();

    /* ----- Stage 8 ------ */
    logger().info() << "Stage 8 started." << logs::end;
    r = writeRegByte(0xEE, 0x06);
    logger().info() << "Data = 0x06 is set in Address = 0xEE" << logs::end;
//    Serial.println("Stage 8 started.");
//    writeSequence(byte(ADDRESS), byte(0xEE), byte(0x06));
//    Serial.println();

    /* ----- Stage 9 ------ */
    logger().info() << "Stage 9 started." << logs::end;
    r = writeRegByte(0xEC, 0xFF);
    logger().info() << "Data = 0xFF is set in Address = 0xEC" << logs::end;
//    Serial.println("Stage 9 started.");
//    writeSequence(byte(ADDRESS), byte(0xEC), byte(0xFF));
//    Serial.println();

    r = writeRegByte(0xEF, 0x03);
    logger().info() << "Data = 0x03 is set in Address = 0xEF" << logs::end;
//    writeSequence(byte(ADDRESS), byte(0xEF), byte(0x03));
//    Serial.println();

    logger().info() << "Read out the data in Address = 0x27." << logs::end;
    logger().info() << "Data: 0B" << logs::end;
    r = readRegByte(0x27);
    logger().info() << "r=" << r << logs::end;
//    Serial.println();
//    Serial.print("Data: 0B");
//    Serial.println(readSequence1(byte(ADDRESS), byte(0x27)), BIN);

    r = writeRegByte(0xEF, 0x00);
    logger().info() << "Data = 0x03 is set in Address = 0xEF" << logs::end;
//    writeSequence(byte(ADDRESS), byte(0xEF), byte(0x00));
//    Serial.println("Data = 0x00 is set in Address = 0xEF");

    r = writeRegByte(0xEC, 0x7F);
    logger().info() << "Data = 0x03 is set in Address = 0xEF" << logs::end;
//    writeSequence(byte(ADDRESS), byte(0xEC), byte(0x7F));
//    Serial.println("Data = 0x7F is set in Address = 0xEC");

}

int Gp2y0e02b::ScanBus()
{
    logger().debug() << "I2C Scanner starting... " << logs::end;
    int nDevices;
    unsigned char error, address;
    nDevices = 0;
    for (address = 0; address < 127; address++) {
        error = readAddr(address);

        if (error == 0) {

            if (address < 16)
                logger().error() << "I2C device found at address 0x0" << (address << 1) << " !" << logs::end;
            else
                logger().error() << "I2C device found at address 0x" << (address << 1) << " !" << logs::end;

            nDevices++;
        } else if (error == 4) {

            if (address < 16)
                logger().error() << "Unknown error at address 0x0" << (address << 1) << " !" << logs::end;
            else
                logger().error() << "Unknown error at address 0x" << (address << 1) << " !" << logs::end;

        }
    }
    if (nDevices == 0) {
        logger().info() << "No I2C devices found!" << logs::end;
        return 0;
    } else {
        logger().info() << nDevices << " devices found. I2C Scanner done." << logs::end;
        return nDevices;
    }
}
unsigned char Gp2y0e02b::readAddr(unsigned char address)
{
    return (unsigned char) i2c_gp2y0e02b_.read(&address, 0);
}
unsigned char Gp2y0e02b::readRegByte(unsigned char command)
{
    return (unsigned char) i2c_gp2y0e02b_.readRegByte(command);
}
int Gp2y0e02b::readReg2Bytes(unsigned char command, unsigned char *aData)
{
    return (int) i2c_gp2y0e02b_.readReg(command, aData, 2);
}
int Gp2y0e02b::readReg3Bytes(unsigned char command, unsigned char *aData)
{
    return (int) i2c_gp2y0e02b_.readReg(command, aData, 3);
}

int Gp2y0e02b::writeRegByte(unsigned char command, unsigned char value)
{
    return i2c_gp2y0e02b_.writeRegByte(command, value);
}
int Gp2y0e02b::writeReg2Bytes(unsigned char command, unsigned char value1, unsigned char value2)
{
    unsigned char data[2];
    data[0] = value1;
    data[1] = value2;
    return (int) i2c_gp2y0e02b_.writeReg(command, data, 2);
}

