#ifndef OPOS6UL_IAEXTENDED_HPP_
#define OPOS6UL_IAEXTENDED_HPP_

#include <string>

#include "../Common/IA/IAbyZone.hpp"



class OPOS6UL_IAExtended
{
private:

	IAbyZone iaz_;

public:
	OPOS6UL_IAExtended(std::string botId, Robot *robot);

	~OPOS6UL_IAExtended();

	IAbyZone & iAbyZone()
	{
		return iaz_;
	}

};

#endif
