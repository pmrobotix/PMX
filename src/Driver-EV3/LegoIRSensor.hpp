/*
 * LegoIRSensor.hpp
 *
 *  Created on: 24 mars 2019
 *      Author: pmx
 */

#ifndef DRIVER_EV3_LEGOIRSENSOR_HPP_
#define DRIVER_EV3_LEGOIRSENSOR_HPP_

#include "../Log/LoggerFactory.hpp"
#include "ev3dev.h"

using namespace std;
using namespace ev3dev;

class LegoIRSensor
{
private:

    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref LegoIRSensor(EV3).
     */
    static inline const logs::Logger & logger()
    {
        static const logs::Logger & instance = logs::LoggerFactory::logger("LegoIRSensor.EV3");
        return instance;
    }

    infrared_sensor ir_;

public:

    /*!
     * \brief Constructor.
     */
    LegoIRSensor(const char* input_number);

    /*!
     * \brief Destructor.
     */
    ~LegoIRSensor()
    {
    }

    /*!
     * \brief donne la distance detectée en mm.
     */
    double getDistanceMM();

};

#endif /* DRIVER_EV3_LEGOIRSENSOR_HPP_ */
