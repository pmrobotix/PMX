#ifndef DRIVER_EV3_LEGOANGLESENSOR_HPP_
#define DRIVER_EV3_LEGOANGLESENSOR_HPP_

#include <string>

#include "../Log/LoggerFactory.hpp"
#include "ev3dev_extend.hpp"
#include "ev3dev.h"

using namespace ev3dev;

class LegoAngleSensor: public i2c_sensor, device_extend
{
private:
    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref LegoAngleSensor(EV3).
     */
    static inline const logs::Logger & logger()
    {
        static const logs::Logger & instance = logs::LoggerFactory::logger("LegoAngleSensor.EV3");
        return instance;
    }

protected:
    std::string address_;
    int fd_value0_;

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
        //this->close_posix(fd_value0_);
    }

    /*!
     * \brief reinitialise les valeurs.
     */
    void reset();

    int getValueDegrees();

};

#endif /* DRIVER_EV3_LEGOANGLESENSOR_HPP_ */
