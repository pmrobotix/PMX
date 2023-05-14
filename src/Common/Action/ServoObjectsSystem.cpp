#include "ServoObjectsSystem.hpp"

#include "../../Log/Logger.hpp"
#include "../../Thread/Thread.hpp"
#include "Actions.hpp"

using namespace std;

ServoObjectsSystem::ServoObjectsSystem(std::string botId, Actions &actions) :
        AActionsElement(actions), botId_(botId)
{
    servodriver_ = AServoDriver::create();

}

ServoObjectsSystem::~ServoObjectsSystem()
{
    delete servodriver_;
}

bool ServoObjectsSystem::is_connected()
{
    return servodriver_->is_connected();
}

bool ServoObjectsSystem::setup(int servo, AServoDriver::ServoType type, int valueMinPulse, int valueMidPulse,
        int valueMaxPulse, bool inversed)
{

    if (!servodriver_->is_connected()) {
        logger().error() << "SERVO NOT CONNECTED !" << logs::end;
        return false;
    }

    if (type == AServoDriver::SERVO_DYNAMIXEL) {
        int p = servodriver_->ping(servo);
        //if (p < 0) logger().debug() << "servo=" << servo << " ERROR PING" << logs::end;
        if (p <= 0) {
            logger().error() << "servo=" << servo << " NO PING" << logs::end;
            return false;
        }
    }

    servodriver_->release(servo);

    if (type == AServoDriver::ServoType::SERVO_STANDARD) {

        servodriver_->setType(servo, type);
        servodriver_->setPolarity(servo, inversed);
        servodriver_->setMinPulse(servo, valueMinPulse);
        servodriver_->setMidPulse(servo, valueMidPulse);
        servodriver_->setMaxPulse(servo, valueMaxPulse);

    }
    return true;
}

//keep_millisec = -1 : wait moving until position
//keep_millisec > 0 : time to wait millisec
//keep_millisec = 0 : continue and hold
void ServoObjectsSystem::deployWithVelocity(int servo, int pos, int keep_millisec, int velocity)
{
    hold(servo);
    servodriver_->setPulsePos(servo, pos, velocity); // percentage ms0to90°
    if (keep_millisec > 0) {
        utils::sleep_for_micros(keep_millisec * 1000); //TODO verifier la torque
        release(servo);
    } else if (keep_millisec <= -1) {

        while (int r = servodriver_->getMoving(servo) >= 1) {
            //            if (r<0)
            //            {
            //                logger().info() << "servo=" << servo << " deploy getMoving break"  << logs::end;
            //                break;
            //            }
            r = servodriver_->getMoving(servo);

            //            if (r <= 0 || r >=200) {
            //                r = servodriver_->getMoving(servo);
            //            }
            //            if (r <= 0 || r >=200) {
            //                            r = servodriver_->getMoving(servo);
            //                        }
            //            if (r == 0) {
            //                int torque = servodriver_->getTorque(servo);
            //                if (torque > 600)
            //                {
            //                    logger().info() << "servo=" << servo << " percent= "<< percent << " TOO MUCH TORQUE torque= "<< torque << logs::end;
            //                    release(servo);
            //                    break; //TODO try a 2nde time
            //                }
            //            }

            //logger().info() << "wait "<< r << logs::end;
            utils::sleep_for_micros(2000);
        }
    }
    //int torque = getTorque(servo);
    logger().info() << "deployWithVelocity servo=" << servo << " pos=" << pos << logs::end;
}

