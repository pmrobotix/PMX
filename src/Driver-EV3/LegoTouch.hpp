/*
 * LegoTouch.hpp
 *
 *  Created on: Sep 11, 2023
 *      Author: pmx
 */

#ifndef DRIVER_EV3_LEGOTOUCH_HPP_
#define DRIVER_EV3_LEGOTOUCH_HPP_


#include "../Log/LoggerFactory.hpp"
#include "ev3dev.h"
#include "ev3dev_extend.hpp"

using namespace ev3dev;

class LegoTouch: public touch_sensor, device_extend {
private:
    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref LegoMotor(EV3).
     */
    static inline const logs::Logger & logger() {
        static const logs::Logger & instance = logs::LoggerFactory::logger("LegoTouch.EV3");
        return instance;
    }
protected:

    int fd_value0;


public:

    /*!
     * \brief Constructor.
     */
    LegoTouch(address_type address = INPUT_AUTO);

    /*!
     * \brief Destructor.
     */
    ~LegoTouch() {
        if (this->connected()) {
            close_posix(fd_value0);
        }
    }

    int custom_get_value();

};




#endif /* DRIVER_EV3_LEGOTOUCH_HPP_ */
