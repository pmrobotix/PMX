#ifndef SIMULAUNCHER_LEDDRIVER_HPP_
#define SIMULAUNCHER_LEDDRIVER_HPP_

#include <string>

#include "../Common/Action.Driver/ALedDriver.hpp"
#include "../Common/Utils/Macro.hpp"
#include "../Log/LoggerFactory.hpp"
#include "../SimuLauncher.Main/ISDLTask.hpp"

class SimLedBar;

class LedDriver: public ALedDriver
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref LedDriver.SIMULAUNCHER.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger(
				"LedDriver.SIMULAUNCHER");
		return instance;
	}

	static int nb_leddriver;

	//id of the current robot
	std::string botId_;

	int height_display_;

	SimLedBar * ledbar_;

	/*!
	 * \brief Nombre de led du robot.
	 */
	int nb_;

	std::string text_;

	int once_;

public:

	/*!
	 * \brief Constructor.
	 */
	LedDriver(std::string botname, int nb);

	virtual void setBit(int position, LedColor color);

	virtual void setBytes(uint hexPosition, LedColor color);

	void load();

	/*!
	 * \brief Destructor.
	 */
	virtual ~LedDriver();

};

class LedDriverSDLTask: public ISDLTask
{
public:
	LedDriverSDLTask(LedDriver *leddriver);

	void run();

	LedDriver *leddriver_;
};

#endif
