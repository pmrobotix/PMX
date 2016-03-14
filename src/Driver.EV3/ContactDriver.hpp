#ifndef EV3_CONTACTDRIVER_HPP_
#define EV3_CONTACTDRIVER_HPP_

#include "../Common/Action.Driver/AContactDriver.hpp"
#include "../Log/LoggerFactory.hpp"
#include "ev3dev.h"

using namespace std;
using namespace ev3dev;

class ContactDriver: public AContactDriver
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref ContactDriver(EV3).
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("ContactDriver.EV3");
		return instance;
	}

	touch_sensor touch_;

public:


	/*!
	 * \brief Constructor.
	 */
	ContactDriver();

	/*!
	 * \brief Destructor.
	 */
	virtual ~ContactDriver();

	virtual int pressed();

};

#endif
