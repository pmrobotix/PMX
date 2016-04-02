#ifndef ARMA_LEDDRIVER_HPP_
#define ARMA_LEDDRIVER_HPP_

#include <sys/types.h>

#include "../Common/Action.Driver/ALedDriver.hpp"
#include "../Log/LoggerFactory.hpp"

class HostGpioPort;

//#include <as_devices/cpp/as_gpio.hpp>



class LedDriver: public ALedDriver
{
	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref LedDriver(ARM).
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("LedDriver.ARM");
		return instance;
	}

	int nb_;

public:

	HostGpioPort * paths[8];
	HostGpioPort * paths_as[8];

	/*!
	 * \brief Liste des gpio à utiliser.
	 */
	//AsGpio * gpio[8];
	virtual void setBit(int index, LedColor color);

	virtual void setBytes(uint hex, LedColor color);

	/*!
	 * \brief Constructor.
	 */
	LedDriver(int nb);

	/*!
	 * \brief Destructor.
	 */
	virtual ~LedDriver();

};

#endif
