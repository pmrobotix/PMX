//drivers...EV3

#include "SensorsDriver.hpp"

#include <unistd.h>
#include <string>

#include "../Log/Logger.hpp"

using namespace std;

ASensorsDriver * ASensorsDriver::create(std::string botName, ARobotPositionShared *robotPos) {
    return new SensorsDriver(robotPos);
}

//ev3-ports:inX:i2c80:mux1
//ev3-ports:inX:i2c81:mux2
//ev3-ports:inX:i2c82:mux3
SensorsDriver::SensorsDriver(ARobotPositionShared *robotPos) :
        beaconSensors_(3, ADDRESS_BeaconSensors)
//        IR_1_AV_D("ev3-ports:in4:i2c80:mux1"), IR_2_AV_G("ev3-ports:in3:i2c80:mux1"), IR_3_AR_D(
//                "ev3-ports:in4:i2c81:mux2"), IR_4_AR_G("ev3-ports:in3:i2c82:mux3")
{

    connected_ = beaconSensors_.connect(); //TODO rename begin
//    logger().debug() << "SensorsDriver()... " << logs::end;
//    beacon_.ScanBus(); //NE FCT PAS ?

    beaconSensors_.display(0);

    robotPositionShared_ = robotPos;
}

SensorsDriver::~SensorsDriver() {
}

bool SensorsDriver::is_connected()
{
    if (connected_)
        return is_alive();
    else
        return false;
}

bool SensorsDriver::is_alive()
{
    return beaconSensors_.is_alive();
}

ASensorsDriver::bot_positions SensorsDriver::getvPositionsAdv() {
    return vadv_;
}
int SensorsDriver::sync() {
    //logger().debug() << "beaconSensors_.getData()"<< logs::end;
    msync_.lock();
    for (int t = 0; t <= 2; t++) {
        regs_ = beaconSensors_.getData();
        if (regs_.flags == 0xFF) {
            logger().error() << "sync()...try again n° " << t << logs::end;
            if (t >= 2) {
                logger().error() << "sync()...try 3 times and regs_.flags == 0xFF!!"  << logs::end;
                //printf("ERROR try 3 times and regs_.flags == 0xFF!!\n");
                return -1;
            }
        }
        else break;
    }

    vadv_.clear();

    if (vadv_.empty()) {
        if (regs_.nbDetectedBots >= 1) {
            vadv_.push_back(RobotPos((int) regs_.nbDetectedBots, regs_.x1_mm * 1.0, regs_.y1_mm* 1.0, regs_.a1_deg, regs_.d1_mm* 1.0));
        }
        if (regs_.nbDetectedBots >= 2) {
            vadv_.push_back(RobotPos((int) regs_.nbDetectedBots, regs_.x2_mm* 1.0, regs_.y2_mm* 1.0, regs_.a2_deg, regs_.d2_mm* 1.0));
        }
        if (regs_.nbDetectedBots >= 3) {
            vadv_.push_back(RobotPos((int) regs_.nbDetectedBots, regs_.x3_mm* 1.0, regs_.y3_mm* 1.0, regs_.a3_deg, regs_.d3_mm* 1.0));
        }
        if (regs_.nbDetectedBots >= 4) {
            vadv_.push_back(RobotPos((int) regs_.nbDetectedBots, regs_.x4_mm* 1.0, regs_.y4_mm* 1.0, regs_.a4_deg, regs_.d4_mm* 1.0));
        }
    }

    msync_.unlock();

    return 0;
}

int SensorsDriver::rightSide() {
    return -1;
}
int SensorsDriver::leftSide() {
    return -1;
}

int SensorsDriver::frontLeft() {
//    float mm1 = IR_2_AV_G.getDistanceMM();
//    //real distance on the ground
//    mm1 = (mm1 * 0.75) + 155; // cos a = 400/500 = 0.8
//    return mm1;

    if (regs_.c2_mm == 0) return 500;
    if (regs_.c2_mm > 500) //regs_.c1_mm > 30 &&
            {
        return 500;
    }
    return regs_.c2_mm;
}
int SensorsDriver::frontCenter() {
    return -1;
}
int SensorsDriver::frontRight() {
//    float mm1 = IR_1_AV_D.getDistanceMM();
//    //real distance on the ground
//    mm1 = (mm1 * 0.75) + 145; // cos a = 400/500 = 0.8
//    return mm1;

    if (regs_.c4_mm == 0) return 500;
    if (regs_.c4_mm > 500) {
        return 500;
    }
    return regs_.c4_mm;
}

int SensorsDriver::backLeft() {
//    float mm1 = IR_4_AR_G.getDistanceMM();
//    //real distance on the ground
//    mm1 = (mm1 * 0.85) + 45; // cos a = 400/500 = 0.8
//    return mm1;
    if (regs_.c6_mm == 0) return 500;
    if (regs_.c6_mm > 500) //regs_.c1_mm > 30 &&
            {
        return 500;
    }
    return regs_.c6_mm;

}
int SensorsDriver::backCenter() {
    return -1;
}
int SensorsDriver::backRight() {
//    float mm1 = IR_3_AR_D.getDistanceMM();
//    //real distance on the ground
//    mm1 = (mm1 * 0.75) + 45; // cos a = 400/500 = 0.8
//    return mm1;

    if (regs_.c8_mm == 0) return 500;
    if (regs_.c8_mm > 500) {
        return 500;
    }
    return regs_.c8_mm;
}

void SensorsDriver::displayNumber(int number)
{
    beaconSensors_.display(number);
}


int SensorsDriver::getAnalogPinData()
{
    return regs_.reserved_analog;
}

