#ifndef LegoEV3_IAEXTENDED_HPP_
#define LegoEV3_IAEXTENDED_HPP_

#include <string>

#include "../Common/IA/IAbyZone.hpp"

class LegoEV3IAExtended
{
private:

public:
	LegoEV3IAExtended(std::string botId, Asserv *asserv);

	~LegoEV3IAExtended();

	IAbyZone iaz_;

};

#endif
