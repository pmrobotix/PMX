//drivers...EV3

#include "AsservDriver.hpp"

#include <unistd.h>
#include <string>

#include "../Log/Logger.hpp"
#include "ev3dev.h"

using namespace std;
using namespace ev3dev;
using namespace std::chrono;

AAsservDriver* AAsservDriver::create(std::string) {
    static AsservDriver *instance = new AsservDriver();
    return instance;
}

AsservDriver::AsservDriver() :
        _motor_right_(OUTPUT_D), _motor_left_(OUTPUT_B), mag_(true, false, false)
{
    logger().debug() << "AsservDriver()" << logs::end;
    m_encoderLSum = 0;
    m_encoderRSum = 0;
    m_encoder1Previous = 0.0;
    m_encoder2Previous = 0.0;

    mag_.init();

    /*
     //_______________________________________TEST OLD
     while (1) {
     usleep(1000);
     uint8_t d1 = mag_.getDiag1();
     printf("\nd=%d ", d1);

     uint8_t g1 = mag_.getAutoGain1();
     printf("\ng=%d ", g1);

     uint16_t l1 = mag_.getAngle1();
     printf("\nl=%d ", l1);

     uint16_t m1 = mag_.getMag1();
     printf("\nm=%d \n", m1);

     usleep(10000);
     uint8_t d2 = mag_.getDiag2();
     printf("\nd2=%d ", d2);

     uint8_t g2 = mag_.getAutoGain2();
     printf("\ng2=%d ", g2);

     uint16_t l2 = mag_.getAngle2();
     printf("\nl2=%d ", l2);

     uint16_t m2 = mag_.getMag2();
     printf("\nm2=%d \n", m2);
     usleep(10000);
     }
     */
/*
//_______________________________________TEST
    while (1) {
        //auto start = std::chrono::system_clock::now();
        //long test = getLeftExternalEncoder();
        //        auto end = system_clock::now();
        //        logger().debug()
        //                        << "  duration= " << (duration_cast<microseconds>(end - start).count()) << logs::end;
        //std::this_thread::sleep_for(std::chrono::microseconds(500));

        auto start = std::chrono::system_clock::now();
        float left = 0.0;
        float right = 0.0;
        int err = mag_.getValues(&right, &left);
        int ll = 0;
        int rr = 0;
        mag_.getEncodersTotalCount(&rr, &ll);
        auto end = system_clock::now();

        logger().info() << "l= "
                << left
                << "\tr= "
                << right
                << "\tL= "
                << ll
                << "\tR= "
                << rr
                << "\tt= "
                << (duration_cast<microseconds>(end - start).count())
                << " err=" << err
                << logs::end;

        std::this_thread::sleep_for(std::chrono::microseconds(20000));

        //        auto start = std::chrono::system_clock::now();
        //        long l1 = (long) mag_.getAccumulatedDirectValueEncoder1();
        //        long l2 = (long) mag_.getAccumulatedDirectValueEncoder2();
        //        auto end = system_clock::now();
        //        logger().debug() << " l1= " << l1 << " l2= " << l2
        //                << " duration= " << (duration_cast<microseconds>(end - start).count()) << logs::end;
        //        std::this_thread::sleep_for(std::chrono::microseconds(3000));
    }
//_______________________________________TEST
*/

    if (_motor_right_.connected()) {

        logger().debug() << "("
                << "RIGHT"
                << ") "
                << _motor_right_.driver_name()
                << " motor on port "
                << _motor_right_.address()
                << " Pol="
                << _motor_right_.polarity()
                << logs::end;
    }
    else {
        logger().error() << "NOT CONNECTED! NO _motor_right_ !" << logs::end;
    }

    if (_motor_left_.connected()) {

        logger().debug() << "("
                << "LEFT "
                << ") "
                << _motor_left_.driver_name()
                << " motor on port "
                << _motor_left_.address()
                << " Pol="
                << _motor_left_.polarity()
                << logs::end;
    }
    else {
        logger().error() << "NOT CONNECTED! NO _motor_left_ !" << logs::end;
    }

    resetEncoders();

}

void AsservDriver::reset() {
    if (_motor_left_.connected()) _motor_left_.reset();

    if (_motor_right_.connected()) _motor_right_.reset();
}

void AsservDriver::endWhatTodo() {

    //TODO ????

}

void AsservDriver::setMotorLeftPosition(int ticks_per_second, long ticks) {
    limit(ticks_per_second, MAXVALUE_speed_sp);
    if (_motor_left_.connected()) {

        //ticks :the units are in tachometer pulse counts, so if you are not using a LEGO motor, then you will need to do some math to convert to/from degrees.
        _motor_left_.set_position_sp(ticks).set_speed_sp(ticks_per_second).run_to_rel_pos();
    }
}

