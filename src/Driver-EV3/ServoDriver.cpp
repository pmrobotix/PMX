//drivers...EV3

#include "ServoDriver.hpp"

#include <unistd.h>
#include <iostream>
#include <string>

#include "../Common/Action.Driver/AServoDriver.hpp"
#include "../Common/Utils/Chronometer.hpp"
#include "../Log/Logger.hpp"
#include "CCAx12Adc.hpp"
#include "CCAx12Teensy.hpp"
#include "../Common/Utils/json.hpp"

using namespace std;

AServoDriver * AServoDriver::create() {
    static ServoDriver *instance = new ServoDriver();
    return instance;
}

ServoDriver::ServoDriver() :
        pwm_(1, 0x60) //ServoStandard card
{

    *servo_current_usec_ = {1500};
    *servo_min_= {500};
    *servo_mid_= {1500};
    *servo_max_= {2500};
    *servo_inv_= {false};
    *servo_rate_= {0};
    *servo_type_= {AServoDriver::SERVO_STANDARD};

    int connected = pwm_.connect(50); //freq 50 (periode 20ms) pour servo analogique
    if(!connected)
        logger().error() << "ServoDriver() pwm 0x60 not CONNECTED!" << logs::end;



//    CCAx12Adc::instance().connect(0x08);
//
//    int conn = CCAx12Teensy::instance().connect(9);
//
//    while(1)
//          {
//              CCAx12Teensy::instance().setLedOn(4);
//              usleep(500000);
//              CCAx12Teensy::instance().setLedOff(4);
//              usleep(500000);
//              logger().error() << "CCAx12Teensy..."  << conn << logs::end;
//          }



//    CCAx12Adc::instance().setLedOn(1);
//    CCAx12Adc::instance().setLedOn(2);
//    CCAx12Adc::instance().setLedOn(3);
//    CCAx12Adc::instance().setLedOn(4);
//    CCAx12Adc::instance().setLedOn(5);
//    CCAx12Adc::instance().setLedOn(6);
//    CCAx12Adc::instance().setLedOn(7);
//    CCAx12Adc::instance().setLedOn(8);
    //CCAx12Adc::instance().setLedOn(9);
//    sleep(1);
//    CCAx12Adc::instance().setLedOff(1);
//    CCAx12Adc::instance().setLedOff(2);
//    CCAx12Adc::instance().setLedOff(3);
//    CCAx12Adc::instance().setLedOff(4);
//    CCAx12Adc::instance().setLedOff(5);
//    CCAx12Adc::instance().setLedOff(6);
//    CCAx12Adc::instance().setLedOff(7);
//    CCAx12Adc::instance().setLedOff(8);
    //CCAx12Adc::instance().setLedOff(9);

//    int r = CCAx12Adc::instance().readAXData(3, CMD_PING_AX);
//    logger().info() << "r=" << r << logs::end;

//    while (1)
//    {
//    int p = CCAx12Adc::instance().readAXData(3, P_PRESENT_POSITION);
//    logger().info() << "p=" << p << logs::end;
//    }
    //int r = CCAx12Adc::instance().writeAXData(8, P_TORQUE_ENABLE, 1);



/*
    CCAx12Adc::instance().connect(0x08);
    CCAx12Adc::instance().setLedOn(1);
    CCAx12Adc::instance().setLedOn(2);
    CCAx12Adc::instance().setLedOn(3);
    CCAx12Adc::instance().setLedOn(4);
    CCAx12Adc::instance().setLedOn(5);
    CCAx12Adc::instance().setLedOn(6);
    CCAx12Adc::instance().setLedOn(7);
    CCAx12Adc::instance().setLedOn(8);
    CCAx12Adc::instance().setLedOn(9);
    sleep(1);
    CCAx12Adc::instance().setLedOff(1);
    CCAx12Adc::instance().setLedOff(2);
    CCAx12Adc::instance().setLedOff(3);
    CCAx12Adc::instance().setLedOff(4);
    CCAx12Adc::instance().setLedOff(5);
    CCAx12Adc::instance().setLedOff(6);
    CCAx12Adc::instance().setLedOff(7);
    CCAx12Adc::instance().setLedOff(8);
    CCAx12Adc::instance().setLedOff(9);
    sleep(1);
    CCAx12Adc::instance().setLedOn(1);
    CCAx12Adc::instance().setLedOn(2);
    CCAx12Adc::instance().setLedOn(3);
    CCAx12Adc::instance().setLedOn(4);
    CCAx12Adc::instance().setLedOn(5);
    CCAx12Adc::instance().setLedOn(6);
    CCAx12Adc::instance().setLedOn(7);
    CCAx12Adc::instance().setLedOn(8);
    CCAx12Adc::instance().setLedOn(9);
    */
//
//    while (1) {
//        int adc1 = 0;
//        adc1 = CCAx12Adc::instance().getADC(1);
//        logger().info() << "adc1=" << adc1 << logs::end;
//        usleep(200000);
//
//        CCAx12Adc::instance().setLedOn(9);
//        //int r = CCAx12Adc::instance().writeAXData(8, P_TORQUE_ENABLE, 0);
//        //int r = CCAx12Adc::instance().readAXData(8, CMD_PING_AX);
//        //r = CCAx12Adc::instance().readAXData(8, P_PRESENT_POSITION);
//        //logger().info() << "r=" << r << logs::end;
//        //int r = CCAx12Adc::instance().writeAXData(8, P_GOAL_SPEED, 600);
//        usleep(500000);
//        CCAx12Adc::instance().setLedOff(9);
////        r = CCAx12Adc::instance().writeAXData(8, P_GOAL_SPEED, 800);
//    }



//    //____________________________________TEST
//    int servo = 0;
//    setMinPulse(servo, 0);
//    setMaxPulse(servo, 3000);
//    setMinPulse(7, 0);
//    setMaxPulse(7, 3000);
//
//     while (1) {
//     std::cout << "800" << std::endl;
//     setRate(servo, 0);
//     setPulsePos(servo, 1000);
//     setRate(7, 0);
//     setPulsePos(7, 1000);
//     usleep(1000000);
//
//     std::cout << "2000" << std::endl;
//     setRate(servo, 0);
//     setPulsePos(servo, 2000);
//
//     setRate(7, 0);
//     setPulsePos(7, 2000);
//     usleep(1000000);
//     }
//     //____________________________________TEST

    /*
     while (1) {

     setRate(servo, 0);
     setPulsePosWithRate(servo, 500);
     setRate(7, 0);
     setPulsePosWithRate(7, 500);
     //usleep(800000);

     setRate(servo, 2000);
     setPulsePosWithRate(servo, 2500);
     setRate(7, 2000);
     setPulsePosWithRate(7, 2500);
     //usleep(800000);
     }
     */
}

