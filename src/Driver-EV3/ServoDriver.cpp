//drivers...EV3

#include "ServoDriver.hpp"

#include <unistd.h>

#include "../Log/Logger.hpp"

using namespace std;
using namespace ev3dev;

AServoDriver * AServoDriver::create(ServoType type)
{
    if (type == AServoDriver::SERVO_STANDARD) {
        static ServoDriver *instance = new ServoDriver();
        return instance;
    } else
        return NULL;
}

ServoDriver::ServoDriver() :
        connected_(0)
{
    logger().debug() << "ServoDriver()" << logs::end;

    //mise du pollong à 0 pour la carte de servo.
    i2c_sensor servo_sensor = i2c_sensor("ev3-ports:in4:i2c88");
    servo_sensor.set_poll_ms(0);


    //CONFIG Specifique si necessaire
    servo_motors_[0] = servo_motor("ev3-ports:in4:i2c88:sv1");
    servo_motors_[1] = servo_motor("ev3-ports:in4:i2c88:sv2");
    servo_motors_[2] = servo_motor("ev3-ports:in4:i2c88:sv3");
    servo_motors_[3] = servo_motor("ev3-ports:in4:i2c88:sv4");
    servo_motors_[4] = servo_motor("ev3-ports:in4:i2c88:sv5");
    servo_motors_[5] = servo_motor("ev3-ports:in4:i2c88:sv6");
    servo_motors_[6] = servo_motor("ev3-ports:in4:i2c88:sv7");
    servo_motors_[7] = servo_motor("ev3-ports:in4:i2c88:sv8");

    if (ping(0)) //test sur le servo 0 pour savoir si la carte est branchée
        connected_ = 1;
    else
        logger().error() << "SERVO NOT CONNECTED! ServoDriver()" << logs::end;

}

bool ServoDriver::testIf(long value, long valeurMin, long valeurMax)
{
    if ((value >= valeurMin) && (value <= valeurMax))
        return true;
    else {
        logger().error() << "SERVO ID NOT EXISTS!! testIf() " << "value=" << value << " min=" << valeurMin << " max="
                << valeurMax << logs::end;
        return false;
    }
}

void ServoDriver::hold(int servo) // 0 à 7
{
    if (!connected_) {
        logger().error() << "SERVO NOT CONNECTED! hold() servo=" << servo << logs::end;
        return;
    }
    if (!testIf(servo, 0, 7))
        return;
    servo_motors_[servo].run();
}

void ServoDriver::setPosition(int servo, int percent)
{
    if (!connected_) {
        logger().error() << "SERVO NOT CONNECTED! setPosition() servo=" << servo << logs::end;
        return;
    }

    if (!testIf(servo, 0, 7))
        return;

    constrain(percent, -100, 100);
    servo_motors_[servo].set_position_sp(percent);
    servo_motors_[servo].run();

    usleep(10000);

}

void ServoDriver::release(int servo)
{
    if (!connected_) {
        logger().error() << "SERVO NOT CONNECTED! release() servo=" << servo << logs::end;
        return;
    }
    if (!testIf(servo, 0, 7))
        return;
    servo_motors_[servo].float_();
    usleep(10000);
}

void ServoDriver::setRate(int servo, int millisec0To90)
{
    if (!connected_) {
        logger().error() << "SERVO NOT CONNECTED! NO setRate ! servo=" << servo << logs::end;
        return;
    }
    if (!testIf(servo, 0, 7))
        return;

    // Sets the rate_sp at which the servo travels from 0 to 100.0% (half of the full
    // range of the servo). Units are in milliseconds. Example: Setting the rate_sp
    // to 1000 means that it will take a 180 degree servo 2 second to move from 0
    // to 180 degrees. Note: Some servo controllers may not support this in which
    // case reading and writing will fail with `-EOPNOTSUPP`.
    servo_motors_[servo].set_rate_sp(millisec0To90);
    usleep(10000);
}

void ServoDriver::turn(int servo, int speed)
{
    if (!connected_) {
        logger().error() << "SERVO NOT CONNECTED! turn() servo=" << servo << logs::end;
        return;
    }
}

int ServoDriver::getMoving(int servo)
{
    if (!testIf(servo, 0, 7))
        return -999;
    return -999;
}

int ServoDriver::getPos(int servo)
{
    if (!connected_) {
        logger().error() << "SERVO NOT CONNECTED! getPos() servo=" << servo << logs::end;
        return -999;
    }
    if (!testIf(servo, 0, 7))
        return -999;
    return servo_motors_[servo].position_sp();
}

int ServoDriver::ping(int servo)
{
//    for (int i = 0; i < 8; i++) {
//        logger().debug() << "  " << servo_motors_[i].connected() << logs::end;
//    }

    return servo_motors_[servo].connected();
    usleep(10000);
}

void ServoDriver::setMinPulse(int servo, int pulse)
{
    if (!testIf(servo, 0, 7))
        return;

    if (!testIf(pulse, 300, 700))
        constrain(pulse, 300, 700);

    servo_motors_[servo].set_min_pulse_sp(pulse); //default 600 [300 to 700]
}

void ServoDriver::setMidPulse(int servo, int pulse)
{
    if (!testIf(servo, 0, 7))
        return;

    if (!testIf(pulse, 1300, 1700))
        constrain(pulse, 1300, 1700);
    servo_motors_[servo].set_mid_pulse_sp(pulse); //default 1500 [1300 to 1700]
}

void ServoDriver::setMaxPulse(int servo, int pulse)
{
    if (!testIf(servo, 0, 7))
        return;
    if (!testIf(pulse, 2300, 2700))
        constrain(pulse, 2300, 2700);
    servo_motors_[servo].set_max_pulse_sp(pulse); //default 2400 [2300 to 2700]
}

void ServoDriver::setPolarity(int servo, bool inversed)
{
    if (!testIf(servo, 0, 7))
        return;
    if (!inversed)
        servo_motors_[servo].set_polarity(servo_motor::polarity_normal);
    else
        servo_motors_[servo].set_polarity(servo_motor::polarity_inversed);
}
