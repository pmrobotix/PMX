/*
 * LegoTouch.cpp
 *
 *  Created on: Sep 11, 2023
 *      Author: pmx
 */

#include "LegoTouch.hpp"

#include <cstdio>
#include <string>


using namespace ev3dev;

LegoTouch::LegoTouch(address_type address) :
        touch_sensor(address)
{
    if (this->connected()) {
        //if both motors are connected, then initialize each motor.

        if (_path.empty()) {
            printf("ERROR - LegoMotor::LegoMotor() path empty !!!!\n");
            return;
        }

        //printf("LegoTouch_path = %s\n", _path.c_str()); ///sys/class/lego-sensor/sensor0/

        fd_value0 = open_posix_in(_path + "value0");

    }

}

int LegoTouch::custom_get_value()
{
    if (this->connected()) {
        //logger().info() << " "<< logs::end;
        return get_attr_int_posix(fd_value0);
    }
    return -1;
}

