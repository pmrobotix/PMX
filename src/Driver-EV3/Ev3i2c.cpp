/*
 * Ev3i2c.cpp
 *
 *  Created on: Feb 15, 2021
 *      Author: pmx
 */

#include "Ev3i2c.hpp"

#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

#include "i2c-dev.h"

Ev3i2c::Ev3i2c(uint i2c_bus_num, bool skipFirstByte) {
    bus_num_ = i2c_bus_num;
    skipFirstByte_ = skipFirstByte;
    address_ = 0;

    //Set up the filename of the I2C Bus. Choose appropriate bus for Raspberry Pi Rev.
    char filename[13] = "/dev/i2c-in";
    if (i2c_bus_num == 4) {
        filename[11] = '4';
    }
    else if (i2c_bus_num == 1) {
        filename[11] = '1';
    }
    else if (i2c_bus_num == 2) {
        filename[11] = '2';
    }
    else if (i2c_bus_num == 3) {
        filename[11] = '3';
    }
    else {
        //bad busnumber
        std::cout << "Ev3i2c BAD I2C BUS" << std::endl;
        exit(0);
    }
    filename[12] = 0; //Add the null character onto the end of the array to make it a string

    *filename_ = *filename;
    filename_str_ = std::string(filename);

    lock();
    i2cHandle_ = open(filename, O_RDWR); //Open the i2c file descriptor in read/write mode
    unlock();
    if (i2cHandle_ < 0) {
        std::cout << "Ev3i2c Can't open I2C BUS" << std::endl; //If there's an error opening this, then display it.
    }
//    lock();
//    std::cout << "Ev3i2c I2C_PEC " << " = " << ioctl(i2cHandle_, I2C_PEC, 0) << std::endl;
//    unlock();

}

int Ev3i2c::begin(uint address) {
    address_ = address;
    lock();
    if (ioctl(i2cHandle_, I2C_SLAVE, address) < 0) { //Using ioctl set the i2c device to talk to address in the "addr" variable.
        std::cout << "Ev3i2c Can't set the I2C address for the slave device" << std::endl; //Display error setting the address for the slave.
    }
    //std::cout << "I2C_PEC " << std::hex << address << " = " << ioctl(i2cHandle_, I2C_PEC, 0) << std::endl;
    unlock();

    int err = ping();
    if (err < 0) {
        std::cout << "Ev3i2c Oh dear, something went wrong - No ping with 0x" << std::hex << address << " at " << filename_str_ << std::endl;
        //exit(EXIT_FAILURE);
        return err;
    }
    else return false;
}

Ev3i2c::~Ev3i2c() {
    if (i2cHandle_) { //If the I2C File handle is still open...
        close(i2cHandle_); //...Close it.
    }
}

int Ev3i2c::ping() {
    int q = 0;
    lock();
    q = i2c_smbus_write_quick(i2cHandle_, I2C_SMBUS_WRITE);
    unlock();
    return q;

//    uint8_t values[] = { 0x00 };
//    mutex_.lock();
//    int er = write(i2cHandle_, values, 1); //Write "length" number of bytes from the "data" buffer to the I2C bus.
//    mutex_.unlock();
//    return er;
}

int Ev3i2c::read(uint8_t *data) {
    int er = 0;
    lock();
    *data = i2c_smbus_read_byte(i2cHandle_);
    unlock();
    return er;
}

int Ev3i2c::write(uint8_t value) {

    int er = 0;
    lock();
    er = i2c_smbus_write_byte(i2cHandle_, value);
    unlock();
    return er;
}

/** @brief Read from given chip at a given register address (ioctl() method).
 *
 * @param byte_reg            register address
 * @param data                read data
 * @param length_data         data size
 *
 * @return error if negative
 */
