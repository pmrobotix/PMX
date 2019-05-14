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
    sensorsdriver = ASensorsDriver::create(robot->getID());
}

Sensors::~Sensors()
{
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

bool Sensors::front()
{
    return sensorsdriver->front();
}

bool Sensors::frontVeryClosed()
{
    return sensorsdriver->frontVeryClosed();
}

bool Sensors::rear()
{
    return sensorsdriver->rear();
}

bool Sensors::rearVeryClosed()
{
    return sensorsdriver->rearVeryClosed();
}

void Sensors::addTimerSensors()
{
    logger().debug() << "startSensors" << logs::end;

    this->actions().addTimer(new SensorsTimer(*this, 100, "sensors"));

}

void SensorsTimer::onTimer(utils::Chronometer chrono)
{

    bool front = sensors_.front();
    bool frontVeryclosed = sensors_.frontVeryClosed();
    if (front) {
        //send collision to asserv
        if (lastdetect_front_nb_ == 0) {

            if (!sensors_.robot()->asserv_default->getIgnoreFrontCollision()) {
                sensors_.robot()->asserv_default->base()->motors().stopMotors(); //pour etre plus reactif sur l'arret sinon on touche
                sensors_.robot()->asserv_default->setFrontCollision();
                sensors_.robot()->asserv_default->base()->motors().stopMotors(); //pour etre plus reactif sur l'arret sinon on touche

                sensors_.robot()->asserv_default->setLowSpeed(true);
            }
        }
        lastdetect_front_nb_++;
    } else {
        lastdetect_front_nb_ = 0;
        sensors_.robot()->asserv_default->setLowSpeed(false);
    }

    //cas nearest
    if (lastdetect_front_nb_ > 0) {
        if (frontVeryclosed) {

            if (!sensors_.robot()->asserv_default->getIgnoreFrontCollision()) {
                sensors_.robot()->asserv_default->base()->motors().stopMotors(); //pour etre plus reactif sur l'arret sinon on touche
                sensors_.robot()->asserv_default->setFrontCollision();
                sensors_.robot()->asserv_default->base()->motors().stopMotors(); //pour etre plus reactif sur l'arret sinon on touche
            }

        }
    }

    //ARRIERE/////////////////////////////////////////////////////////////////
    bool rear = sensors_.rear();
    bool rearVeryclosed = sensors_.rearVeryClosed();
    if (rear) {
        //send collision to asserv

        if (lastdetect_back_nb_ == 0) {

            if (!sensors_.robot()->asserv_default->getIgnoreRearCollision()) {
                sensors_.robot()->asserv_default->base()->motors().stopMotors(); //pour etre plus reactif sur l'arret sinon on touche
                sensors_.robot()->asserv_default->setRearCollision();
                //sensors_.robot()->asserv_default->setLowSpeed(true); //TODO a faire en arrière parceque ca interfere avec la partie au dessus
            }
        }
        lastdetect_back_nb_++;
    } else {
        lastdetect_back_nb_ = 0;
        //sensors_.robot()->asserv_default->setLowSpeed(false); //TODO a faire en arrière parceque ca interfere avec la partie au dessus
    }

    if (lastdetect_back_nb_ > 0) {
        if (rearVeryclosed) {
            sensors_.robot()->asserv_default->base()->motors().stopMotors(); //pour etre plus reactif sur l'arret sinon on touche
            if (!sensors_.robot()->asserv_default->getIgnoreRearCollision())
                sensors_.robot()->asserv_default->setRearCollision();
        }
    }

    logger().error() << "onTimer() " << this->info() << "=" << chrono.getElapsedTimeInMicroSec() << " us "
            << lastdetect_front_nb_ << " front=" << front << " rear=" << rear << "  frontVeryclosed=" << frontVeryclosed
            << logs::end;
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

