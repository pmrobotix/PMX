#include <string>
#include "APF9328IAExtended.hpp"

class Robot;

APF9328IAExtended::APF9328IAExtended(std::string botId, Robot *robot)
		: iaz_(robot)
{

}

APF9328IAExtended::~APF9328IAExtended()
{
}
