#ifndef SENSORS_HPP_
#define SENSORS_HPP_

#include "../../Log/LoggerFactory.hpp"
#include "../Action.Driver/ASensorsDriver.hpp"
#include "AActionsElement.hpp"


class Sensors: public AActionsElement
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref Sensors.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("Sensors");
		return instance;
	}

public:
	ASensorsDriver* sensorsdriver;

	/*!
	 * \brief Constructor.
	 *
	 */
	Sensors(Actions & actions);

	/*!
	 * \brief Destructor.
	 */
	~Sensors();

	bool front();

	bool rear();


};

#endif
