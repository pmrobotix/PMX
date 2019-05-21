#include "Sensors.hpp"

#include <sstream>

#include "../Action.Driver/ASensorsDriver.hpp"
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
}

Sensors::~Sensors()
{
    delete sensorsdriver_;
}

SensorsTimer::SensorsTimer(Sensors & sensors, int timeSpan_ms, std::string name) :
        sensors_(sensors), chrono_("SensorsTimer")
{
    name_ = name;
    lasttime_ = 0;
    timeSpan_ms_ = timeSpan_ms;
    logger().debug() << "timeSpan_ms=" << timeSpan_ms << logs::end;
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

int Sensors::rightSide()
{
    return sensorsdriver_->rightSide();
}
int Sensors::leftSide()
{
    return sensorsdriver_->leftSide();
}

//retourne 0, sinon le niveau detecté 2 veryClosed, 1 first level
int Sensors::front()
{
    int fL = sensorsdriver_->frontLeft();
    int fC = sensorsdriver_->frontCenter();
    int fR = sensorsdriver_->frontRight();

    //logger().info() << " L " << enableFrontLeft_ << " C " << enableFrontCenter_ << " R " << enableFrontRight_ << logs::end;
    int level = 0;

    if ((enableFrontLeft_ && (fL < frontLeftThreshold_)) || (enableFrontCenter_ && (fC < frontCenterThreshold_))
            || (enableFrontRight_ && (fR < frontRightThreshold_))) {
        logger().debug() << "1 frontLeft= " << fL << " frontCenter= " << fC << " frontRight= " << fR << logs::end;
        level = 1;
    }

    if ((enableFrontLeft_ && (fL < frontLeftVeryClosedThreshold_))
            || (enableFrontCenter_ && (fC < frontCenterVeryClosedThreshold_))
            || (enableFrontRight_ && (fR < frontRightVeryClosedThreshold_))) {
        logger().debug() << "2 frontLeft= " << fL << " frontCenter= " << fC << " frontRight= " << fR << logs::end;

        level = 2;
    }

    return level;
}

int Sensors::back()
{
    int bL = sensorsdriver_->backLeft();
    int bC = sensorsdriver_->backCenter();
    int bR = sensorsdriver_->backRight();
    int level = 0;

    if ((enableBackLeft_ && (bL < backLeftThreshold_)) || (enableBackCenter_ && (bC < backCenterThreshold_))
            || (enableBackRight_ && (bR < backRightThreshold_))) {
        logger().debug() << "1 backLeft= " << bL << " backCenter= " << bC << " backRight= " << bR << logs::end;
        level = 1;
    }

    if ((enableBackLeft_ && (bL < backLeftVeryClosedThreshold_))
            || (enableBackCenter_ && (bC < backCenterVeryClosedThreshold_))
            || (enableBackRight_ && (bR < backRightVeryClosedThreshold_))) {
        logger().debug() << "2 backLeft= " << bL << " backCenter= " << bC << " backRight= " << bR << logs::end;

        level = 2;
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
    int frontLevel = sensors_.front();
    if (frontLevel == 1) {
        //send collision to asserv
        if (lastdetect_front_nb_ == 0) {

            sensors_.robot()->asserv_default->base()->motors().stopMotors(); //pour etre plus reactif sur l'arret sinon on touche
            sensors_.robot()->asserv_default->warnFrontCollisionOnTraj();
            sensors_.robot()->asserv_default->base()->motors().stopMotors(); //pour etre plus reactif sur l'arret sinon on touche

            sensors_.robot()->asserv_default->setLowSpeedForward(true, 40); //TODO valeur differente a faire par robot
        }
        lastdetect_front_nb_++;
    }

//cas nearest
    if (lastdetect_front_nb_ > 0) {
        if (frontLevel == 2) {
            sensors_.robot()->asserv_default->base()->motors().stopMotors(); //pour etre plus reactif sur l'arret sinon on touche
            sensors_.robot()->asserv_default->warnFrontCollisionOnTraj();
            sensors_.robot()->asserv_default->base()->motors().stopMotors(); //pour etre plus reactif sur l'arret sinon on touche
        }
        if (frontLevel == 0) {
            lastdetect_front_nb_ = 0;
            sensors_.robot()->asserv_default->setLowSpeedForward(false); //TOD le zero ne sert a rien
        }
    }

//ARRIERE/////////////////////////////////////////////////////////////////
    int backLevel = sensors_.back();
    if (backLevel == 1) {
        //send collision to asserv

        if (lastdetect_back_nb_ == 0) {

            sensors_.robot()->asserv_default->base()->motors().stopMotors(); //pour etre plus reactif sur l'arret sinon on touche
            sensors_.robot()->asserv_default->warnBackCollisionOnTraj();
            sensors_.robot()->asserv_default->base()->motors().stopMotors(); //pour etre plus reactif sur l'arret sinon on touche

            sensors_.robot()->asserv_default->setLowSpeedBackward(true, 40); //TODO a faire par robot

        }
        lastdetect_back_nb_++;
    }

    if (lastdetect_back_nb_ > 0) {
        if (backLevel == 2) {
            sensors_.robot()->asserv_default->base()->motors().stopMotors(); //pour etre plus reactif sur l'arret sinon on touche
            sensors_.robot()->asserv_default->warnBackCollisionOnTraj();
            sensors_.robot()->asserv_default->base()->motors().stopMotors(); //pour etre plus reactif sur l'arret sinon on touche
        }
        if (backLevel == 0) {
            lastdetect_back_nb_ = 0;
            sensors_.robot()->asserv_default->setLowSpeedBackward(false);
        }
    }

    logger().info() << "onTimer() " << this->info() << "=" << chrono.getElapsedTimeInMicroSec() << " us "
            << lastdetect_front_nb_ << " front=" << frontLevel << " back=" << backLevel << logs::end;
}

void SensorsTimer::onTimerEnd(utils::Chronometer chrono)
{
//    logger().debug() << "onTimerEnd() " << this->info() << "=" << chrono.getElapsedTimeInMicroSec() << " us"
//            << logs::end;

}

std::string SensorsTimer::info()
{
    std::ostringstream oss;
    oss << "SensorsTimer for " << sensors_.robot()->getID();
    return oss.str();
}

