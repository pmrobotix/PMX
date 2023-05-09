#include "MD25.hpp"

#include "../Log/Logger.hpp"
#include <unistd.h>

/*** CONSTRUCTOR AND DESTRUCTOR ***/

MD25::MD25() :
//connexion i2c 5V
        i2c_MD25_(1), connected_(true)
{
    /* Default values */
    mode = MODE_1;

}

bool MD25::begin() {

    //open i2c and setslave
    i2c_MD25_.setSlaveAddr(ADDR_MD25);

    int soft = software_rev_num_get();
    logger().debug() << "begin() : MD25 soft="<< soft << logs::end;

    //test du voltage: bat_voltage_get?

    if (soft == 4) {
        connected_ = true;

        mode_set(mode); //mode0 => 0-128-255   ;  mode1 => -128+127
        acceleration_set(10);
        auto_speed_set(true);
        speed1_set(0);
        speed2_set(0);
        timeout_set(false);
        encoder_reset();
    }else
    {
        connected_ = false;
        logger().debug() << "begin() : MD25 NOT CONNECTED !" << logs::end;
    }
    return connected_;
}

/*** GENERIC METHODS ***/

/**
 * Write a number of bytes.
 *
 * @param reg_addr      Address of register to access.
 * @param data          Pointer to where data is stored.
 * @param bytes         Number of bytes to write.
 */
int MD25::write(unsigned char reg_addr, unsigned char *data, int bytes) {
    /* Variables */
//    char data_out[bytes];
    /* Add register address to char array */
    //data_out[0] = reg_addr;
    /* Populate data array */
//    for (int i = 1; i < bytes + 1; i++) {
//        data_out[i] = data[i - 1];
//    }
    /* Write address and data */
//    if (i2c_interface->write(i2c_address + I2C_WRITE_BIT, &data_out[0], bytes + 1, true)) {
//        return -1;
//    }
    if (i2c_MD25_.writeReg(reg_addr, data, bytes) < 0) return -1;
    usleep(100); //tested with min 75us

    /* Return success */
    return 0;
}

/**
 * Read a number of bytes.
 *
 * @param reg_addr      Address of register to access.
 * @param data          Pointer to where data it to be stored.
 * @param bytes         Number of bytes to read.
 */
int MD25::read(unsigned char reg_addr, unsigned char *data, int bytes) {
//    /* Setup register to read */
//    if (!i2c_interface->write(i2c_address + I2C_WRITE_BIT, &reg_addr, 1, true)) {
//
//        /* Read register */
//        if (i2c_interface->read(i2c_address + I2C_READ_BIT, data, bytes, false)) {
//            return -1;
//        }
//        wait_us(100); //tested with min 75us
//    } else {
//        return -1;
//    }
    if (i2c_MD25_.readReg(reg_addr, data, bytes) < 0) return -1;
    usleep(100); //tested with min 75us

    /* Return success */
    return 0;
}

/**
 * Discover I2C devices.
 *
 * @return  char containing indicator of i2c device presence in form of bits
 *          i.e. if LSB of returned char is 1 then a device is present at I2C_START_ADDR.
 *          If bit 2 of char is 1 then a device is present at I2C_START_ADDR + 2.
 */
//char MD25::doDiscover() {
//    /* Initialize device char */
//    char device = 0;
//
//    /* Check for devices */
//    for (int i = 0; i < 8; i++) {
//        if (!i2c_interface->write(I2C_START_ADDR + (2 * i), NULL, 0)) { // 0 returned is ok
//            device |= (1 << i);
//        }
//    }
//
//    /* Return device char */
//    return device;
//}
/*** CONTROL METHODS ***/

/**
 * Set mode of operation.
 *
 * @param mode      Set mode of operation (0 - 3) (default: 0).
 */
int MD25::mode_set(int mode) {
    /* Check input is valid */
    if (mode < 0 || mode > 3) return -1;

    /* Variables */
    unsigned char data = (char) mode;

    /* Set mode */
    if (write(REG_MODE, &data, 1) == -1) return -1;

    /* Check set */
    data = mode_get();
    if (data != mode) return -1;

    /* Set mode variable */
    this->mode = mode;

    /* Return success */
    return 0;
}

/**
 * Get mode of operation.
 */
int MD25::mode_get() {
    /* Variables */
    unsigned char data;

    /* Get mode */
    if (read(REG_MODE, &data, 1) == -1) return -1;

    /* Return mode */
    return data;
}

/**
 * Reset the encoder registers.
 */
int MD25::encoder_reset() {
    /* Variables */
    unsigned char data = CMD_ENCODER_RESET;

    /* Reset encoders */
    if (write(REG_COMMAND, &data, 1) == -1) return -1;

    /* Return success */
    return 0;
}

/**
 * Enable/disable automatic speed regulation (default: enabled).
 *
 * @param enabled   enable = 1 | disable = 0.
 */
int MD25::auto_speed_set(bool enabled) {
    /* Variables */
    unsigned char data;

    /* Set data */
    if (enabled) {
        data = CMD_AUTO_SPEED_ENABLE;
    }
    else {
        data = CMD_AUTO_SPEED_DISABLE;
    }

    /* Reset encoders */
    if (write(REG_COMMAND, &data, 1) == -1) return -1;

    /* Return success */
    return 0;
}

