//drivers...EV3

#include "ServoUsingMotorDriver.hpp"

#include "../Log/Logger.hpp"

using namespace std;
using namespace ev3dev;

AServoUsingMotorDriver * AServoUsingMotorDriver::create()
{
    static ServoUsingMotorDriver *instance = new ServoUsingMotorDriver();
    return instance;
}

ServoUsingMotorDriver::ServoUsingMotorDriver() :
        connected_(0), _servo_device(OUTPUT_B)
{
    logger().debug() << "ServoDeviceDriver()" << logs::end;

    _servo_device.reset();
    _servo_device.set_polarity(motor::polarity_normal);
    _servo_device.set_ramp_down_sp(0);
    _servo_device.set_ramp_up_sp(0);
    _servo_device.set_stop_action(motor::stop_action_brake);

    if (_servo_device.connected()) {

        logger().info() << "(" << "RIGHT" << ") " << _servo_device.driver_name() << " motor on port "
                << _servo_device.address() << " Pol=" << _servo_device.polarity() << logs::end;
    } else {
        logger().error() << "NOT CONNECTED! NO _servo_device !" << logs::end;
    }

}

void ServoUsingMotorDriver::setMotorPosition(int power, int ticks, int ramptimems)
{
    if (_servo_device.connected())
    {
        _servo_device.set_ramp_down_sp(ramptimems);
        _servo_device.set_ramp_up_sp(ramptimems);

        power = limit(power, MAXVALUE_speed_sp);

        logger().error() << "ticks= "<< ticks << " power=" << power<< logs::end;
        _servo_device.set_position_sp(ticks).set_speed_sp(power).run_to_abs_pos();
        //_motor_left. set_position_sp( direction).set_speed_sp(500).run_to_rel_pos();
    }

    /*
 if (_servo_device.connected())
 {
 _servo_device.set_ramp_down_sp(ramptimems);
 _servo_device.set_ramp_up_sp(ramptimems);
 if (_servo_device.speed_regulation_enabled() == "on") //speed_sp
 {
 power = limit(power, MAXVALUE_speed_sp); //real MAX speed of EV3
 _servo_device.set_position_sp(ticks).set_speed_sp(power).run_to_rel_pos();
 }
 else if (_servo_device.speed_regulation_enabled() == "off") //duty_cycle_sp
 {
 power = limit(power, MAXVALUE_duty_cycle_sp);
 _servo_device.set_position_sp(ticks).set_duty_cycle_sp(power).run_to_rel_pos();
 }
 logger().error() << "ticks = " << ticks << logs::end;
 }*/
}

long ServoUsingMotorDriver::getInternalEncoder()
{
    if (_servo_device.connected()) {
        return _servo_device.position();
    } else
        return 0;
}

void ServoUsingMotorDriver::stopMotor()
{

    if (_servo_device.connected()) {
        _servo_device.stop();
    }
}

void ServoUsingMotorDriver::resetEncoder(int pos)
{
    if (_servo_device.connected()) {
        _servo_device.set_position(pos);
    }
}

int ServoUsingMotorDriver::getMotorCurrent()
{
    return 0;
}


int ServoUsingMotorDriver::limit(int power, int max)
{
    if ((power < -max)) {
        logger().error() << "ERROR Motor power " << power << " exceeded minimum " << -max << "!!" << logs::end;
        power = -max;
    } else if (power > max) {
        logger().error() << "ERROR Motor power " << power << "exceeded maximum " << max << "!!" << logs::end;
        power = max;
    }
    return power;
}
