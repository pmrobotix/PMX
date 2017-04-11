#ifndef EV3_SWITCHDRIVER_HPP_
#define EV3_SWITCHDRIVER_HPP_

#include "../Common/Action.Driver/ASwitchDriver.hpp"
#include "../Log/LoggerFactory.hpp"
#include "ev3dev.h"

using namespace std;
using namespace ev3dev;

class SwitchDriver: public ASwitchDriver
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref SwitchDriver(EV3).
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("SwitchDriver.EV3");
		return instance;
	}

	touch_sensor touch_;

public:


	/*!
	 * \brief Constructor.
	 */
	SwitchDriver();

	/*!
	 * \brief Destructor.
	 */
	~SwitchDriver();

	int pressed();

	int pressed(unsigned char pin);

};

#endif
