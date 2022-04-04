/*!
 * \file
 * \brief Définition de la classe CCAx12Adc.
 * Program for the Made by PM-ROBOTIX card to manage servomotor AX12, LED board, and ADC
 */

#ifndef DRIVER_EV3_CCAX12ADC_HPP
#define DRIVER_EV3_CCAX12ADC_HPP

#include "Ev3i2c.hpp"

#include "../Log/LoggerFactory.hpp"

#define AX12ADC_ADDR		 0x08

// Commands, first byte sent
#define CMD_SET_LED_ON		10
#define CMD_SET_LED_OFF		11
#define CMD_GET_ADC			12
#define CMD_PING_AX			20
#define CMD_READ_AX			21
#define CMD_WRITE_AX		22

// AX12 memory address
#define P_MODEL_NUMBER				0
#define P_VERSION					2
#define P_ID						3
#define P_BAUD_RATE					4
#define P_RETURN_DELAY_TIME			5
#define P_CW_ANGLE_LIMIT			6
#define P_CCW_ANGLE_LIMIT			8
#define P_SYSTEM_DATA2				10
#define P_LIMIT_TEMPERATURE			11
#define P_DOWN_LIMIT_VOLTAGE		12
#define P_UP_LIMIT_VOLTAGE			13
#define P_MAX_TORQUE				14
#define P_RETURN_LEVEL				16
#define P_ALARM_LED					17
#define P_ALARM_SHUTDOWN			18
#define P_OPERATING_MODE			19
#define P_DOWN_CALIBRATION		    20
#define P_UP_CALIBRATION			22
#define P_TORQUE_ENABLE				24
#define P_LED						25
#define P_CW_COMPLIANCE_MARGIN		26
#define P_CCW_COMPLIANCE_MARGIN		27
#define P_CW_COMPLIANCE_SLOPE		28
#define P_CCW_COMPLIANCE_SLOPE		29
#define P_GOAL_POSITION				30
#define P_GOAL_SPEED				32
#define P_TORQUE_LIMIT				34
#define P_PRESENT_POSITION			36
#define P_PRESENT_SPEED				38
#define P_PRESENT_LOAD				40
#define P_PRESENT_VOLTAGE			42
#define P_PRESENT_TEMPERATURE		43
#define P_REGISTERED_INSTRUCTION	44
#define P_PAUSE_TIME				45
#define P_MOVING					46
#define P_LOCK						47
#define P_PUNCH						48

/*!
 * \brief Implementation for CCAx12Adc card.
 */
class CCAx12Adc: public utils::Mutex
{
private:

    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref CCAx12Adc (EV3).
     */
    static const logs::Logger & logger()
    {
        static const logs::Logger & instance = logs::LoggerFactory::logger("CCAx12Adc.EV3");
        return instance;
    }

    Ev3i2c i2c_CCAx12Adc_;

    bool connected_;
    utils::Mutex mutex_;
    bool begin_;

    /*!
     * \brief Constructeur de la classe.
     */
    CCAx12Adc();

    int getAddressSize(int address);

    //int write_i2c(unsigned char command, unsigned char value);

    int write(unsigned char command);
    int read();

    int readRegs(uint8_t reg_address, uint8_t len, uint8_t* data);
    int writeReg(uint8_t reg_address, uint8_t value);
    int writeRegs(uint8_t reg_address, uint8_t* values);
public:

    static CCAx12Adc & instance()
    {
        static CCAx12Adc instance; //static instance pour utilsier ADC+I2C+LED
        return instance;
    }


    /*!
     * \brief Destructeur de la classe.
     */
    virtual inline ~CCAx12Adc()
    {
    }

    // configuration and initialisation avec l'adresse i2c sur le bus
    int begin(uint i2c_aAddr);

    //todo get address i2c //ping de la carte à faire

    // Set the led on
    // @param led :  1 - 10
    void setLedOn(int led);

    // Set the led off
    // @param led :  1 - 10
    void setLedOff(int led);

    // Get the ADC value (12bits) for -5 to 5 volts
    // @param ADC :  0 - 9
    // @returns value : 0 - 4095
    int getADC(int adc);

    int convertToVoltage(int adc);

    // Ping a Dynamixel Servo
    // @param id : the id of the dynamixel
    // @returns 1 is servo is found
    int pingAX(int id);

    // Read data (8bits or 16bits) at a specified address
    // @param id : the id of the dynamixel
    // @param address : address of the data
    // @returns the data
    int readAXData(int id, int address);

    // Write data (8bits or 16bits) at a specified address
    // @param id : the id of the dynamixel
    // @param address : address of the data
    int writeAXData(int id, int address, int data);

    bool isConnected()
    {
        return connected_;
    }

};

#endif
