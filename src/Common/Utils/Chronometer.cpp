/*!
 * \file
 * \brief Implémentation de la classe Chronometer.
 */

#include "Chronometer.hpp"

#include <chrono>
#include <thread>

#include "../../Log/Logger.hpp"
#include "../../Thread/Thread.hpp"

utils::Chronometer::Chronometer(std::string name) :
        stopped_(1), timerPeriod_us_(0), name_(name), periodNb_(0), endSetTime_us(0), timerStartTime_us_(0), lastTime_(0)
{
    startCount_.tv_sec = 0;
    startCount_.tv_usec = 0;
    endCount_.tv_sec = 0;
    endCount_.tv_usec = 0;

    //TODO deprecated
    endSet_.tv_sec = 0;
    endSet_.tv_usec = 0;
}

void utils::Chronometer::start() {
    stopped_ = 0; // reset stop flag
    gettimeofday(&startCount_, NULL);
    lastTime_ = getElapsedTimeInMicroSec();

}

void utils::Chronometer::stop() {
    stopped_ = 1; // set chronometer stopped_ flag
    gettimeofday(&endCount_, NULL);
}

unsigned long long utils::Chronometer::getElapsedTimeInMicroSec() {
    if (!stopped_) {
        gettimeofday(&endCount_, NULL);
    }

    long seconds = endCount_.tv_sec - startCount_.tv_sec;
    return seconds * 1000000 + (endCount_.tv_usec - startCount_.tv_usec);
}

float utils::Chronometer::getElapsedTimeInMilliSec() {
    return this->getElapsedTimeInMicroSec() * 0.001;
}

float utils::Chronometer::getElapsedTimeInSec() {
    return this->getElapsedTimeInMicroSec() * 0.000001;
}

float utils::Chronometer::getElapsedTime() {
    return this->getElapsedTimeInSec();
}

timeval utils::Chronometer::getTime() {
    timeval result;
    gettimeofday(&result, NULL);
    return result;
}

int utils::Chronometer::waitTimer(int delay_us, bool debug) {
    periodNb_++;
    unsigned long long endTaskTime = getElapsedTimeInMicroSec();
    //int workTime = endTaskTime - lastTime_;
    int t = timerPeriod_us_ - (int)(endTaskTime - lastTime_);
    //printf("endTaskTime=%lld , lastime=%lld , workTime=%lld, period_us=%lld\n", endTaskTime, lastTime_, workTime, timerPeriod_us_);
//    if (debug)
//        logger().info() << "waitTimer() name=" << name_ << " endTaskTime=" << endTaskTime << " (diff)workTime="
//                << workTime << " " << timerPeriod_us_ << "!!" << logs::end;

    //if (workTime < timerPeriod_us_) {
    if (t >= 0) {
        std::this_thread::sleep_for(std::chrono::microseconds(t-delay_us));
    }
    else {
        //logger().error() << "!!! OVERFLOW overTime=" << t << " ; " << timerPeriod_us_ << "!!!" << logs::end;
        logger().error() << "OVERFLOW " << name_ << " "<< (t-delay_us) / 1000<< logs::end;
    }

// fin
    lastTime_ = getElapsedTimeInMicroSec();

    return periodNb_;
}

/*
 int utils::Chronometer::checkTimer(unsigned int usec)
 {
 struct timeval ctv;
 gettimeofday(&ctv, NULL);

 if ((ctv.tv_usec >= endSet_.tv_usec) && (ctv.tv_sec >= endSet_.tv_sec)) {
 if (usec != 0)
 setTimer(timerPeriod_us_);
 else
 this->stop();

 return 1;
 } else
 return 0;
 }*/

/*
 int utils::Chronometer::waitTimer_OLD()
 {
 periodNb_++;

 long long nextTime = timerStartTime_us_ + (timerPeriod_us_ * periodNb_);
 long long endTaskTime = getElapsedTimeInMicroSec();
 if (nextTime >= endTaskTime) {
 long long t = nextTime - endTaskTime;

 usleep(t);

 } else {
 long long diff = (endTaskTime - nextTime) / timerPeriod_us_;
 periodNb_ += diff;
 //printf("\nutils::Chronometer::waitTimer() OVERFLOW periodNb=%lld diff=%lld timerPeriod_us_=%dc!!\n", periodNb_, diff, timerPeriod_us_);
 logger().error() << "utils::Chronometer::waitTimer() OVERFLOW !!! periodNb=" << periodNb_
 << " (endTaskTime - nextTime)us=" << (endTaskTime - nextTime) << " timerPeriod_us_=" << timerPeriod_us_
 << "!!\n" << logs::end;

 }

 return periodNb_;
 }*/

void utils::Chronometer::setTimer(unsigned int usec) {
    timerPeriod_us_ = usec;
    this->start();
    timerStartTime_us_ = 0;
}