void ServoDriver::setPulsePosWithRate(int servo, int pos_microsec) { //TODO add the rate here

    servo = constrain(servo, 0, NB_SERVO_STD - 1);
    //version 3
    utils::Chronometer chrono("ServoDriver::setPositionWithRate");
    int order_pos_microsec_ = constrain(pos_microsec, servo_min_[servo], servo_max_[servo]);
    int current_pos_ = servo_current_usec_[servo];
    int rate_ms_per_1000 = servo_rate_[servo];
    if (rate_ms_per_1000 == 0) {
        setPulsePos(servo, pos_microsec);
    }
    int diff_pos = order_pos_microsec_ - current_pos_;
    //calcul du temps de déplacement souhaité pour diff
    int timing_of_move_ms = rate_ms_per_1000 * diff_pos / 1000.0;

    std::cout << "order_pos_microsec_= "
            << order_pos_microsec_
            << " current_pos_= "
            << current_pos_
            << " rate_ms_per_1000= "
            << rate_ms_per_1000
            << " diff_pos= "
            << diff_pos
            << " timing_of_move_ms= "
            << timing_of_move_ms
            << std::endl;

    if (diff_pos > 0) {
        chrono.start();
        long t = chrono.getElapsedTimeInMilliSec();
        //std::cout << "init t= "<< t << std::endl;
        long tf = t;
        while (t <= timing_of_move_ms) {
            t = chrono.getElapsedTimeInMilliSec();
            if ((t - tf) > 10) {
                //calcul de la position pour t
                int cur_pos = t * 1000.0 / rate_ms_per_1000;
                std::cout << " t= " << std::dec << t << " tf= " << std::dec << tf << " cur_pos=" << std::dec << cur_pos << std::endl;
                setPulsePos(servo, current_pos_ + cur_pos);
                tf = t;
            }
            else std::this_thread::sleep_for(std::chrono::milliseconds(2));
        }
    }

    /*
     //version 2
     utils::Chronometer chrono("ServoDriver::setPositionWithRate");
     int order_pos_microsec_ = constrain(pos_microsec, servo_min_[servo], servo_max_[servo]);
     int current_pos_ = servo_current_usec_[servo];
     int rate_ms_per_1000 = servo_rate_[servo];
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
     int microsec_ = constrain(microsec, servo_min_[servo], servo_max_[servo]);
     int current_pos_ = servo_current_usec_[servo];
     int rate_ = servo_rate_[servo];
     if (rate_ == 0)
     {
     setPosition(servo, microsec);
     }
     int diff = current_pos_ - microsec_;
     //25usec pour laisser au moins 7ms le temps d'une requete i2c
     if (diff < 0)
     for (int pos = current_pos_; pos < microsec_; pos += 25) {
     setPosition(servo, pos);
     int waitms = rate_ / 1000.0 * 24;

     usleep(waitms*1000.0);
     }
     */

}

