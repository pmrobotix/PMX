/*
 * ev3i2c.h
 *
 *  Created on: Feb 15, 2021
 *      Author: pmx
 */

#ifndef DRIVER_EV3_EV3I2C_HPP_
#define DRIVER_EV3_EV3I2C_HPP_

#include <sys/types.h>
#include <cstdint>
#include <string>
#include "../Thread/Mutex.hpp"

class Ev3i2c {
private:
    int bus_num_;
    bool skipPEC_;
    //Var to hold the i2c communications handle,
    //...this will only be needed by the class itself, so it's private
    int i2cHandle_;
    char filename_[13];
    std::string filename_str_;
    uint address_;

    //utils::Mutex mutex_;

    utils::Mutex mutex_1;
    utils::Mutex mutex_2;
    utils::Mutex mutex_3;
    utils::Mutex mutex_4;


public:
    //Function: Constructor - First parameter is the I2C address of the slave being talked to.
    //...Second parameter is optional. Set true if using a Rev0 Raspberry Pi.
    //...Returns: Nothing.
    Ev3i2c(uint i2c_bus_num = 1, bool skipPEC = false);

    //Function: Destructor - Closes the I2C File handle.
    //...Returns: Nothing.
    ~Ev3i2c();

    //return 0 if ok, negatif if error
    int begin(uint address);

    int read(uint8_t *data);

    int write(uint8_t value);

    //Function: i2cRead - First parameter is a pointer to a char array. This will also contain the
    //...values passed back from the Arduino.
    //...Second parameter is the length of the array.
    //...Returns: Error value. > 0 is ok. < 0 means there was an error.
//    int i2cRead(uint8_t *data, uint8_t length);
    int readReg(uint8_t byte_reg, uint8_t *data, uint8_t length_data);

    //return 0; <0 if errors
    int ping();

    //Function: i2cWrite - First parameter is a pointer to a char array.containing the data to send.
    //...Second parameter is the length of the array.
    //...Returns: Error value. > 0 is ok. < 0 means there was an error.
    int writeReg(uint8_t byte_reg, uint8_t *values, uint8_t length_values);

    //Function: i2cWrite - No Parameters.
    //...Returns: An Int value sent from the arduino, or -1 if there was an error.
   // int i2cReadArduinoInt();

    //Function: i2cWrite - First parameter the int to send to the array.
    //...Returns: Error value. > 0 is ok. < 0 means there was an error.
    //int i2cWriteArduinoInt(int input);

    //Lock par bus i2c suivant l'ID du bus i2c de EV3 (de 1 à 4)
    void lock();

    //unlock par bus I2C suivant l'ID du bus i2c de EV3 (de 1 à 4)
    void unlock();


};

#endif /* DRIVER_EV3_EV3I2C_HPP_ */
