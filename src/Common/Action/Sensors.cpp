#include "Sensors.hpp"

#include <sstream>

#include "../Action.Driver/ASensorsDriver.hpp"
#include "../Asserv/Asserv.hpp"
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
		sensors_(sensors)
{
	name_ = name;
	lasttime_ = 0;
	timeSpan_ms_ = timeSpan_ms;
	logger().debug() << "timeSpan_ms=" << timeSpan_ms << logs::end;
}

bool Sensors::front()
{
	return sensorsdriver->front();
}

bool Sensors::rear()
{
	return sensorsdriver->rear();
}

void Sensors::startSensors()
{
	logger().debug() << "startSensors" << logs::end;

	this->actions().addTimer(new SensorsTimer(*this, 300, "sensors"));

}

void Sensors::stopSensors()
{

}

void SensorsTimer::onTimer(utils::Chronometer chrono)
{
	logger().debug() << "onTimer() " << this->info() << "=" << chrono.getElapsedTimeInMicroSec()
			<< " us" << logs::end;

	bool front = sensors_.front();

	if (front)
	{
		//send collision to asserv
		sensors_.robot()->asserv_default->setFrontCollision();
	}

	bool rear = sensors_.rear();

	if (rear)
	{
		//send collision to asserv
		sensors_.robot()->asserv_default->setRearCollision();
	}
}

void SensorsTimer::onTimerEnd(utils::Chronometer chrono)
{
	logger().debug() << "onTimerEnd() " << this->info() << "=" << chrono.getElapsedTimeInMicroSec()
			<< " us" << logs::end;

}

std::string SensorsTimer::info()
{
	std::ostringstream oss;
	oss << "SensorsTimer for " << sensors_.robot()->getID();
	return oss.str();
}

