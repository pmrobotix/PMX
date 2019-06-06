#include "Sensors.hpp"

#include <bits/stl_algo.h>
#include <unistd.h>
#include <sstream>

#include "../Action.Driver/ASensorsDriver.hpp"
#include "../Arguments.hpp"
#include "../Asserv/Asserv.hpp"
#include "../Asserv/MotorControl.hpp"
#include "../Asserv/MovingBase.hpp"
#include "../Robot.hpp"
#include "Actions.hpp"

using namespace std;

Sensors::Sensors(Actions & actions, Robot * robot) :
        AActionsElement(actions), robot_(robot)

{
    sensorsdriver_ = ASensorsDriver::create(robot->getID());

    addThresholdFront(0, 0, 0);
    addThresholdFrontVeryClosed(0, 0, 0);
    addThresholdBack(0, 0, 0);
    addThresholdBackVeryClosed(0, 0, 0);
    addConfigFront(false, false, false);
    addConfigBack(false, false, false);

    setIgnoreAllFrontNearObstacle(false);
    setIgnoreAllBackNearObstacle(false);

}
Sensors::~Sensors()
{
    delete sensorsdriver_;
}

SensorsTimer::SensorsTimer(Sensors & sensors, int timeSpan_ms, std::string name) :
        sensors_(sensors), chrono_("SensorsTimerChrono")
{
    nameListener_ = name;
    lasttime_ = 0;
    timeSpan_ms_ = timeSpan_ms;

    lastdetect_front_nb_ = 0;
    lastdetect_back_nb_ = 0;
}

void Sensors::addThresholdFront(int left, int center, int right)
{
    frontLeftThreshold_ = left;
    frontCenterThreshold_ = center;
    frontRightThreshold_ = right;
}
void Sensors::addThresholdFrontVeryClosed(int left, int center, int right)
{
    frontLeftVeryClosedThreshold_ = left;
    frontCenterVeryClosedThreshold_ = center;
    frontRightVeryClosedThreshold_ = right;
}
void Sensors::addThresholdBack(int left, int center, int right)
{
    backLeftThreshold_ = left;
    backCenterThreshold_ = center;
    backRightThreshold_ = right;
}
void Sensors::addThresholdBackVeryClosed(int left, int center, int right)
{
    backLeftVeryClosedThreshold_ = left;
    backCenterVeryClosedThreshold_ = center;
    backRightVeryClosedThreshold_ = right;
}

void Sensors::addConfigFront(bool left, bool center, bool right)
{
    enableFrontLeft_ = left;
    enableFrontCenter_ = center;
    enableFrontRight_ = right;
}
void Sensors::addConfigBack(bool left, bool center, bool right)
{
    enableBackLeft_ = left;
    enableBackCenter_ = center;
    enableBackRight_ = right;
}

void Sensors::setIgnoreFrontNearObstacle(bool ignoreLeft, bool ignoreCenter, bool ignoreRight)
{
    ignoreFrontLeft_ = ignoreLeft;
    ignoreFrontCenter_ = ignoreCenter;
    ignoreFrontRight_ = ignoreRight;
}
void Sensors::setIgnoreBackNearObstacle(bool ignoreLeft, bool ignoreCenter, bool ignoreRight)
{
    ignoreBackLeft_ = ignoreLeft;
    ignoreBackCenter_ = ignoreCenter;
    ignoreBackRight_ = ignoreRight;
}
void Sensors::setIgnoreAllFrontNearObstacle(bool ignore)
{
    ignoreFrontLeft_ = ignore;
    ignoreFrontCenter_ = ignore;
    ignoreFrontRight_ = ignore;
}
void Sensors::setIgnoreAllBackNearObstacle(bool ignore)
{
    ignoreBackLeft_ = ignore;
    ignoreBackCenter_ = ignore;
    ignoreBackRight_ = ignore;
}

int Sensors::sensorDist(std::string sensorname)
{
    if (sensorname == "fL") {
        return sensorsdriver_->frontLeft();
    }
    if (sensorname == "fC") {
        return sensorsdriver_->frontCenter();
    }
    if (sensorname == "fR") {
        return sensorsdriver_->frontRight();
    }
    if (sensorname == "bL") {
        return sensorsdriver_->backLeft();
    }
    if (sensorname == "bC") {
        return sensorsdriver_->backCenter();
    }
    if (sensorname == "bR") {
        return sensorsdriver_->backRight();
    }
    return -1;
}

float Sensors::multipleRightSide(int nb)
{
    int max = 0;
    int min = 0;
    int value = 0;

    int data[nb];
    int nb_moy = 0;
    float moy = 0.0;
    for (int ii = 0; ii < nb; ii++) {
        data[ii] = rightSide();
        logger().info() << "Right= " << data[ii] << logs::end;
        usleep(50000);
    }

    //Now we call the sort function
    std::sort(data, data + nb);

    for (int ii = 0; ii < nb; ii++) {
        logger().info() << "rtrie= " << data[ii] << logs::end;
        //usleep(50000);
    }

    moy = (data[3] + data[4] + data[5] + data[6]) / 4;

    return moy;
}

