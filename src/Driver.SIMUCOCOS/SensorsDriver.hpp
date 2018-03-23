#ifndef SIMUCOCOS_SENSORSDRIVER_HPP_
#define SIMUCOCOS_SENSORSDRIVER_HPP_

#include "../Common/Action.Driver/ASensorsDriver.hpp"
#include "../Log/LoggerFactory.hpp"


using namespace std;


class SensorsDriver: public ASensorsDriver
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref SensorsDriver(SIMU).
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("SensorsDriver.SIMUCOCOS");
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