//keep_millisec = -1 : wait moving until position
//keep_millisec > 0 : time ms to wait then release
//keep_millisec = 0 : continue and hold
void ServoObjectsSystem::deploy(int servo, int pos, int keep_millisec)
{
    hold(servo);
    servodriver_->setPulsePos(servo, pos); // percentage
    if (keep_millisec > 0) {
        utils::sleep_for_micros(keep_millisec * 1000); //TODO verifier la torque
        release(servo);
    } else if (keep_millisec <= -1) {

        while (int r = servodriver_->getMoving(servo) >= 1) {
//            if (r<0)
//            {
//                logger().info() << "servo=" << servo << " deploy getMoving break"  << logs::end;
//                break;
//            }
            r = servodriver_->getMoving(servo);

//            if (r <= 0 || r >=200) {
//                r = servodriver_->getMoving(servo);
//            }
//            if (r <= 0 || r >=200) {
//                            r = servodriver_->getMoving(servo);
//                        }
//            if (r == 0) {
//                int torque = servodriver_->getTorque(servo);
//                if (torque > 600)
//                {
//                    logger().info() << "servo=" << servo << " percent= "<< percent << " TOO MUCH TORQUE torque= "<< torque << logs::end;
//                    release(servo);
//                    break; //TODO try a 2nde time
//                }
//            }

            //logger().info() << "wait "<< r << logs::end;
            utils::sleep_for_micros(2000);
        }
    }
    //int torque = getTorque(servo);
    logger().info() << "servo=" << servo << " pos= " << pos << logs::end;
}

void ServoObjectsSystem::setTorque(int servo, int torque)
{
    servodriver_->setRate(servo, torque); //torque
}

int ServoObjectsSystem::getTorque(int servo)
{
    return servodriver_->getTorque(servo);
}

//Pour oposul , milli0to90 est la vitesse de 0 à 1023
int ServoObjectsSystem::setPos(int servo, int pos, int milli0to90)
{

    hold(servo);
    servodriver_->setPulsePos(servo, pos, milli0to90);
    return 0; //TODO return utile ?
}

int ServoObjectsSystem::getPulseWidth(int servo)
{
    //Pour les servos STD, permet de simplement lire la valeur de goal
    return servodriver_->getPulsePos(servo);
}

void ServoObjectsSystem::release(int servo)
{
    servodriver_->release(servo);
}

void ServoObjectsSystem::hold(int servo)
{
    servodriver_->hold(servo);
}
int ServoObjectsSystem::getSpeed(int servo)
{
    logger().error() << "NOT IMPLEMENTED !!!" << logs::end;
    return -1;
}
void ServoObjectsSystem::setSpeed(int servo, int speed)
{
    logger().error() << "NOT IMPLEMENTED !!!" << logs::end;
    //servodriver_->setRate(servo, speed); //torque //TODO le vrai setspeed
}

void ServoObjectsSystem::turn(int servo, int speed_percent, int keep_millisec)
{
    logger().error() << "NOT IMPLEMENTED !!!" << logs::end;
}

void ServoObjectsSystem::detectAll()
{
    logger().error() << "NOT IMPLEMENTED !!!" << logs::end;
    for (int i = 0; i < 255; i++) {
        if (int err = servodriver_->ping(i) == 0) {
            logger().info() << i << " err=" << err << logs::end;
        }
    }
}

void ServoObjectsSystem::detect()
{
    logger().error() << "NOT IMPLEMENTED !!!" << logs::end;
//    if (servodriver_->ping(51) != 0) {
//        logger().error() << "ERROR AX12 CONNECTION !!!" << logs::end;
//    }
}

ServoObjectsTimer::ServoObjectsTimer(ServoObjectsSystem & sOsS, int number_servos, uint timeSpan_us, int servo1, int pos1, int velocity) :
        servoObjectsSystem_(sOsS), servo_(servo1), goal_pos_(pos1), velocity_(velocity)
{
    std::string name = std::to_string(number_servos);
    //init du timer POSIX associé ITimerPosixListener ou ITimerListener
    this->init(name, timeSpan_us);

}

//void ServoObjectsSystem::deployByTimerTask(int servo, int percent, int keep_millisec)
//{
// //add the timer to move and wait until arrive at position.
//
//}
void ServoObjectsSystem::move_1_servo(int servo1, int pos1, int torque1, int time_eta_ms, bool keep_torque,
        int escape_torque)
{

}

