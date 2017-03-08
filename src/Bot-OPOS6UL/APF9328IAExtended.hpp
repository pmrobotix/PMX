#ifndef APF9328_IAEXTENDED_HPP_
#define APF9328_IAEXTENDED_HPP_

#include <string>

#include "../Common/IA/IAbyZone.hpp"



class APF9328IAExtended
{
private:

	IAbyZone iaz_;

public:
	APF9328IAExtended(std::string botId, Robot *robot);

	~APF9328IAExtended();

	IAbyZone & iAbyZone()
	{
		return iaz_;
	}

};

#endif