float Sensors::multipleLeftSide(int nb)
{
    int max = 0;
    int min = 9999999;
    int value = 0;

    int data[nb];
    int nb_moy = 0;
    float moy = 0.0;
    for (int ii = 0; ii < nb; ii++) {
        data[ii] = leftSide();
        logger().info() << "Left= " << data[ii] << logs::end;
        usleep(50000);
    }

    //Now we call the sort function
    std::sort(data, data + nb);

    for (int ii = 0; ii < nb; ii++) {
        logger().info() << "ltrie= " << data[ii] << logs::end;
        //usleep(50000);
    }

    moy = (data[3] + data[4] + data[5] + data[6]) / 4;

    return moy;
}

int Sensors::rightSide()
{
    return sensorsdriver_->rightSide();
}
int Sensors::leftSide()
{
    return sensorsdriver_->leftSide();
}

//retourne 0, sinon le niveau detecté 2 veryClosed, 1 first level
int Sensors::front(bool display)
{
    int fL = sensorDist("fL");
    int fC = sensorDist("fC");
    int fR = sensorDist("fR");

    //logger().info() << " L " << enableFrontLeft_ << " C " << enableFrontCenter_ << " R " << enableFrontRight_ << logs::end;
    int level = 0;
    if (enableFrontLeft_)
        if (this->robot()->asserv()->filtre_IsInsideTable(fL, -1)) { //negatif = capteur placé à gauche
            if ((!ignoreFrontLeft_ && (fL < frontLeftThreshold_))) {
                if (display)
                    logger().info() << "1 frontLeft= " << fL << logs::end;
                level = 1;
            }
        }
    if (enableFrontCenter_)
        if (this->robot()->asserv()->filtre_IsInsideTable(fC, 0)) {
            if ((!ignoreFrontCenter_ && (fC < frontCenterThreshold_))) {
                if (display)
                    logger().info() << "1 frontCenter= " << fC << logs::end;
                level = 1;
            }
        }
    if (enableFrontRight_)
        if (this->robot()->asserv()->filtre_IsInsideTable(fR, 1)) {
            if ((!ignoreFrontRight_ && (fR < frontRightThreshold_))) {
                if (display)
                    logger().info() << "1 frontRight= " << fR << logs::end;
                level = 1;
            }
        }
    if (enableFrontLeft_)
        if (this->robot()->asserv()->filtre_IsInsideTable(fL, -1)) { //negatif = capteur placé à gauche
            if ((!ignoreFrontLeft_ && (fL < frontLeftVeryClosedThreshold_))) {
                if (display)
                    logger().info() << "2 frontLeft= " << fL << logs::end;
                level = 2;
            }
        }
    if (enableFrontCenter_)
        if (this->robot()->asserv()->filtre_IsInsideTable(fC, 0)) {
            if ((!ignoreFrontCenter_ && (fC < frontCenterVeryClosedThreshold_))) {
                if (display)
                    logger().info() << "2 frontCenter= " << fC << logs::end;
                level = 2;
            }
        }
    if (enableFrontRight_)
        if (this->robot()->asserv()->filtre_IsInsideTable(fR, +1)) {
            if ((!ignoreFrontRight_ && (fR < frontRightVeryClosedThreshold_))) {
                if (display)
                    logger().info() << "2 frontRight= " << fR << logs::end;
                level = 2;
            }
        }

    return level;
}

int Sensors::back(bool display)
{
    int bL = sensorDist("bL");
    int bC = sensorDist("bC");
    int bR = sensorDist("bR");
    int level = 0;
    if (enableBackLeft_)
        if (this->robot()->asserv()->filtre_IsInsideTable(-bL, -1)) { //negatif = capteur placé à gauche
            if ((!ignoreBackLeft_ && (bL < backLeftThreshold_))) {
                logger().info() << "1 backLeft= " << bL << logs::end;
                level = 1;
            }
        }
    if (enableBackCenter_)
        if (this->robot()->asserv()->filtre_IsInsideTable(-bC, 0)) {
            if ((!ignoreBackCenter_ && (bC < backCenterThreshold_))) {
                logger().info() << "1 backCenter= " << bC << logs::end;
                level = 1;
            }
        }
    if (enableBackRight_)
        if (this->robot()->asserv()->filtre_IsInsideTable(-bR, 1)) {
            if ((!ignoreBackRight_ && (bR < backRightThreshold_))) {
                logger().info() << "1 backRight= " << bR << logs::end;
                level = 1;
            }
        }
    if (enableBackLeft_)
        if (this->robot()->asserv()->filtre_IsInsideTable(-bL, -1)) {
            if ((!ignoreBackLeft_ && (bL < backLeftVeryClosedThreshold_))) {
                logger().info() << "2 backLeft= " << bL << logs::end;
                level = 2;
            }
        }
    if (enableBackCenter_)
        if (this->robot()->asserv()->filtre_IsInsideTable(-bC, 0)) {
            if ((!ignoreBackCenter_ && (bC < backCenterVeryClosedThreshold_))) {
                logger().info() << "2 backCenter= " << bC << logs::end;
                level = 2;
            }
        }
    if (enableBackRight_)
        if (this->robot()->asserv()->filtre_IsInsideTable(-bR, 1)) {
            if ((!ignoreBackRight_ && (bR < backRightVeryClosedThreshold_))) {
                logger().info() << "2 backRight= " << bR << logs::end;
                level = 2;
            }
        }

    return level;
}

