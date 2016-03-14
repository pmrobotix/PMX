#ifndef EV3_LEDDRIVER_EV3_HPP_
#define EV3_LEDDRIVER_EV3_HPP_

#include "../Common/Action.Driver/ALedDriver.hpp"
#include "../Common/Utils/Macro.hpp"
#include "../Log/LoggerFactory.hpp"

class LedDriver: public ALedDriver
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref LedDriver(EV3).
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("LedDriver.EV3");
		return instance;
	}

	int nb_;

public:

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
