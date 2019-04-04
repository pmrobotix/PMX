/*
 * LegoAngleSensor.cpp
 *
 *  Created on: 29 mars 2019
 *      Author: pmx
 */

#include "LegoAngleSensor.hpp"
#include "ev3dev.h"

constexpr char LegoAngleSensor::ht_angle[];

LegoAngleSensor::LegoAngleSensor(address_type address) :
        sensor(address, { ht_angle })
{
    this->set_mode("ANGLE-ACC");
    reset();
}

void LegoAngleSensor::reset()
{
    this->set_command("RESET");
}

long LegoAngleSensor::getValueDegrees()
{
    long ticks = this->value(0);
    return ticks;
}