void ServoDriver::setPulsePos(int servo, int pulsewidth_us) {
    servo = constrain(servo, 0, NB_SERVO_STD - 1);
    pulsewidth_us = constrain(pulsewidth_us, servo_min_[servo], servo_max_[servo]);
    servo_current_usec_[servo] = pulsewidth_us;
    if (servo_type_[servo] == AServoDriver::SERVO_STANDARD) {
        pwm_.fastWriteMicroseconds(servo, pulsewidth_us);
    }
    else if (servo_type_[servo] == AServoDriver::SERVO_DYNAMIXEL) {
        //TODO

    }

    //telemetry log
//    nlohmann::json j;
//    j["servo" + servo] = pulsewidth_us;
//    logger().telemetry(j.dump());
}

void ServoDriver::release(int servo) {
    servo = constrain(servo, 0, NB_SERVO_STD - 1);
    if (servo_type_[servo] == AServoDriver::SERVO_STANDARD) {
        pwm_.fastWriteMicroseconds(servo, 0);
    }
    else if (servo_type_[servo] == AServoDriver::SERVO_DYNAMIXEL) {
        //TODO
    }
}

void ServoDriver::hold(int servo) // 0 à 7
{
    servo = constrain(servo, 0, NB_SERVO_STD - 1);
    if (servo_type_[servo] == AServoDriver::SERVO_STANDARD) {
        pwm_.fastWriteMicroseconds(servo, servo_current_usec_[servo]);
    }
    else if (servo_type_[servo] == AServoDriver::SERVO_DYNAMIXEL) {
        //TODO
    }

}

void ServoDriver::setRate(int servo, int millisec0To90) {

    // Sets the rate_sp at which the servo travels from 0 to 100.0% (half of the full
    // range of the servo). Units are in milliseconds. Example: Setting the rate_sp
    // to 1000 means that it will take a 180 degree servo 2 second to move from 0
    // to 180 degrees. Note: Some servo controllers may not support this in which
    // case reading and writing will fail with `-EOPNOTSUPP`.

    //speed servo SG5010 : 0.11 sec/60° => 0.00183 sec/1° => 0.33sec/180° 330ms/180°
    //Speed servo 225BB : 0.14 (4.8V) x 0.11(6V) sec @ 60 deg

    servo_rate_[constrain(servo, 0, NB_SERVO_STD - 1)] = constrain(millisec0To90, 0, 10000);

}

void ServoDriver::turn(int servo, int speed) {
    logger().error() << "turn() NOT IMPLEMENTED" << logs::end;
    return;
}

int ServoDriver::getMoving(int servo) {
    logger().error() << "getMoving() NOT IMPLEMENTED" << logs::end;
    return -1;
}

int ServoDriver::getPulsePos(int servo) {
    servo = constrain(servo, 0, NB_SERVO_STD - 1);

    if (servo_type_[servo] == AServoDriver::SERVO_STANDARD) {
        //return servo_current_usec_[servo];

        int pwm = pwm_.getPWM(servo, true);
        return pwm;

    }
    else if (servo_type_[servo] == SERVO_DYNAMIXEL) {

        int r = CCAx12Adc::instance().readAXData(servo, P_PRESENT_POSITION);
        return r;
    }
    else return -1;
}

int ServoDriver::getTorque(int servo) {
    logger().error() << "getTorque() NOT IMPLEMENTED" << logs::end;
    return -1;
}

int ServoDriver::ping(int servo) {
    logger().error() << "ping() NOT IMPLEMENTED" << logs::end;
    return 0;
}

void ServoDriver::setMinPulse(int servo, int pulse) {
    servo_min_[constrain(servo, 0, NB_SERVO_STD - 1)] = constrain(pulse, 0, 1000);    //default 500 [0 to 1000]
}

void ServoDriver::setMidPulse(int servo, int pulse) {
    servo_mid_[constrain(servo, 0, NB_SERVO_STD - 1)] = constrain(pulse, 1000, 2000); //default 1500 [1000 to 2000]
}

void ServoDriver::setMaxPulse(int servo, int pulse) {
    servo_max_[constrain(servo, 0, NB_SERVO_STD - 1)] = constrain(pulse, 2000, 3000); //default 2500 [2000 to 3000]
}

void ServoDriver::setPolarity(int servo, bool inverted) {
    servo_inv_[constrain(servo, 0, NB_SERVO_STD - 1)] = inverted; //TODO Faire les calculs en inversé sur la position par rapport au milieu
}

void ServoDriver::setType(int servo, ServoType type) {
    servo_type_[constrain(servo, 0, NB_SERVO_STD - 1)] = type;
}

