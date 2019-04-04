/*
 * LegoAngleSensor.hpp
 *
 *  Created on: 29 mars 2019
 *      Author: pmx
 */

#ifndef DRIVER_EV3_LEGOANGLESENSOR_HPP_
#define DRIVER_EV3_LEGOANGLESENSOR_HPP_

#include "ev3dev.h"

using namespace ev3dev;




class LegoAngleSensor: public sensor
{
public:


    static constexpr char ht_angle[]   = "ht-nxt-angle";

    /*!
     * \brief Constructor.
     */
    LegoAngleSensor(address_type address = INPUT_AUTO);

    /*!
     * \brief Destructor.
     */
    ~LegoAngleSensor()
    {
    }

    /*!
     * \brief reinitialise les valeurs.
     */
    void reset();

    long getValueDegrees();

};

#endif /* DRIVER_EV3_LEGOANGLESENSOR_HPP_ */
