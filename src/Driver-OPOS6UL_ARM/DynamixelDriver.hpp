/*
 * DynamixelDriver.hpp
 *
 *  Created on: 3 avr. 2019
 *      Author: pmx
 */

#ifndef DRIVER_OPOS6UL_ARM_DYNAMIXELDRIVER_HPP_
#define DRIVER_OPOS6UL_ARM_DYNAMIXELDRIVER_HPP_

#include "../Common/Interface.Driver/AServoDriver.hpp"
#include "../Log/LoggerFactory.hpp"
#include "CCAx12Adc.hpp"

using namespace std;

class DynamixelDriver: public AServoDriver
{
private:

    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref DynamixelDriver(OPOS6UL).
     */
    static inline const logs::Logger & logger()
    {
        static const logs::Logger & instance = logs::LoggerFactory::logger("DynamixelDriver.OPO");
        return instance;
    }

    int connected_;

protected:

public:
    /*!
     * \brief Constructor.
     */
    DynamixelDriver();

    /*!
     * \brief Destructor.
     */
    inline ~DynamixelDriver()
    {

    }

    void setType(int servo, ServoType type);

    void hold(int servo);

    //pos AX12 0->1023
    void setPulsePos(int servo, int pulsewidth);

    void release(int servo);

    void setRate(int servo, int speed);

    void turn(int servo, int speed);

    int getMoving(int servo);

    int getPulsePos(int servo);

    int ping(int servo);

    void setMinPulse(int servo, int value = 0); //default 0 [0 to 200]

    void setMidPulse(int servo, int value = 512); //default 512 [300 to 700]

    void setMaxPulse(int servo, int value = 1023); //default 1023 [800 to 1023]

    void setPolarity(int servo, bool inversed);

    int getTorque(int servo);

};

#endif /* DRIVER_OPOS6UL_ARM_DYNAMIXELDRIVER_HPP_ */
