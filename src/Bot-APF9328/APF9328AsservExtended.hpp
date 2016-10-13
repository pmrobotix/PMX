#ifndef APF9328_ASSERVEXTENDED_HPP_
#define APF9328_ASSERVEXTENDED_HPP_

#include <string>

#include "../Common/Asserv/Asserv.hpp"

class APF9328AsservExtended: public Asserv
{
private:

public:
	APF9328AsservExtended(std::string botId, Robot * robot)
			: Asserv(botId, robot) //on appelle le constructeur pere
	{

	}

	~APF9328AsservExtended()
	{
	}

	void startMotionTimerAndOdo();

};

#endif
