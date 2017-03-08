#include <string>
#include "OPOS6UL_IAExtended.hpp"

class Robot;

OPOS6UL_IAExtended::OPOS6UL_IAExtended(std::string botId, Robot *robot)
		: iaz_(robot)
{

}

OPOS6UL_IAExtended::~OPOS6UL_IAExtended()
{
}
