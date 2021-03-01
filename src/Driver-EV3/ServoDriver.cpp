//drivers...EV3

#include "ServoDriver.hpp"

#include <unistd.h>
#include <iostream>
#include <string>

#include "../Common/Utils/Chronometer.hpp"
#include "../Log/Logger.hpp"

using namespace std;


AServoDriver * AServoDriver::create(ServoType type) {
    if (type == AServoDriver::SERVO_STANDARD) {
        static ServoDriver *instance = new ServoDriver();
        return instance;
    }
    else return NULL;
}

ServoDriver::ServoDriver() :
        pwm_(0x60)
{
    logger().debug() << "ServoDriver()" << logs::end;

    *servo_current_usec_ = {1500};
    *servo_min_= {500};
    *servo_mid_= {1500};
    *servo_max_= {2500};
    *servo_inv_= {false};
    *servo_rate_= {0};
    pwm_.begin();

//    setMinPulse(0,500);
//    setMaxPulse(0,2600);
//    while (1) {
//
//        setRate(0, 0);
//        setPositionWithRate(0, 500);
//        usleep(800000);
//
//        setRate(0, 2000);
//        setPositionWithRate(0, 2600);
//
//        usleep(800000);
//    }

}

void ServoDriver::hold(int servo) // 0 à 7
{
    pwm_.fastWriteMicroseconds(constrain(servo, 0, NB_SERVO - 1), servo_current_usec_[constrain(servo, 0, NB_SERVO - 1)]);
}

void ServoDriver::setPositionWithRate(int servo, int pos_microsec) {
    //version 3
        utils::Chronometer chrono("ServoDriver::setPositionWithRate");
        int order_pos_microsec_ = constrain(pos_microsec, servo_min_[constrain(servo, 0, NB_SERVO - 1)], servo_max_[constrain(servo, 0, NB_SERVO - 1)]);
        int current_pos_ = servo_current_usec_[constrain(servo, 0, NB_SERVO - 1)];
        int rate_ms_per_1000 = servo_rate_[constrain(servo, 0, NB_SERVO - 1)];
        if (rate_ms_per_1000 == 0) {
            setPosition(servo, pos_microsec);
        }
        int diff_pos = order_pos_microsec_ - current_pos_;
        //calcul du temps de déplacement souhaité pour diff
        int timing_of_move_ms = rate_ms_per_1000 * diff_pos / 1000.0;

        std::cout << "order_pos_microsec_= "<< order_pos_microsec_
                << " current_pos_= "<< current_pos_
                << " rate_ms_per_1000= "<< rate_ms_per_1000
                << " diff_pos= "<< diff_pos
                << " timing_of_move_ms= "<< timing_of_move_ms

                << std::endl;

        if (diff_pos > 0) {
            chrono.start();
            long t = chrono.getElapsedTimeInMilliSec();
            //std::cout << "init t= "<< t << std::endl;
            long tf = t;
            while (t <= timing_of_move_ms) {
                t = chrono.getElapsedTimeInMilliSec();
                if ((t - tf) > 20) {
                    //calcul de la position pour t
                    int cur_pos = t * 1000.0 / rate_ms_per_1000;
                    std::cout << " t= "<< t
                            << " tf= "<< tf
                            << " cur_pos=" << cur_pos
                            << std::endl;
                    setPosition(servo, current_pos_+ cur_pos);
                    tf = t;
                }else
                    std::this_thread::sleep_for(std::chrono::milliseconds(2));
            }
        }

/*
    //version 2
    utils::Chronometer chrono("ServoDriver::setPositionWithRate");
    int order_pos_microsec_ = constrain(pos_microsec, servo_min_[constrain(servo, 0, NB_SERVO - 1)], servo_max_[constrain(servo, 0, NB_SERVO - 1)]);
    int current_pos_ = servo_current_usec_[constrain(servo, 0, NB_SERVO - 1)];
    int rate_ms_per_1000 = servo_rate_[constrain(servo, 0, NB_SERVO - 1)];
    if (rate_ms_per_1000 == 0) {
        setPosition(servo, pos_microsec);
    }
    int diff_pos = order_pos_microsec_ - current_pos_;
    //calcul du temps de déplacement souhaité pour diff
    int timing_of_move_ms = rate_ms_per_1000 * diff_pos / 1000.0;

    std::cout << "order_pos_microsec_= "<< order_pos_microsec_
            << " current_pos_= "<< current_pos_
            << " rate_ms_per_1000= "<< rate_ms_per_1000
            << " diff_pos= "<< diff_pos
            << " timing_of_move_ms= "<< timing_of_move_ms

            << std::endl;

    if (diff_pos > 0) {
        chrono.start();
        long t = chrono.getElapsedTimeInMilliSec();
        //std::cout << "init t= "<< t << std::endl;
        long tf = t;
        while (t < timing_of_move_ms) {
            t = chrono.getElapsedTimeInMilliSec();
            if ((t - tf) > 10) {
                //calcul da la position pour t
                int cur_pos = t * 1000.0 / rate_ms_per_1000;
                std::cout << " t= "<< t
                        << " tf= "<< tf
                        << " cur_pos=" << cur_pos
                        << std::endl;
                setPosition(servo, current_pos_+ cur_pos);
                tf = t;
            }else
                    std::this_thread::sleep_for(std::chrono::milliseconds(2));
        }
    }
*/
    /*//version 1
     int microsec_ = constrain(microsec, servo_min_[constrain(servo, 0, NB_SERVO - 1)], servo_max_[constrain(servo, 0, NB_SERVO - 1)]);
     int current_pos_ = servo_current_usec_[constrain(servo, 0, NB_SERVO - 1)];
     int rate_ = servo_rate_[constrain(servo, 0, NB_SERVO - 1)];
     if (rate_ == 0)
     {
     setPosition(servo, microsec);
     }
     int diff = current_pos_ - microsec_;
     //25usec pour laisser au moins 7ms le temps d'une requete i2c
     if (diff < 0)
     for (int pos = current_pos_; pos < microsec_; pos += 25) {
     setPosition(constrain(servo, 0, NB_SERVO - 1), pos);
     int waitms = rate_ / 1000.0 * 24;

     usleep(waitms*1000.0);
     }
     */

}
void ServoDriver::setPosition(int servo, int microsec) {

    int microsec_ = constrain(microsec, servo_min_[constrain(servo, 0, NB_SERVO - 1)], servo_max_[constrain(servo, 0, NB_SERVO - 1)]);

    servo_current_usec_[constrain(servo, 0, NB_SERVO - 1)] = microsec_;
    pwm_.fastWriteMicroseconds(constrain(servo, 0, NB_SERVO - 1), servo_current_usec_[constrain(servo, 0, NB_SERVO - 1)]);
}

