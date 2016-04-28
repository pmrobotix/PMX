#include "Sensors.hpp"

using namespace std;

Sensors::Sensors(Actions & actions)
		: AActionsElement(actions)

{
	sensorsdriver = ASensorsDriver::create();
}

Sensors::~Sensors()
{
}

bool Sensors::front()
{
	return sensorsdriver->front();
}

bool Sensors::rear()
{
	return sensorsdriver->rear();
}
