#ifndef LegoEV3_IAEXTENDED_HPP_
#define LegoEV3_IAEXTENDED_HPP_

#include "../Common/IA/IAbyZone.hpp"

class LegoEV3IAExtended
{
private:

	IAbyZone iaz_;

public:
	LegoEV3IAExtended(std::string botId, Robot *robot);

	~LegoEV3IAExtended();

	IAbyZone & iAbyZone()
	{
		return iaz_;
	}

};

#endif
