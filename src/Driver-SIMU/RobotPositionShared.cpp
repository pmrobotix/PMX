//drivers...SIMU

#include "RobotPositionShared.hpp"

#include <cstdio>

using namespace std;

ARobotPositionShared* ARobotPositionShared::create()
{
    //return new RobotPositionShared();

    static RobotPositionShared *instance = new RobotPositionShared();
    return instance;
}

RobotPositionShared::RobotPositionShared()
{
    t_set_us_ = 0;
    chrono_.start();
}

RobotPositionShared::~RobotPositionShared()
{
}

void RobotPositionShared::setRobotPosition(ROBOTPOSITION p)
{
    this->lock();
    p_ = p;
    t_set_us_ = chrono_.getElapsedTimeInMicroSec();
    this->unlock();

    //printf("__setRobotPosition  p.x=%f p.y=%f \n", p_.x, p_.y);
}

ROBOTPOSITION RobotPositionShared::getRobotPosition(int debug)
{
    ROBOTPOSITION p;
    unsigned long long t_get_us;
    this->lock();
    p = p_;
    t_get_us = chrono_.getElapsedTimeInMicroSec();
    this->unlock();
//    if (debug == 1)
//        printf("--getRobotPosition  p.x=%f p.y=%f diffT=%ld \n", p.x, p.y, (long) (t_get_us - t_set_us_));

    return p;
}