/**
 * Enable/disable 2 sec timeout of motors when no I2C comms (default: enabled).
 *
 * @param enabled   enable = 1 | disable = 0.
 */
int MD25::timeout_set(bool enabled) {
    /* Variables */
    unsigned char data;

    /* Set data */
    if (enabled) {
        data = CMD_TIMEOUT_ENABLE;
    }
    else {
        data = CMD_TIMEOUT_DISABLE;
    }

    /* Reset encoders */
    if (write(REG_COMMAND, &data, 1) == -1) return -1;

    /* Return success */
    return 0;
}

/**
 * Set a new I2C device address (Default: 0xB0).
 *
 * @param address   Address of device (B0 -> BE; inc: 2).
 */
int MD25::i2c_addr_set(unsigned char address) {
    /* Check input is valid */
    if (address < I2C_START_ADDR || address > I2C_START_ADDR + 0x0F) return -1;
    if (address % 2 == 1) return -1;

    /* Set data */
    unsigned char data[4];
    data[0] = CMD_CHANGE_I2C_ADDR_1;
    data[1] = CMD_CHANGE_I2C_ADDR_2;
    data[2] = CMD_CHANGE_I2C_ADDR_3;
    data[3] = address;

    /* Set new address */
    if (write(REG_COMMAND, &data[0], 4) == -1) return -1;

    /* Set address variable */
    //i2c_address = address;
    /* Return success */
    return 0;
}

/*** DATA METHODS ***/

/**
 * Set the speed of motor 1. (Only mode 0 or 1).
 *
 * @param speed     Faster the higher a number (mode 0: 0 -> 255 | mode 1: -128 to 127).
 */
int MD25::speed1_set(int speed) {
    /* Check input is valid */
    if (mode == 0) {
        if (speed < 0 || speed > 255) return -1;
    }
    else if (mode == 1) {
        if (speed < -128 || speed > 127) return -1;
    }
    else {
        return 0;
    }

    /* Variable */
    unsigned char data = (char) speed;

    /* Set data */
    if (mode == 1) data = (signed char) data;

    /* Set speed */
    if (write(REG_SPEED1, &data, 1) == -1) return -1;

    /* Return success */
    return 0;
}

/**
 * Get the set speed of motor 1. (Only mode 0 or 1).
 *
 * @return  Faster the higher a number (mode 0: 0 -> 255 | mode 1: -128 to 127).
 */
int MD25::speed1_get() {
    /* Check valid mode */
    if (mode < 0 || mode > 1) return -1;

    /* Variables */
    unsigned char data;

    /* Get speed */
    if (read(REG_SPEED1, &data, 1) == -1) return -1;

    /* Return speed */
    if (mode == 1) return (signed char) data;
    return data;
}

/**
 * Set the speed of motor 2. (Only mode 0 or 1).
 *
 * @param speed     Faster the higher a number (mode 0: 0 -> 255 | mode 1: -128 to 127).
 */
int MD25::speed2_set(int speed) {
    /* Check input is valid */
    if (mode == 0) {
        if (speed < 0 || speed > 255) return -1;
    }
    else if (mode == 1) {
        if (speed < -128 || speed > 127) return -1;
    }
    else {
        return 0;
    }

    /* Variable */
    unsigned char data = (unsigned char) speed;

    /* Set data */
    if (mode == 1) data = (signed char) data;

    /* Set speed */
    if (write(REG_SPEED2, &data, 1) == -1) return -1;

    /* Return success */
    return 0;
}

/**
 * Get the set speed of motor 2. (Only mode 0 or 1).
 *
 * @return  Faster the higher a number (mode 0: 0 -> 255 | mode 1: -128 to 127).
 */
int MD25::speed2_get() {
    /* Check valid mode */
    if (mode < 0 || mode > 1) return -1;

    /* Variables */
    unsigned char data;

    /* Get speed */
    if (read(REG_SPEED2, &data, 1) == -1) return -1;

    /* Return speed */
    if (mode == 1) return (signed char) data;
    return data;
}

/**
 * Set the speed. (Only mode 2 or 3).
 *
 * @param speed     Faster the higher a number (mode 2: 0 -> 255 | mode 3: -128 to 127).
 */
int MD25::speed_set(int speed) {
    /* Check input is valid */
    if (mode == 2) {
        if (speed < 0 || speed > 255) return -1;
    }
    else if (mode == 3) {
        if (speed < -128 || speed > 127) return -1;
    }
    else {
        return 0;
    }

    /* Set data */
    unsigned char data = (char) speed;

    /* Set speed */
    if (write(REG_SPEED1, &data, 1) == -1) return -1;

    /* Return success */
    return 0;
}

/**
 * Get the set speed. (Only mode 2 or 3).
 *
 * @return  Faster the higher a number (mode 2: 0 -> 255 | mode 3: -128 to 127).
 */
