#ifndef DRIVER_EV3_SERVOMOTOREXTEND_HPP_
#define DRIVER_EV3_SERVOMOTOREXTEND_HPP_

#include <string>

#include "../Log/LoggerFactory.hpp"
#include "ev3dev_extend.hpp"
#include "ev3dev.h"

using namespace ev3dev;

class ServoMotorExtend: public servo_motor, device_extend
{
private:
    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref ServoMotorExtend(EV3).
     */
    static inline const logs::Logger & logger()
    {
        static const logs::Logger & instance = logs::LoggerFactory::logger("ServoMotorExtend.EV3");
        return instance;
    }

protected:
    std::string address_;
    int fd_position_sp_in_;
    //int fd_position_sp_out_;

public:

    /*!
     * \brief Constructor.
     */
    ServoMotorExtend(address_type address = INPUT_AUTO);

    /*!
     * \brief Destructor.
     */
    ~ServoMotorExtend()
    {
        this->close_posix(fd_position_sp_in_);
        //this->close_posix(fd_position_sp_out_);
    }

    void setPositionPOSIX(int percent);

    int getPositionPOSIX();

};

#endif /* DRIVER_EV3_SERVOMOTOREXTEND_HPP_ */


