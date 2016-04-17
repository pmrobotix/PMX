#ifndef LegoEV3_ASSERVEXTENDED_HPP_
#define LegoEV3_ASSERVEXTENDED_HPP_

#include <string>

#include "../Common/Asserv/Asserv.hpp"
#include "../Common/Asserv/MovingBase.hpp"

class LegoEV3AsservExtended: public Asserv
{
private:

public:
	LegoEV3AsservExtended(std::string botId, Robot * robot)
			: Asserv(botId, robot) //on appelle le constructeur pere
	{
	}

	~LegoEV3AsservExtended()
	{
	}

	void startMotionTimerAndOdo();

};

#endif

