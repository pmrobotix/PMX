#ifndef DRIVER_EV3_LEGOMOTOR_HPP_
#define DRIVER_EV3_LEGOMOTOR_HPP_

#include "../Log/LoggerFactory.hpp"
#include "ev3dev.h"
#include "ev3dev_extend.hpp"

using namespace ev3dev;

class LegoMotor: public large_motor, device_extend {
private:
    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref LegoMotor(EV3).
     */
    static inline const logs::Logger & logger() {
        static const logs::Logger & instance = logs::LoggerFactory::logger("LegoMotor.EV3");
        return instance;
    }
protected:

    int fd_duty_cycle_sp_;
    int fd_command_;
    bool first_cmd_;
    int fd_pos_;
    int fd_speed_;

public:

    /*!
     * \brief Constructor.
     */
    LegoMotor(address_type address = INPUT_AUTO);

    /*!
     * \brief Destructor.
     */
    ~LegoMotor() {
        if (this->connected()) {
            close_posix(fd_duty_cycle_sp_);
            close_posix(fd_command_);
            close_posix(fd_pos_);
        }

    }

    void custom_set_duty_cycle_sp_run_direct(int value);
    void custom_set_duty_cycle_sp(int value);
    void custom_set_command(std::string data);

    int custom_get_position();
    int custom_get_speed(); //return tacho counts per second
};

#endif
