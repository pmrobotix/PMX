#ifndef ARM_SENSORSDRIVER_HPP_
#define ARM_SENSORSDRIVER_HPP_

#include "../Common/Action.Driver/ASensorsDriver.hpp"
#include "../Log/LoggerFactory.hpp"
#include "IrSensor.hpp"

using namespace std;

class SensorsDriver: public ASensorsDriver
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref SensorsDriver(ARM).
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("SensorsDriver.ARM");
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
	virtual ~SensorsDriver();

	virtual bool front();
	virtual bool rear();

};

#endif
