#ifndef OPOS6UL_SENSORSDRIVER_HPP_
#define OPOS6UL_SENSORSDRIVER_HPP_

#include "../Common/Action.Driver/ASensorsDriver.hpp"
#include "../Log/LoggerFactory.hpp"
#include "IrSensor.hpp"

using namespace std;

class SensorsDriver: public ASensorsDriver
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref SensorsDriver(OPOS6UL).
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("SensorsDriver.OPO");
		return instance;
	}



public:

	/*!
	 * \brief Constructor.
	 */
	SensorsDriver();

	/*!
	 * \brief Destructor.
	 */
	~SensorsDriver();

	bool front();
	bool rear();

};

#endif
