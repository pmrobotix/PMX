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

class LegoAngleSensor: public i2c_sensor
{

protected:
    mutable  std::ifstream *_ifs_value;

public:

    static constexpr char ht_angle[] = "ht-nxt-angle";



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

    void openIfstreamFile(const std::string &name) const;
    int get_attr_int_optimized() const;

    uintmax_t wc(char const *fname); //static ??

};

#endif /* DRIVER_EV3_LEGOANGLESENSOR_HPP_ */