void ServoDriver::release(int servo) {
    pwm_.fastWriteMicroseconds(constrain(servo, 0, NB_SERVO - 1), 0);
}

void ServoDriver::setRate(int servo, int millisec0To90) {

    // Sets the rate_sp at which the servo travels from 0 to 100.0% (half of the full
    // range of the servo). Units are in milliseconds. Example: Setting the rate_sp
    // to 1000 means that it will take a 180 degree servo 2 second to move from 0
    // to 180 degrees. Note: Some servo controllers may not support this in which
    // case reading and writing will fail with `-EOPNOTSUPP`.


    //speed servo SG5010 : 0.11 sec/60°
    //Speed servo 225BB : 0,11 sec/60°

    servo_rate_[constrain(servo, 0, NB_SERVO - 1)] = constrain(millisec0To90, 0, 10000);

}

void ServoDriver::turn(int servo, int speed) {
    return;
}

int ServoDriver::getMoving(int servo) {
    return -1;
}

int ServoDriver::getPos(int servo) {
    return servo_current_usec_[constrain(servo, 0, NB_SERVO - 1)];
}

int ServoDriver::ping(int servo) {
    return 0;
}

void ServoDriver::setMinPulse(int servo, int pulse) {
    servo_min_[constrain(servo, 0, NB_SERVO - 1)] = constrain(pulse, 0, 1000);    //default 500 [0 to 1000]
}

void ServoDriver::setMidPulse(int servo, int pulse) {
    servo_mid_[constrain(servo, 0, NB_SERVO - 1)] = constrain(pulse, 1000, 2000); //default 1500 [1000 to 2000]
}

void ServoDriver::setMaxPulse(int servo, int pulse) {
    servo_max_[constrain(servo, 0, NB_SERVO - 1)] = constrain(pulse, 2000, 3000); //default 2500 [2000 to 3000]
}

void ServoDriver::setPolarity(int servo, bool inverted) {
    servo_inv_[constrain(servo, 0, NB_SERVO - 1)] = inverted; //TODO Faire les calculs en inversé sur la position par rapport au milieu
}

int ServoDriver::getTorque(int servo) {
    return -1;
}
