#include "LegoEV3IAExtended.hpp"

#include <string>

class Robot;

LegoEV3IAExtended::LegoEV3IAExtended(std::string botId, Robot *robot)
		: iaz_(robot)
{
	//iaz_.ia_setAsserv(asserv);
	//iaz_.ia_setRobot(robot);

}

LegoEV3IAExtended::~LegoEV3IAExtended()
{
}