void AsservDriver::setMotorRightPosition(int ticks_per_second, long ticks) {
    limit(ticks_per_second, MAXVALUE_speed_sp);
    if (_motor_right_.connected()) {
        //the units are in tachometer pulse counts, so if you are not using a LEGO motor, then you will need to do some math to convert to/from degrees.
        _motor_right_.set_position_sp(ticks).set_speed_sp(ticks_per_second).run_to_rel_pos();
    }
}

int AsservDriver::limit(int power, int max) {
    if ((power < -max)) {
        logger().info() << "ERROR Motor power " << power << " exceeded minimum " << -max << "!!" << logs::end;
        power = -max;
    }
    else if (power > max) {
        logger().info() << "ERROR Motor power " << power << "exceeded maximum " << max << "!!" << logs::end;
        power = max;
    }
    return power;
}

//regulation enabled  => power in ticks per second -860 / +860
//regulation disabled => power in percentage -100 / +100

// Commands: read-only
// Returns a list of commands that are supported by the motor
// controller. Possible values are `run-forever`, `run-to-abs-pos`, `run-to-rel-pos`,
// `run-timed`, `run-direct`, `stop` and `reset`. Not all commands may be supported.
//
// - `run-forever` will cause the motor to run until another command is sent.
// - `run-to-abs-pos` will run to an absolute position specified by `position_sp`
//   and then stop using the action specified in `stop_action`.
// - `run-to-rel-pos` will run to a position relative to the current `position` value.
//   The new position will be current `position` + `position_sp`. When the new
//   position is reached, the motor will stop using the action specified by `stop_action`.
// - `run-timed` will run the motor for the amount of time specified in `time_sp`
//   and then stop the motor using the action specified by `stop_action`.
// - `run-direct` will run the motor at the duty cycle specified by `duty_cycle_sp`.
//   Unlike other run commands, changing `duty_cycle_sp` while running *will*
//   take effect immediately.
// - `stop` will stop any of the run commands before they are complete using the
//   action specified by `stop_action`.
// - `reset` will reset all of the motor parameter attributes to their default value.
//   This will also have the effect of stopping the motor.

//http://www.ev3dev.org/docs/tutorials/tacho-motors/
void AsservDriver::setMotorLeftPower(int value, int timems) {
    if (_motor_left_.connected()) {
        if (timems == 0) {
            limit(value, MAXVALUE_speed_sp);
            _motor_left_.set_speed_sp(value).run_forever();
        }
        else if (timems > 0) {
            limit(value, MAXVALUE_speed_sp);
            //logger().debug() << "LEFT  value = " << value << logs::end;
            _motor_left_.set_speed_sp(value).set_time_sp(timems).run_timed();
        }
        else {
            limit(value, MAXVALUE_duty_cycle_sp);
            //logger().debug() << "LEFT  percent = " << value << logs::end;
            //_motor_left_.set_duty_cycle_sp(value).run_direct();
            _motor_left_.custom_set_duty_cycle_sp_run_direct(value);
        }
    }
}

void AsservDriver::setMotorRightPower(int value, int timems) {
    if (_motor_right_.connected()) {
        if (timems == 0) {
            limit(value, MAXVALUE_speed_sp);
            _motor_right_.set_speed_sp(value).run_forever();
        }
        else if (timems > 0) {
            limit(value, MAXVALUE_speed_sp);
            //logger().debug() << "RIGHT value = " << value << logs::end;
            _motor_right_.set_speed_sp(value).set_time_sp(timems).run_timed();
        }
        else {
            limit(value, MAXVALUE_duty_cycle_sp);
            //logger().error() << "RIGHT percent = " << value << logs::end;
            //_motor_right_.set_duty_cycle_sp(value).run_direct();
            _motor_right_.custom_set_duty_cycle_sp_run_direct(value);
        }
    }
}

void AsservDriver::getCountsExternal(int32_t* countR, int32_t* countL) {
    float R = 0.0;
    float L = 0.0;
    mag_.getValues(&R, &L);
    *countR = (int) R;
    *countL = (int) L;
}

long AsservDriver::getLeftExternalEncoder() {
    //utiliser pour test uniquement arbitrairement ici LEFT=Encoder1
    int count = 0;
    int err = mag_.getAccumulatedDirectValueEncoder1(&count);
    if (err < 0) logger().error() << "getLeftExternalEncoder Encoder1 err=" << err << logs::end;
    return count;
}

