#include "LegoAngleSensor.hpp"

#include <unistd.h>

#include "../Log/Logger.hpp"

constexpr char LegoAngleSensor::ht_angle[];

LegoAngleSensor::LegoAngleSensor(address_type address) :
        i2c_sensor(address, { ht_angle })
{
    address_ = address;
    if (this->connected()) {
        //Mise à jour du polling pour mettre a jour en i2c toutes les n ms
        this->set_poll_ms(1);
        this->set_mode("ANGLE-ACC");

        fd_value0_ = this->open_posix_in(_path +"value0");
    }
}

void LegoAngleSensor::reset()
{
    logger().info() << " addr= " << address() << " " << this->_path << logs::end;
    if (this->connected())
        this->set_command("RESET");
}

int LegoAngleSensor::getValueDegrees()
{
    //logger().info() << " getValueDegrees " << logs::end;
    if (this->connected()) {

        int accumulatedAngleDegrees = this->get_attr_int_posix(fd_value0_);


//        std::string s = get_attr_string_posix(fd_value0_);
//        int accumulatedAngleDegrees = std::stoi(s, nullptr, 0); //convert to int

        //int accumulatedAngleDegrees =  this->get_attr_int("value0");
        //int accumulatedAngleDegrees =  this->value();

        //logger().info() << address_ << " " << ticks << logs::end;
        return accumulatedAngleDegrees;
    } else
        return -999999;
}
