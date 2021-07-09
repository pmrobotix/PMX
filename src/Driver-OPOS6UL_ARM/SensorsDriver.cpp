//drivers...OPO

#include "SensorsDriver.hpp"

#include <list>
#include <string>

#include "../Log/Logger.hpp"

using namespace std;

ASensorsDriver * ASensorsDriver::create(std::string) {
    return new SensorsDriver();
}

SensorsDriver::SensorsDriver() :
        beaconSensors_(0, ADDRESS_BeaconSensors), gp2_1_(0, ADDRESS_gp2y0e02b), gp2_2_(1, ADDRESS_gp2y0e02b), connected_gp2y0e02b_(false)
{
    regs_ = {};
    settings_ = {};
    vadv_.clear();
}

SensorsDriver::~SensorsDriver() {
}

ASensorsDriver::bot_positions SensorsDriver::getvPositionsAdv() {
    //msync_.lock();
    //bot_positions tmp = vadv_;
    //msync_.unlock();
    return vadv_;
}
int SensorsDriver::sync() {
    //logger().debug() << "beaconSensors_.getData()"<< logs::end;
    msync_.lock();
    regs_ = beaconSensors_.getData();
    if (regs_.flags == 0xFF) {
        printf("ERROR regs_.flags == 0xFF!!");
        return -1;
    }

    vadv_.clear();

    if (vadv_.empty()) {
        if (regs_.nbDetectedBots >= 1) {
            vadv_.push_back(RobotPos(regs_.x1_mm, regs_.y1_mm, regs_.a1_deg, regs_.d1_mm, (int) regs_.nbDetectedBots));
        }
        if (regs_.nbDetectedBots >= 2) {
            vadv_.push_back(RobotPos(regs_.x2_mm, regs_.y2_mm, regs_.a2_deg, regs_.d2_mm, (int) regs_.nbDetectedBots));
        }
        if (regs_.nbDetectedBots >= 3) {
            vadv_.push_back(RobotPos(regs_.x3_mm, regs_.y3_mm, regs_.a3_deg, regs_.d3_mm, (int) regs_.nbDetectedBots));
        }
        if (regs_.nbDetectedBots >= 4) {
            vadv_.push_back(RobotPos(regs_.x4_mm, regs_.y4_mm, regs_.a4_deg, regs_.d4_mm, (int) regs_.nbDetectedBots));
        }
    }

    msync_.unlock();

    return 0;
}

int SensorsDriver::leftSide() {
    int dist_from_center_mm = 90;
    int d = gp2_2_.getDistanceMm() + dist_from_center_mm; //TODO  true if only 90° !!!!
    return d;
}
int SensorsDriver::rightSide() {
    int dist_from_center_mm = 103;
    int d = gp2_1_.getDistanceMm() + dist_from_center_mm; //TODO  true if only 90° !!!!
    return d;
}
//
//int SensorsDriver::getFrontDistMmFromObject(int diagonal_dist_mm) //TODO position xy ?
//{
//    int dist_from_wheel_axe_mm = 155;
//    float cos_alpha = 45.0/48.0;
//    int lmm = diagonal_dist_mm * cos_alpha + dist_from_wheel_axe_mm;
//    return lmm;
//}
//
//int SensorsDriver::getBackDistMmFromObject(int diagonal_dist_mm)
//{
//    int dist_from_wheel_axe_mm = 70;
//    float cos_alpha = 42.0/48.0;
//    int lmm = diagonal_dist_mm * cos_alpha + dist_from_wheel_axe_mm;
//    return lmm;
//}

//retourne la distance minimum du dernier sync
int SensorsDriver::frontLeft() {
    //TODO filtrage minimum a faire ici aussi?

    //int res = -1;
    //msync_.lock();
    //if (regs_.c1_mm == 0) res = -1;
    //if (regs_.c2_mm == 0) res = -1;

    if (regs_.c2_mm == 0) return 500;

    //seuil minimum
    if (regs_.c2_mm > 500) //regs_.c1_mm > 30 &&
            {
//        //int dist = 0;
//        if (regs_.c1_mm < regs_.c2_mm) res = regs_.c1_mm;
//        if (regs_.c1_mm >= regs_.c2_mm) res = regs_.c2_mm;
//    }else
//    {
        //msync_.unlock();
        return 500;
    }

    //msync_.unlock();
    return regs_.c2_mm;
}

int SensorsDriver::frontCenter() {
    //int dist_from_wheel_axe_mm = 140;
    //return irCenter_.getDistanceMm() + dist_from_wheel_axe_mm;
    return -1;
}
int SensorsDriver::frontRight() {
//TODO filtrage minimum a faire ici aussi?

//    int res = -1;
//    msync_.lock();
//    if (regs_.c3_mm == 0) res = -1;
//    if (regs_.c4_mm == 0) res = -1;
//    //int dist = 0;
//    if (regs_.c3_mm < regs_.c4_mm) res = regs_.c3_mm;
//    if (regs_.c3_mm >= regs_.c4_mm) res = regs_.c4_mm;
//    msync_.unlock();
//    return res;
//    int res = -1;
//    msync_.lock();

    if (regs_.c4_mm == 0) return 500;

    //seuil minimum
    if (regs_.c4_mm > 500) {

        return 500;
    }

    //msync_.unlock();
    return regs_.c4_mm;
}

int SensorsDriver::backLeft() {
    return -1;
}
int SensorsDriver::backCenter() {
    return -1;
}
int SensorsDriver::backRight() {
    return -1;
}
