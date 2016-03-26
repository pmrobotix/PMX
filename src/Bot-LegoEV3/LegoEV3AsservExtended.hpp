#ifndef LegoEV3_ASSERVEXTENDED_HPP_
#define LegoEV3_ASSERVEXTENDED_HPP_

#include <string>

#include "../Common/Asserv/Asserv.hpp"
#include "../Common/Asserv/MovingBase.hpp"
#include "../Common/Asserv.Insa/AsservInsa.hpp"

class LegoEV3AsservExtended: public Asserv
{
private:

	/*!
	 * \brief motorisation = motors + encoders
	 */
	MovingBase movingBase_;

	AsservInsa asservinsa_;

public:
	LegoEV3AsservExtended(std::string botId)
			: movingBase_(botId, *this), asservinsa_()
	{
		asservinsa_.setMovingBase(&movingBase_);
	}

	~LegoEV3AsservExtended()
	{
	}

	/*!
	 * \brief return objet movingBase.
	 * \return movingBase_.
	 */
	MovingBase & base()
	{
		return movingBase_;
	}

	void startAsservInsa()
	{

		asservinsa_.motion_Init();
		//logger().debug("AsservInsa is started");

	}

	void stopAsservInsa()
	{

		//logger().debug("AsservInsa is stopped");

	}

	void stop()
	{

	}
};

#endif

