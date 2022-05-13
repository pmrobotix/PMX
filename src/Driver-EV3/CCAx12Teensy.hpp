/*
 * CCAx12Teensy.hpp
 *
 *  Created on: Apr 17, 2022
 *      Author: pmx
 */

#ifndef DRIVER_EV3_CCAX12TEENSY_HPP_
#define DRIVER_EV3_CCAX12TEENSY_HPP_

#include "Ev3i2c.hpp"

#include "../Log/LoggerFactory.hpp"

#define AX12TEENSY_ADDR                     0x10

// AXTeensy commands

#define AXT_CMD_SET_LED_ON              10
#define AXT_CMD_SET_LED_OFF             11
#define AXT_CMD_GET_ADC                 12
#define AXT_CMD_PING_AX                 20
#define AXT_CMD_READ_AX                 21
#define AXT_CMD_WRITE_AX                22

#define AXT_STATUS                      0x88
#define AXT_OK                          0
#define AXT_AX_MISSING                  8

// Errors
#define AXT_ERROR_SEND_CMD    -2000;
#define   AXT_ERROR_READ_STATUS         -3000;
#define AXT_ERROR_CRC                   -4000;



// AX12 memory address
#define P_MODEL_NUMBER                                      0
#define P_VERSION                                 2
#define P_ID                                                3
#define P_BAUD_RATE                               4
#define P_RETURN_DELAY_TIME                       5
#define P_CW_ANGLE_LIMIT                          6
#define P_CCW_ANGLE_LIMIT                         8
#define P_SYSTEM_DATA2                                      10
#define P_LIMIT_TEMPERATURE                       11
#define P_DOWN_LIMIT_VOLTAGE                      12
#define P_UP_LIMIT_VOLTAGE                        13
#define P_MAX_TORQUE                                        14
#define P_RETURN_LEVEL                                      16
#define P_ALARM_LED                               17
#define P_ALARM_SHUTDOWN                          18
#define P_OPERATING_MODE                          19
#define P_DOWN_CALIBRATION                        20
#define P_UP_CALIBRATION                          22
#define P_TORQUE_ENABLE                                     24
#define P_LED                                               25
#define P_CW_COMPLIANCE_MARGIN                              26
#define P_CCW_COMPLIANCE_MARGIN                             27
#define P_CW_COMPLIANCE_SLOPE                     28
#define P_CCW_COMPLIANCE_SLOPE                              29
#define P_GOAL_POSITION                                     30
#define P_GOAL_SPEED                                        32
#define P_TORQUE_LIMIT                                      34
#define P_PRESENT_POSITION                        36
#define P_PRESENT_SPEED                                     38
#define P_PRESENT_LOAD                                      40
#define P_PRESENT_VOLTAGE                         42
#define P_PRESENT_TEMPERATURE                     43
#define P_REGISTERED_INSTRUCTION                  44
#define P_PAUSE_TIME                                        45
#define P_MOVING                                  46
#define P_LOCK                                              47
#define P_PUNCH                                             48

/*!
 * \brief Implementation for CCAx12Teensy card.
 */
class CCAx12Teensy: public utils::Mutex {
private:

    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref CCAx12Teensy (EV3).
     */
    static const logs::Logger & logger() {
        static const logs::Logger & instance = logs::LoggerFactory::logger("CCAx12Teensy.EV3");
        return instance;
    }

    Ev3i2c i2c_CCAx12Teensy_;
    bool connected_;
    utils::Mutex mutex_;

    int i2cHandle_;
    short int i2c_aAddr_;


    /*!
     * \brief Constructeur de la classe.
     */
    CCAx12Teensy();

    int getAddressSize(int address);
    uint8_t computeCRC(uint8_t *buf, uint16_t length);
    int checkCRC(uint8_t* buf, uint16_t length);

    int writeI2C(uint8_t * data, uint16_t bytesToWrite);
    uint8_t readI2C(uint8_t cmd, uint8_t * buf, uint16_t bytesToRead);

public:

    static CCAx12Teensy & instance() {
        static CCAx12Teensy instance; //static instance pour utiliser indiferemment ADC+I2C+LED+autres si dispo
        return instance;
    }

    /*!
     * \brief Destructeur de la classe.
     */
    virtual inline ~CCAx12Teensy() {
    }

    // configuration and initialisation avec l'adresse i2c sur le bus
    //return 0 if ok;
    bool connect(short int i2c_aAddr);

    int setLed(uint8_t led, bool on);

    // Set the led on
    // @param led :  1 - 9
    int setLedOn(uint8_t led);

    // Set the led off
    // @param led :  1 - 9
    int setLedOff(uint8_t led);

    // Get the ADC value (12bits) for -5 to 5 volts
    // @param ADC :  0 - 9
    // @returns value : 0 - 4095
    int getADC(uint8_t adc);


    int convertToVoltage(int adc);

    /**
     * Ping a Dynamixel Servo
     * @param port : the port (1-4)
     * @param id : the id of the dynamixel
     * @returns AXT_OK is servo is found, AXT_AX_MISSING if not found
     * (on error, result is <0)
     */
    int pingAX(uint8_t port, uint8_t id);

    /**
     * Read data at a specified address
     * @param port : the port (1-4)
     * @param id : the id of the dynamixel
     * @param address : address of the data
     * @returns the data
     */
    int readAXData(uint8_t port, uint8_t id, uint8_t address);

    /**
     * Write data at a specified address
     * @param port : the port (1-4)
     * @param id : the id of the dynamixel
     * @param address : address of the data
     */
    int writeAXData(uint8_t port, uint8_t id, uint8_t address, uint16_t data);

    bool isConnected() {
        return connected_;
    }

};

#endif