int Ev3i2c::readReg(uint8_t byte_reg, uint8_t *data, uint8_t length_data) {

    if (skipFirstByte_) {
        uint8_t lengthWithPEC = length_data + 1;
        uint8_t dataPEC[lengthWithPEC];
        int len = 0;
        lock();
        len = i2c_smbus_read_i2c_block_data(i2cHandle_, byte_reg, lengthWithPEC, dataPEC);
        unlock();
        if (len != lengthWithPEC) {
            printf("\nEv3i2c readReg Error len != length_data)");
            return len; //-1 if error
        }
        for (int i = 0; i < length_data; i++) {
            //printf("\nreadReg len=%d length_data=%d reg=%02x i=%d dataPEC=%02x", len, length_data, byte_reg, i, dataPEC[i]);
            data[i] = dataPEC[i + 1];
            //data[i] = dataPEC[i];
            //printf("\nreadReg data  %02x", data[i]);
        }
    }
    else {

        int len = 0;
        lock();
        len = i2c_smbus_read_i2c_block_data(i2cHandle_, byte_reg, length_data, data);
        unlock();
        if (len != length_data) {
            printf("\nEv3i2c readReg Error len != length_data)");
            return len; //-1 if error
        }

//        for (int i = 0; i < length_data; i++) {
//            printf("\nreadReg len=%d length_data=%d reg=%02x i=%d data=%02x", len, length_data, byte_reg, i, data[i]);
//
//        }
    }
    return 0;

//    uint8_t lengthWithPEC = length_data + 1;
//    uint8_t dataPEC[lengthWithPEC];
//    uint8_t values[] = { byte_reg };
//    mutex_.lock();
//    int er = write(i2cHandle_, values, 1); //Write "length" number of bytes from the "data" buffer to the I2C bus.
//    if (er >= 0) {
//        er = read(i2cHandle_, dataPEC, lengthWithPEC);
//    }
//    mutex_.unlock();
//    if (er < 0) return -99; //-1 if error
//    for (int i = 0; i < length_data; i++)
//    {
//        printf("\nreadReg dataPEC %02x", dataPEC[i]);
//        data[i] = dataPEC[i + 1];
//        printf("\nreadReg data  %02x", data[i]);
//    }
//    return 0;

}


/** @brief Write from given chip at a given register address (ioctl() method).
 *
 * @param byte_reg            register address
 * @param values                write data
 * @param length_values         data size
 *
 * @return error if negative
 */
int Ev3i2c::writeReg(uint8_t byte_reg, uint8_t *values, uint8_t length_values) {
    int er = 0;
    lock();
    er = i2c_smbus_write_i2c_block_data(i2cHandle_, byte_reg, length_values, values);
    unlock();
    return er;

//    int er = 0;
//    uint8_t data[length_values + 1];
//    for (int i = 1; i <= length_values ; i++)
//        data[i] = values[i - 1];
//    data[0] = byte_reg;
//    mutex_.lock();
//    er = write(i2cHandle_, data, length_values + 1); //Write "length" number of bytes from the "data" buffer to the I2C bus.
//    mutex_.unlock();
//    return er;
}

//int Ev3i2c::i2cReadArduinoInt() {
//    const int arr_size = 2;
//    unsigned char tmp[arr_size]; //We know an Arduino Int is 2 Bytes.
//    int retval = -1;
//
//    if (i2cRead(tmp, arr_size) > 0) {
//        retval = tmp[1] << 8 | tmp[0]; //Using bit shifting, turn the 2 byte array into an Int.
//    }
//    return retval;
//}
//
//int Ev3i2c::i2cWriteArduinoInt(int input) {
//    const int arr_size = 2;
//    unsigned char tmp[arr_size]; //We know an Arduino Int is 2 Bytes.
//    int retval = 0;
//
//    tmp[0] = input; //get lowest 8 bits into the first part of the array;
//    tmp[1] = input >> 8; //get the highest 8 bits into the second part of the array;
//    retval = (i2cWrite(tmp, arr_size) > 0);
//    return retval;
//}

void Ev3i2c::lock() {

    if (bus_num_ == 1) mutex_1.lock();

    if (bus_num_ == 2) mutex_2.lock();

    if (bus_num_ == 3) mutex_3.lock();

    if (bus_num_ == 4) mutex_4.lock();
}

void Ev3i2c::unlock() {

    if (bus_num_ == 1) mutex_1.unlock();

    if (bus_num_ == 2) mutex_2.unlock();

    if (bus_num_ == 3) mutex_3.unlock();

    if (bus_num_ == 4) mutex_4.unlock();
}
