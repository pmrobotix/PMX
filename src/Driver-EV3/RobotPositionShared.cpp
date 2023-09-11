//drivers...EV3

#include "RobotPositionShared.hpp"

#include <stdio.h>

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
    //tabP_[20] = {0};
    //v_ = {0};
}

RobotPositionShared::~RobotPositionShared()
{
}

void RobotPositionShared::setRobotPosition(ROBOTPOSITION p)
{
    unsigned int max = 40;
    this->lock();
    //p_ = p;

    if (v_.size() > max) //le max a garder
        v_.pop_back();
    v_.insert(v_.begin(), p);
    //t_set_us_ = chrono_.getElapsedTimeInMicroSec();
    if (vtime_.size() > max)
        vtime_.pop_back();
    vtime_.insert(vtime_.begin(), chrono_.getElapsedTimeInMicroSec());
    this->unlock();

    //printf("__setRobotPosition  p.x=%f p.y=%f \n", p_.x, p_.y);
}

ROBOTPOSITION RobotPositionShared::getRobotPosition(int debug)
{
    ROBOTPOSITION p;

    unsigned long long t_get_us;
    unsigned long long t_set_us_temp;

    //pour l'EV3
    //10 => 160ms
    //14 => 220ms
    //15 => 240ms
    //16 => 250ms
    //18 => 280ms
    //19 => 300ms
    //20 => 320ms
    //25 => 400ms
    unsigned int index = 15;

    this->lock();
    //p = p_;

    if (v_.size() > index)
        p = v_.at(index);
    else
        p = v_.back();

    t_get_us = chrono_.getElapsedTimeInMicroSec();

    if (vtime_.size() > index)
        t_set_us_temp = vtime_.at(index);
    else
        t_set_us_temp = vtime_.back();

    this->unlock();
//    if (debug == 1)
//        printf("--getRobotPosition  p.x=%f p.y=%f diffT=%ld \n", p.x, p.y, (long) (t_get_us - t_set_us_temp));

    return p;
}

