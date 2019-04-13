
#ifndef DRIVER_EV3_LEGOMOTOR_HPP_
#define DRIVER_EV3_LEGOMOTOR_HPP_

#include "ev3dev.h"

using namespace ev3dev;

class LegoMotor: public large_motor
{

protected:

    mutable std::ofstream * _os_duty_cycle_sp;
    mutable std::ofstream * _os_command;
public:

    /*!
     * \brief Constructor.
     */
    LegoMotor(address_type address = INPUT_AUTO);

    /*!
     * \brief Destructor.
     */
    ~LegoMotor()
    {
    }

    void set_duty_cycle_sp_run_direct(int value);

    void set_duty_cycle_sp_optimised(int value);

    void run_direct_optimised();

};

#endif
