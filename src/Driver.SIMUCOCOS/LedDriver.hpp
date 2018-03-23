#ifndef SIMUCOCOS_LEDDRIVER_HPP_
#define SIMUCOCOS_LEDDRIVER_HPP_

#include <sys/types.h>

#include "../Common/Action.Driver/ALedDriver.hpp"
#include "../Log/LoggerFactory.hpp"

#define NB_GPIO 255 //max number of led

class LedDriver: public ALedDriver
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref LedDriver(SIMU).
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("LedDriver.SIMUCOCOS");
		return instance;
	}

	int nb_;

public:

	/*!
	 * \brief Liste des gpio à utiliser pour ce robot.
	 */
	int *gpio;

	uint hexa;


	void setBit(int position, LedColor color);

	void setBytes(uint hexPosition, LedColor color);

	/*!
	 * \brief Constructor.
	 */
	LedDriver(int nb);

	/*!
	 * \brief Destructor.
	 */
	~LedDriver();

};

#endif
