//drivers...EV3

#include "ServoDriverMindstorm.hpp"

#include <unistd.h>

#include "../Log/Logger.hpp"
#include "ev3dev.h"
#include <string>

using namespace std;
using namespace ev3dev;
//
//AServoDriver * AServoDriver::create(ServoType type) {
//    if (type == AServoDriver::SERVO_STANDARD) {
//        static ServoDriver *instance = new ServoDriver();
//        return instance;
//    }
//    else return NULL;
//}

ServoDriverMindstorm::ServoDriverMindstorm() :
        connected_(0)
{
    *connected_servo = {0};
    logger().debug() << "ServoDriver()" << logs::end;

    for (int x = 1;x <= 8;x++)
    {
        string servo_name = "ev3-ports:in4:i2c88:sv"+std::to_string(x);

        servo_motors_[x-1] = ServoMotorExtend(servo_name);
        if (ping(x-1)) //test sur le servo 0 pour savoir si la carte est branchée
        {
            logger().info() << "Servo "  << x - 1 << " connected." << logs::end;
            connected_servo[x-1] = 1;
            if (!connected_)
            connected_ = 1;
        }
    }

//remplacer la configuration /etc/modprobe.d
    //http://docs.ev3dev.org/projects/lego-linux-drivers/en/ev3dev-stretch/i2c.html#practical-examples
//    if (connected_) {
//        //mise du polling à 0 pour la carte de servo.
//        i2c_sensor servo_sensor = i2c_sensor("ev3-ports:in4:i2c88");
//        //servo_sensor.set_poll_ms(15);
//    }
}

//bool ServoDriver::testIf(long value, long valeurMin, long valeurMax) {
//    if ((value >= valeurMin) && (value <= valeurMax)) return true;
//    else {
//        logger().error() << "SERVO ID NOT EXISTS!! testIf() " << "value=" << value << " min=" << valeurMin << " max=" << valeurMax << logs::end;
//        return false;
//    }
//}


void ServoDriverMindstorm::hold(int servo) // 0 à 7
{
    if (!connected_servo[constrain(servo, 0, 7)]) {
        logger().error() << "SERVO NOT CONNECTED! hold() servo=" << servo << logs::end;
        return;
    }

    servo_motors_[constrain(servo, 0, 7)].run();
}

void ServoDriverMindstorm::setPosition(int servo, int percent) {
    if (!connected_servo[constrain(servo, 0, 7)]) {
        logger().error() << "SERVO NOT CONNECTED! setPosition() servo=" << servo << logs::end;
        return;
    }


    //servo_motors_[constrain(servo, 0, 7)].set_position_sp(constrain(percent, -100, 100));
    servo_motors_[constrain(servo, 0, 7)].setPositionPOSIX(percent);


    //servo_motors_[constrain(servo, 0, 7)].run();
}

void ServoDriverMindstorm::release(int servo) {
    if (!connected_servo[constrain(servo, 0, 7)]) {
        logger().error() << "SERVO NOT CONNECTED! release() servo=" << servo << logs::end;
        return;
    }
    servo_motors_[constrain(servo, 0, 7)].float_();
    usleep(10000);
}

void ServoDriverMindstorm::setRate(int servo, int millisec0To90) {
    if (!connected_servo[constrain(servo, 0, 7)]) {
        logger().error() << "SERVO NOT CONNECTED! NO setRate ! servo=" << servo << logs::end;
        return;
    }

    // Sets the rate_sp at which the servo travels from 0 to 100.0% (half of the full
    // range of the servo). Units are in milliseconds. Example: Setting the rate_sp
    // to 1000 means that it will take a 180 degree servo 2 second to move from 0
    // to 180 degrees. Note: Some servo controllers may not support this in which
    // case reading and writing will fail with `-EOPNOTSUPP`.
    servo_motors_[servo].set_rate_sp(millisec0To90);


}

void ServoDriverMindstorm::turn(int servo, int speed) {
    if (!connected_servo[constrain(servo, 0, 7)]) return;
}

int ServoDriverMindstorm::getMoving(int servo) {
    return -999;
}

int ServoDriverMindstorm::getPos(int servo) {
    if (!connected_servo[constrain(servo, 0, 7)]) {
        logger().error() << "SERVO NOT CONNECTED! getPos() servo=" << servo << logs::end;
        return -999;
    }
    return servo_motors_[constrain(servo, 0, 7)].position_sp();
}

int ServoDriverMindstorm::ping(int servo) {
    return servo_motors_[constrain(servo, 0, 7)].connected();
}

void ServoDriverMindstorm::setMinPulse(int servo, int pulse) {
    if (!connected_servo[constrain(servo, 0, 7)]) return;
    servo_motors_[constrain(servo, 0, 7)].set_min_pulse_sp(constrain(pulse, 0, 1000)); //default 500 [0 to 1000]
}

void ServoDriverMindstorm::setMidPulse(int servo, int pulse) {
    if (!connected_servo[constrain(servo, 0, 7)]) return;
    servo_motors_[constrain(servo, 0, 7)].set_mid_pulse_sp(constrain(pulse, 1000, 2000)); //default 1500 [1000 to 2000]
}

void ServoDriverMindstorm::setMaxPulse(int servo, int pulse) {
    if (!connected_servo[constrain(servo, 0, 7)]) return;
    servo_motors_[constrain(servo, 0, 7)].set_max_pulse_sp(constrain(pulse, 2000, 3000)); //default 2500 [2000 to 3000]
}

void ServoDriverMindstorm::setPolarity(int servo, bool inversed) {
    if (!connected_servo[constrain(servo, 0, 7)]) return;
    if (!inversed) servo_motors_[constrain(servo, 0, 7)].set_polarity(servo_motor::polarity_normal);
    else servo_motors_[servo].set_polarity(servo_motor::polarity_inversed);
}

int ServoDriverMindstorm::getTorque(int servo) {
    if (!connected_servo[constrain(servo, 0, 7)]) return -99;
    return -1;
}
