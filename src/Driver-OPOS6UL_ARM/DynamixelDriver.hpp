/*
 * DynamixelDriver.hpp
 *
 *  Created on: 3 avr. 2019
 *      Author: pmx
 */

#ifndef DRIVER_OPOS6UL_ARM_DYNAMIXELDRIVER_HPP_
#define DRIVER_OPOS6UL_ARM_DYNAMIXELDRIVER_HPP_

#include "../Common/Action.Driver/AServoDriver.hpp"
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

    void hold(int servo);

    //pos AX12 0->1023
    void setPosition(int servo, int pos);

    void release(int servo);

    void setRate(int servo, int speed);

    void turn(int servo, int speed);

    int getMoving(int servo);

    int getPos(int servo);

    int ping(int servo);

};

#endif /* DRIVER_OPOS6UL_ARM_DYNAMIXELDRIVER_HPP_ */
