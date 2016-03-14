#ifndef SIMULAUNCHER_MAIN_SIMULATOR_HPP_
#define SIMULAUNCHER_MAIN_SIMULATOR_HPP_

#include "../Log/LoggerFactory.hpp"

class BotManager;
class MainWindow;
class SecondWindow;
class ThirdWindow;

class Simulator
{

private:
	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref Simulator.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("Simulator");
		return instance;
	}

	Simulator();

public:

	static Simulator & instance()
	{
		static Simulator instance;
		return instance;
	}

	//Starts up SDL and creates window
	bool initSDL();

	//create windows
	bool createWindows();

	//Frees media and shuts down SDL
	void closeSDL();

	void execute();

	~Simulator();

	void enableLedDriver();

	MainWindow *mainWindow;
	SecondWindow *secondWindow;
	ThirdWindow *thirdWindow;
	BotManager *botm;

};

#endif /* SIMULAUNCHER_MAIN_SIMULATOR_HPP_ */
