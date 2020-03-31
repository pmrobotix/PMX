#include "LegoMotor.hpp"

#include <cstdio>
#include <string>
#include "ev3dev.h"

#include "../Log/Logger.hpp"

using namespace ev3dev;

LegoMotor::LegoMotor(address_type address) :
        large_motor(address)
{
    if (this->connected()) {
        //if both motors are connected, then initialize each motor.

        this->reset();

        //this->set_polarity(motor::polarity_inversed);
        this->set_polarity(motor::polarity_normal);

        this->set_ramp_down_sp(0);
        this->set_ramp_up_sp(0);
        this->set_stop_action(motor::stop_action_brake);

    }

    if (_path.empty()) {
        printf("ERROR - LegoMotor::LegoMotor() path empty !!!!\n");
        return;
    }

    first_cmd_ = false;
    fd_duty_cycle_sp_ = open_posix_out(_path+"duty_cycle_sp");
    fd_command_ = open_posix_out(_path+"command");

    fd_pos_ = open_posix_in(_path+"position");
    fd_speed_ = open_posix_in(_path+"speed");
}


void LegoMotor::custom_set_duty_cycle_sp_run_direct(int value)
{
    custom_set_duty_cycle_sp(value);
    if (!first_cmd_ )
    {
        first_cmd_= true;
        custom_set_command("run-direct");
    }
    if (value == 0)
        first_cmd_= false;
}

void LegoMotor::custom_set_duty_cycle_sp(int value)
{
    set_attr_int_posix(fd_duty_cycle_sp_, value);
    //this->set_attr_int("duty_cycle_sp", value);
    //this->set_duty_cycle_sp(value);
}

void LegoMotor::custom_set_command(std::string data)
{
    set_attr_string_posix(fd_command_, data);
    //this->set_attr_string("command", data);
    //this->set_command(data);
}

int LegoMotor::custom_get_position()
{
    //logger().info() << " speed= " << custom_get_speed() << logs::end;

    return get_attr_int_posix(fd_pos_);
    //return this->get_attr_int("position");
    //return this->position();
}

int LegoMotor::custom_get_speed()
{
    return get_attr_int_posix(fd_speed_);
    //return this->get_attr_int("speed");
    //return this->speed();
}

