#include "LegoMotor.hpp"

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

//constexpr char LegoAngleSensor::ht_angle[];

LegoMotor::LegoMotor(address_type address) :
        large_motor(address)
{
    if (this->connected()) {
        //if both motors are connected, then initialize each motor.

        //_motor_right_.
        this->reset();

        this->set_polarity(motor::polarity_inversed);

        this->set_ramp_down_sp(0);
        this->set_ramp_up_sp(0);
        this->set_stop_action(stop_action_brake);

    }

    if (_path.empty()) {
        return;
    }

    _os_duty_cycle_sp = new std::ofstream();
    _os_duty_cycle_sp->rdbuf()->pubsetbuf(NULL, 0);
    _os_duty_cycle_sp->open(_path + "duty_cycle_sp");

    _os_command = new std::ofstream();
    _os_command->rdbuf()->pubsetbuf(NULL, 0);
    _os_command->open(_path + "command");

}

void LegoMotor::set_duty_cycle_sp_run_direct(int value)
{
    //this->set_duty_cycle_sp(value).run_direct();

    this->set_duty_cycle_sp_optimised(value);
    this->run_direct_optimised();
}

void LegoMotor::set_duty_cycle_sp_optimised(int value)
{
    _os_duty_cycle_sp->clear();
    *_os_duty_cycle_sp << value;
}

void LegoMotor::run_direct_optimised()
{
    _os_command->clear();
    *_os_command << "run-direct";
}

