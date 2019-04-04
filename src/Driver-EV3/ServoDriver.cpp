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

    //CONFIG Specifique si necessaire
    servo_motors_[0] = servo_motor("in1:i2c88:sv1");
    servo_motors_[0].set_polarity(servo_motor::polarity_normal);
    servo_motors_[0].set_min_pulse_sp(600); //default 600 [300 to 700]
    servo_motors_[0].set_mid_pulse_sp(1470); //default 1500 [1300 to 1700]
    servo_motors_[0].set_max_pulse_sp(2350); //default 2400 [2300 to 2700]

    servo_motors_[1] = servo_motor("in1:i2c88:sv2");
    servo_motors_[2] = servo_motor("in1:i2c88:sv3");
    servo_motors_[3] = servo_motor("in1:i2c88:sv4");
    servo_motors_[4] = servo_motor("in1:i2c88:sv5");
    servo_motors_[5] = servo_motor("in1:i2c88:sv6");
    servo_motors_[6] = servo_motor("in1:i2c88:sv7");
    servo_motors_[7] = servo_motor("in1:i2c88:sv8");

    if (ping(0)) //test sur le servo 0 pour savoir si la carte est branchée
        connected_ = 1;
    else
        logger().error() << "SERVO NOT CONNECTED! ServoDriver()" << logs::end;

    for (int i = 0; i < 8; i++) {
        servo_motors_[i].float_();
    }

    servo_motors_[7].run();
    servo_motors_[7].set_position_sp(50);
}

//Limitation d'une valeur à un intervalle [valeurMin , valeurMax]
long ServoDriver::constrain(long value, long valeurMin, long valeurMax)
{
    if (value <= valeurMin)
        return valeurMin;

    if (value >= valeurMax)
        return valeurMax;

    return value;
}

bool ServoDriver::testIf(long value, long valeurMin, long valeurMax)
{
    if ((value >= valeurMin) && (value <= valeurMax))
        return true;
    else {
        logger().error() << "SERVO ID NOT EXISTS!! testIf()" << logs::end;
        return false;
    }
}

void ServoDriver::hold(int servo) // 0 à 7
{
    if (!connected_) {
        logger().error() << "SERVO NOT CONNECTED! hold()" << logs::end;
        return;
    }
    if (!testIf(servo, 0, 7))
        return;
    servo_motors_[servo].run();
}

void ServoDriver::setPosition(int servo, int pos)
{
    if (!connected_) {
        logger().error() << "SERVO NOT CONNECTED! setPosition()" << logs::end;
        return;
    }
    constrain(pos, -100, 100);
    if (!testIf(servo, 0, 7))
        return;

    servo_motors_[servo].run();
    servo_motors_[servo].set_position_sp(pos);
    usleep(10000);

}

void ServoDriver::release(int servo)
{
    if (!connected_) {
        logger().error() << "SERVO NOT CONNECTED! release()" << logs::end;
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
        logger().error() << "SERVO NOT CONNECTED! NO setRate !" << logs::end;
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
    //usleep(10000);
}

void ServoDriver::turn(int servo, int speed)
{
    if (!connected_) {
        logger().error() << "SERVO NOT CONNECTED! turn()" << logs::end;
        return;
    }
}

int ServoDriver::getMoving(int servo)
{

    return -999;
}

int ServoDriver::getPos(int servo)
{
    if (!connected_) {
        logger().error() << "SERVO NOT CONNECTED! getPos()" << logs::end;
        return -999;
    }
    if (!testIf(servo, 0, 7))
        return -999;
    return servo_motors_[servo].position_sp();
}

int ServoDriver::ping(int)
{
//    for (int i = 0; i < 8; i++) {
//        logger().debug() << "  " << servo_motors_[i].connected() << logs::end;
//    }

    return servo_motors_[0].connected();
    usleep(10000);
}