int MD25::speed_get() {
    /* Check valid mode */
    if (mode < 2 || mode > 3) return -1;

    /* Variables */
    unsigned char data;

    /* Get speed */
    if (read(REG_SPEED1, &data, 1) == -1) return -1;

    /* Return speed */
    return data;
}

/**
 * Set the turn. (Only mode 2 or 3).
 *
 * @param turn      Faster the higher a number (mode 2: 0 -> 255 | mode 3: -128 to 127).
 */
int MD25::turn_set(int turn) {
    /* Check input is valid */
    if (mode == 2) {
        if (turn < 0 || turn > 255) return -1;
    }
    else if (mode == 3) {
        if (turn < -128 || turn > 127) return -1;
    }
    else {
        return 0;
    }

    /* Set data */
    unsigned char data = (char) turn;

    /* Set turn */
    if (write(REG_SPEED2, &data, 1) == -1) return -1;

    /* Return success */
    return 0;
}

/**
 * Get the set turn. (Only mode 2 or 3).
 *
 * @return  Faster the higher a number (mode 2: 0 -> 255 | mode 3: -128 to 127).
 */
int MD25::turn_get() {
    /* Check valid mode */
    if (mode < 2 || mode > 3) return -1;

    /* Variables */
    unsigned char data;

    /* Get turn */
    if (read(REG_SPEED2, &data, 1) == -1) return -1;

    /* Return turn */
    return data;
}

/**
 * Set the desired acceleration rate.
 *
 * if new speed > current speed:
 *      steps = (new speed - current speed) / acceleration register
 * if new speed < current speed:
 *      steps = (current speed - new speed) / acceleration register
 * time = steps * 25ms
 * Example:
 *      Time/step: 25ms | Current speed: 0 | New speed: 255 | Steps: 255 | Acceleration time: 6.375s.
 * @param acceleration  Faster the higher a number (default: 5).
 */
int MD25::acceleration_set(int acceleration) {
    /* Check input is valid */
    if (acceleration < 0 || acceleration > 255) return -1;

    /* Set data */
    unsigned char data = (char) acceleration;

    /* Set acceleration */
    if (write(REG_ACCELERATION_RATE, &data, 1) == -1) return -1;

    /* Return success */
    return 0;
}

/**
 * Get the set desired acceleration rate.
 *
 * if new speed > current speed:
 *      steps = (new speed - current speed) / acceleration register
 * if new speed < current speed:
 *      steps = (current speed - new speed) / acceleration register
 * time = steps * 25ms
 * Example:
 *      Time/step: 25ms | Current speed: 0 | New speed: 255 | Steps: 255 | Acceleration time: 6.375s.
 * @return  Faster the higher a number (default: 5).
 */
int MD25::acceleration_get() {
    /* Variables */
    unsigned char data;

    /* Get acceleration */
    if (read(REG_ACCELERATION_RATE, &data, 1) == -1) return -1;

    /* Return acceleration */
    return data;
}

/**
 * Get encoder 1 position.
 */
int MD25::encoder1_get() {
    /* Variables */
    unsigned char data[4];
    int position;

    /* Get encoder bytes */
    if (read(REG_ENC1A, &data[0], 4) == -1) return -1;

    /* Combine the position bytes */
    position = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];

    /* Return position */
    return position;
}

/**
 * Get encoder 2 position.
 */
int MD25::encoder2_get() {
    /* Variables */
    unsigned char data[4];
    int position;

    /* Get encoder bytes */
    if (read(REG_ENC2A, &data[0], 4) == -1) return -1;

    /* Combine the position bytes */
    position = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];

    /* Return position */
    return position;
}

/**
 * Get battery voltage.
 * It reads 10 times the voltage i.e. 121 for 12.1V.
 *
 * @return  Normalized float.
 */
int MD25::bat_voltage_get() {
    /* Variables */
    unsigned char data;

    /* Get battery voltages */
    if (read(REG_BATTERY_VOLTS, &data, 1) == -1) return -1;

    /* Return battery voltage */
    return (int) (data);
}

/**
 * Get current of motor 1.
 * It reads 10 times the current i.e. 25 for 2.5A.
 *
 * @return  Normalized float.
 */
int MD25::motor1_current_get() {
    /* Variables */
    unsigned char data;

    /* Get motor current */
    if (read(REG_MOTOR1_CURRENT, &data, 1) == -1) return -1;

    /* Return motor current */
    return (int) (data);
}

/**
 * Get current of motor 2.
 * It reads 10 times the current i.e. 25 for 2.5A.
 *
 * @return  Normalized float.
 */
int MD25::motor2_current_get() {
    /* Variables */
    unsigned char data;

    /* Get motor current */
    if (read(REG_MOTOR2_CURRENT, &data, 1) == -1) return -1;

    /* Return motor current */
    return (int) (data);
}

/**
 * Get the software revision number in the PIC16F873 controller.
 */
int MD25::software_rev_num_get() {
    /* Variables */
    unsigned char data;

    /* Get software revision number */
    if (read(REG_SOFTWARE_REVISION, &data, 1) == -1) return -1;

    /* Return software revision number */
    return data;
}

