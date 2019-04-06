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
    if (this->connected())
    {
        this->set_mode("ANGLE-ACC");
    }
 }

void LegoAngleSensor::reset()
{
    if (this->connected())
        this->set_command("RESET");
}

long LegoAngleSensor::getValueDegrees()
{
    if (this->connected())
    {
    long ticks = this->value(0);
    return ticks;
    }else
        return -999999;
}
