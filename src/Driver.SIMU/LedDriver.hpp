#ifndef SIMU_LEDDRIVER_HPP_
#define SIMU_LEDDRIVER_HPP_

#include "../Common/Action.Driver/ALedDriver.hpp"
#include "../Common/Utils/Macro.hpp"
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
		static const logs::Logger & instance = logs::LoggerFactory::logger("LedDriver.SIMU");
		return instance;
	}

	int nb_;

public:

	/*!
	 * \brief Liste des gpio à utiliser pour ce robot.
	 */
	int *gpio;

	uint hexa;


	virtual void setBit(int position, LedColor color);

	virtual void setBytes(uint hexPosition, LedColor color);

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