// lego pos: percentage velocity:ms0to90°
//keep_torque_ms
void ServoObjectsSystem::move_2_servos(int servo1, int pos1, int torque1, int servo2, int pos2, int torque2,
        int time_eta_ms, int keep_torque_extratimems, int escape_torque)
{
    logger().debug() << "move_2_servos create start"  << logs::end;
    hold(servo1);
    hold(servo2);
    //mouvement a faire en time_eta_ms
    //conversion velocity pour 0to90° //correspond à 1000pulses_pwm environ sur un servo std
    //           time_eta_ms            diff en pulses_pwm
    int cur_pos1 = servodriver_->getPulsePos(servo1);
    int cur_pos2 = servodriver_->getPulsePos(servo2);

    logger().debug() << "cur_pos1=" << cur_pos1 << " cur_pos2=" << cur_pos2  << logs::end;
    int diff1 = std::abs(pos1 - cur_pos1);
    int diff2 = std::abs(pos2 - cur_pos2);
    int velocity_ms_0to90_1 = time_eta_ms * 1000.0 / diff1;
    int velocity_ms_0to90_2 = time_eta_ms * 1000.0 / diff2;

    logger().debug() << "velocity_ms_0to90_1=" << velocity_ms_0to90_1 << " velocity_ms_0to90_2=" << velocity_ms_0to90_2  << logs::end;


    logger().debug() << "move_2_servos create timer"  << logs::end;
    this->actions().addTimer(new ServoObjectsTimer(*this, 2, 6000, servo1, pos1, velocity_ms_0to90_1));

    //wait or not wait ?


    /*
    servodriver_->setPulsePos(servo1, pos1, velocity0to90_1);
    servodriver_->setPulsePos(servo2, pos2, velocity0to90_2);

    if (keep_torque_extratimems > 0) {
        utils::sleep_for_micros(keep_torque_extratimems * 1000);
        release (servo1);
        release (servo2);
    } else if (keep_torque_extratimems <= -1) {

        int r1 = 1;
        int r2 = 1;
        while (r1 >= 1 || r2 >= 1) {

            r1 = servodriver_->getMoving(servo1);
            r2 = servodriver_->getMoving(servo2);

            utils::sleep_for_micros(2000);
        }
    }
    //todo esc_torque
    //int torque = getTorque(servo);
    logger().info() << "servo1=" << servo1 << " pos1= " << cur_pos1 << logs::end;
    */
}

void ServoObjectsTimer::onTimer(utils::Chronometer chrono)
{

    //servoObjectsSystem_.servodriver()->setPulsePos(servo_, 1500);
    //logger().error() << "onTimer start chrono_ms=" << chrono.getElapsedTimeInMilliSec() << logs::end;

    if (chrono.getElapsedTimeInMilliSec() >= 4000)
    {
        requestToStop_ = true;

        return;
    }

    /*
    int diff_pos = order_pos_microsec_ - current_pos_;
        //calcul du temps de déplacement souhaité pour diff
    int timing_of_move_ms = rate_ms_per_1000 * std::abs(diff_pos) / 1000.0;

    long t = chrono.getElapsedTimeInMilliSec();
            //std::cout << "init t= "<< t << std::endl;
            long tf = t;
            while (t <= timing_of_move_ms) {
                t = chrono.getElapsedTimeInMilliSec();
                if ((t - tf) > 10) {
                    //calcul de la position pour t
                    int cur_pos = t * 1000.0 / rate_ms_per_1000;
                    //std::cout << " t= " << std::dec << t << " tf= " << std::dec << tf << " cur_pos=" << std::dec << cur_pos << std::endl;
                    int newpos = 1500;
                    if (diff_pos > 0) {
                        newpos = current_pos_ + cur_pos;
                    }
                    else {
                        newpos = current_pos_ - cur_pos;
                    }

                    setpwm(servo, newpos);
                    servo_current_usec_[servo] = newpos;
                    tf = t;
                }
                else std::this_thread::sleep_for(std::chrono::milliseconds(2));
            }
            */
}

void ServoObjectsTimer::onTimerEnd(utils::Chronometer chrono)
{

}

std::string ServoObjectsTimer::info()
{
    std::ostringstream oss;
    oss << "ServoObjectsTimer [" << name() << "] for " << servoObjectsSystem_.id();
    return oss.str();
}

