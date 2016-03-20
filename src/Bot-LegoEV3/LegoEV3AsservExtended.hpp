#ifndef LegoEV3_ASSERVEXTENDED_HPP_
#define LegoEV3_ASSERVEXTENDED_HPP_

#include <cstdio>
#include <string>

#include "../Common/Asserv/Asserv.hpp"
#include "../Common/Asserv/MovingBase.hpp"

class LegoEV3AsservExtended: public Asserv
{
private:

	/*!
	 * \brief motorisation = motors + encoders
	 */
	MovingBase movingBase_;

public:
	LegoEV3AsservExtended(std::string botId)
			: movingBase_(botId, *this)
	{
	}

	~LegoEV3AsservExtended()
	{
		//printf("~LegoEV3AsservExtended()\n");
	}

	/*!
	 * \brief return objet movingBase.
	 * \return movingBase_.
	 */
	MovingBase & base()
	{
		return movingBase_;
	}
};

#endif

