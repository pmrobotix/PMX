#include "ServoMotorExtend.hpp"

#include <unistd.h>

#include "../Log/Logger.hpp"

//constexpr char LegoAngleSensor::ht_angle[];

ServoMotorExtend::ServoMotorExtend(address_type address) :
        servo_motor(address)
{
    address_ = address;
    if (this->connected()) {
        //Mise à jour du polling pour mettre a jour en i2c toutes les n ms
        //this->set_poll_ms(1);
        //this->set_mode("ANGLE-ACC");

        fd_position_sp_in_ = this->open_posix_in(_path + "position_sp");

    }
}

void ServoMotorExtend::setPositionPOSIX(int percent) {

    if (this->connected()) {
        logger().error() << "ServoMotorExtend::setPosition = " << _path << " " << percent << logs::end;



        int out = open_posix_out(_path + "position_sp");
        this->set_attr_int_posix(out, percent);
        this->close_posix(out);

//        int outcmd = open_posix_out(_path + "command");
//        this->set_attr_string_posix(outcmd, "run");
//        this->close_posix(outcmd);
    }
//    if (!connected_servo[constrain(servo, 0, 7)]) {
//        logger().error() << "SERVO NOT CONNECTED! setPosition() servo=" << servo << logs::end;
//        return;
//    }
//    logger().error() << "TEST " << servo << logs::end;
//
//    servo_motors_[constrain(servo, 0, 7)].set_position_sp(constrain(percent, -100, 100));
//
//    //servo_motors_[servo].run();
//
//    //usleep(10000);
}
int ServoMotorExtend::getPositionPOSIX() {
    //logger().error() << "TEST " << this->open_get_attr_int_close_posix(_path + "position_sp") << logs::end;

    //return this->open_get_attr_int_close_posix(_path + "position_sp");
    return this->get_attr_int_posix(fd_position_sp_in_);
}

//void ServoMotorExtend::reset()
//{
//    logger().info() << " addr= " << address() << " " << this->_path << logs::end;
//    if (this->connected())
//        this->set_command("RESET");
//}
//
//int ServoMotorExtend::getValueDegrees()
//{
//    //logger().info() << " getValueDegrees " << logs::end;
//    if (this->connected()) {
//
//        //int accumulatedAngleDegrees = this->get_attr_int_posix(fd_value0_);
//        //int accumulatedAngleDegrees =  this->get_attr_int("value0");
//        //int accumulatedAngleDegrees =  this->value();
//
//        //logger().info() << address_ << " " << ticks << logs::end;
//        return accumulatedAngleDegrees;
//    } else
//        return -999999;
//}