void Sensors::addTimerSensors(int timeSpan_ms)
{
    logger().debug() << "startSensors" << logs::end;

    this->actions().addTimer(new SensorsTimer(*this, timeSpan_ms, "sensors"));

}
void Sensors::stopTimerSensors()
{
    this->actions().stopTimer("sensors");
}

void SensorsTimer::onTimer(utils::Chronometer chrono)
{
    int frontLevel = sensors_.front(false);
    if (frontLevel >= 1) {
        //send collision to asserv
        if (lastdetect_front_nb_ == 0) {
            sensors_.robot()->asserv()->base()->motors().stopMotors(); //pour etre plus reactif sur l'arret sinon on touche
            sensors_.robot()->asserv()->warnFrontCollisionOnTraj();
            sensors_.robot()->asserv()->base()->motors().stopMotors(); //pour etre plus reactif sur l'arret sinon on touche
            sensors_.robot()->asserv()->setLowSpeedForward(true);

        }
        lastdetect_front_nb_++;
    }

//cas nearest
    if (lastdetect_front_nb_ > 0) {
        if (frontLevel >= 2) {
            sensors_.robot()->asserv()->base()->motors().stopMotors(); //pour etre plus reactif sur l'arret sinon on touche
            sensors_.robot()->asserv()->warnFrontCollisionOnTraj();
            sensors_.robot()->asserv()->base()->motors().stopMotors(); //pour etre plus reactif sur l'arret sinon on touche
        }
        if (frontLevel == 0) {
            lastdetect_front_nb_ = 0;
            sensors_.robot()->asserv()->setLowSpeedForward(false); //surcharge par robot utilisée
            sensors_.robot()->asserv()->resetFrontCollisionOnTraj();
        }
    }

//ARRIERE/////////////////////////////////////////////////////////////////
    int backLevel = sensors_.back(false);
    if (backLevel >= 1) {
        //send collision to asserv

        if (lastdetect_back_nb_ == 0) {
            sensors_.robot()->asserv()->base()->motors().stopMotors(); //pour etre plus reactif sur l'arret sinon on touche
            sensors_.robot()->asserv()->warnBackCollisionOnTraj();
            sensors_.robot()->asserv()->base()->motors().stopMotors(); //pour etre plus reactif sur l'arret sinon on touche
            sensors_.robot()->asserv()->setLowSpeedBackward(true);

        }
        lastdetect_back_nb_++;
    }

    if (lastdetect_back_nb_ > 0) {
        if (backLevel >= 2) {
            sensors_.robot()->asserv()->base()->motors().stopMotors(); //pour etre plus reactif sur l'arret sinon on touche
            sensors_.robot()->asserv()->warnBackCollisionOnTraj();
            sensors_.robot()->asserv()->base()->motors().stopMotors(); //pour etre plus reactif sur l'arret sinon on touche
        }
        if (backLevel == 0) {
            lastdetect_back_nb_ = 0;
            sensors_.robot()->asserv()->setLowSpeedBackward(false); //surcharge par robot utilisée
            //TODO sensors_.robot()->asserv()->resetBackCollisionOnTraj();
        }
    }

//    logger().error() << "onTimer() " << this->info() << "=" << chrono.getElapsedTimeInMicroSec() << " us "
//            << lastdetect_front_nb_ << " front=" << frontLevel << " back=" << backLevel << logs::end;
}

void SensorsTimer::onTimerEnd(utils::Chronometer chrono)
{
//    logger().debug() << "onTimerEnd() " << this->info() << "=" << chrono.getElapsedTimeInMicroSec() << " us"
//            << logs::end;

}

std::string SensorsTimer::info()
{
    std::ostringstream oss;
    oss << "SensorsTimer [" << nameListener_ << "] for " << sensors_.robot()->getID();
    return oss.str();
}

