/*!
 * \file
 * \brief Définition de la classe CCAx12TeensyDriverTest.
 */

#ifndef EV3TEST_CCAX12TEENSYDRIVERTEST_HPP
#define EV3TEST_CCAX12TEENSYDRIVERTEST_HPP

#include <cstdint>

#include "../../src/Log/LoggerFactory.hpp"
#include "../Suite/UnitTest.hpp"

namespace test {


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
 * \brief Teste la classe \ref CCAx12TeensyDriverTest.
 */
class CCAx12TeensyDriverTest: public UnitTest
{
private:

    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref CCAx12TeensyDriverTest(EV3).
     */
    static inline const logs::Logger & logger()
    {
        static const logs::Logger & instance = logs::LoggerFactory::logger("CCAx12TeensyDriverTest.EV3");
        return instance;
    }

    //AServoDriver* servodriver_;

    // I2C communication
    int32_t fd_i2c;
    uint8_t slave_addr_i2c;

public:

    /*!
     * \brief Constructeur de la classe.
     */
    CCAx12TeensyDriverTest() :
            UnitTest("CCAx12TeensyDriverTest")
    {
        //servodriver_ = AServoDriver::create();
    }

    /*!
     * \brief Destructeur de la classe.
     */
    virtual ~CCAx12TeensyDriverTest()
    {
    }

    int initI2C(uint8_t slaveAddress);

    int writeI2C(char * data, uint16_t bytesToWrite) ;
    uint8_t readI2C(char cmd, char * buf, uint16_t bytesToRead);

    int getAddressSize(int address);


    uint8_t computeCRC(char *buf, uint16_t length);
    int checkCRC(char* buf, uint16_t length);

    int setLed(uint8_t led, bool on);
    int setLedOn(uint8_t led);
    int setLedOff(uint8_t led);

    int getADC(uint8_t adc);
    int pingAX(uint8_t port, uint8_t id);
    int readAXData(uint8_t port, uint8_t id, uint8_t address);
    int writeAXData(uint8_t port,uint8_t id, uint8_t address, uint16_t data);

    virtual void suite();

    void testSet();

};
}

#endif