long AsservDriver::getRightExternalEncoder() {
    //utiliser pour test uniquement arbitrairement ici RIGHT=Encoder2
    int count = 0;
    int err = mag_.getAccumulatedDirectValueEncoder2(&count);
    if (err < 0) logger().error() << "getRightExternalEncoder Encoder2 err=" << err << logs::end;
    return count;
}

void AsservDriver::getCountsInternal(int32_t* countR, int32_t* countL) {
    printf("\nsservDriver getCountsInternal NOT IMPLEMENTED !!\n");
    //TODO avec encoder motor

//    *countR = getRightExternalEncoder();
//    *countL = getLeftExternalEncoder();

}

long AsservDriver::getLeftInternalEncoder() {
    if (_motor_left_.connected()) {
        //+/- 2,147,483,648
        return _motor_left_.custom_get_position();
    }
    else return 0;
}

long AsservDriver::getRightInternalEncoder() {
    if (_motor_right_.connected()) {

        return _motor_right_.custom_get_position();

    }
    else return 0;
}

void AsservDriver::stopMotorLeft() {
    setMotorLeftPower(0, 0);
    if (_motor_left_.connected()) {
        //_motor_left_.stop();
        _motor_left_.custom_set_command("stop");
    }
    else logger().error() << "NOT CONNECTED! NO stopMotorLeft" << logs::end;
}

void AsservDriver::stopMotorRight() {
    setMotorRightPower(0, 0);
    if (_motor_right_.connected()) {
        //_motor_right_.stop();
        _motor_right_.custom_set_command("stop");
    }
    else logger().error() << "NOT CONNECTED! NO stopMotorRight" << logs::end;
}

void AsservDriver::resetEncoders() {
    resetInternalEncoders();
    resetExternalEncoders();
}

void AsservDriver::resetInternalEncoders() {
    if (_motor_left_.connected()) {
        _motor_left_.set_position(0);
    }
    if (_motor_right_.connected()) {
        _motor_right_.set_position(0);
    }
}

void AsservDriver::resetExternalEncoders() {
    mag_.reset();
}

int AsservDriver::getMotorLeftCurrent() {
    return 0;
}

int AsservDriver::getMotorRightCurrent() {
    return 0;
}

void AsservDriver::odo_SetPosition(float x_mm, float y_mm, float angle_rad) {
}

RobotPosition AsservDriver::odo_GetPosition() {

    return {0,0,0,0};
}

//DEPRECATED
int AsservDriver::path_GetLastCommandStatus() {
    return 0;
}

void AsservDriver::path_InterruptTrajectory() {
}

void AsservDriver::path_CollisionOnTrajectory() {
}

void AsservDriver::path_CollisionRearOnTrajectory() {
}

void AsservDriver::path_CancelTrajectory() {
}

void AsservDriver::path_ResetEmergencyStop() {
}

TRAJ_STATE AsservDriver::motion_DoFace(float x_mm, float y_mm) {
    return TRAJ_ERROR;
}
TRAJ_STATE AsservDriver::motion_DoLine(float dist_mm) {

    return TRAJ_ERROR;
}
TRAJ_STATE AsservDriver::motion_DoRotate(float angle_radians) {
    return TRAJ_ERROR;
}
TRAJ_STATE AsservDriver::motion_DoArcRotate(float angle_radians, float radius) {
    return TRAJ_ERROR;
}

TRAJ_STATE AsservDriver::motion_Goto(float x_mm, float y_mm) {

    return TRAJ_ERROR;
}

TRAJ_STATE AsservDriver::motion_GotoReverse(float x_mm, float y_mm) {

    return TRAJ_ERROR;
}

TRAJ_STATE AsservDriver::motion_GotoChain(float x_mm, float y_mm) {

    return TRAJ_ERROR;
}

TRAJ_STATE AsservDriver::motion_GotoReverseChain(float x_mm, float y_mm) {

    return TRAJ_ERROR;
}

void AsservDriver::motion_FreeMotion() {
}

//TODO deprecated  mm chose que Freemotion ???
void AsservDriver::motion_DisablePID() {
}
void AsservDriver::motion_AssistedHandling() {
}
void AsservDriver::motion_ActivateManager(bool enable) {

    //TODO ???
}

void AsservDriver::motion_setLowSpeedForward(bool enable, int) {

}

void AsservDriver::motion_setLowSpeedBackward(bool enable, int) {

}

void AsservDriver::motion_ActivateReguDist(bool enable) {

}
void AsservDriver::motion_ActivateReguAngle(bool enable) {

}
void AsservDriver::motion_ResetReguDist() {

}
void AsservDriver::motion_ResetReguAngle() {

}
TRAJ_STATE AsservDriver::motion_DoDirectLine(float dist_mm) {
    return TRAJ_ERROR;
}
