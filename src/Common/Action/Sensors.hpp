#ifndef SENSORS_HPP_
#define SENSORS_HPP_

#include <string>

#include "../../Log/Logger.hpp"
#include "../../Log/LoggerFactory.hpp"
#include "../Utils/Chronometer.hpp"
#include "AActionsElement.hpp"
#include "ITimerListener.hpp"

class Robot;

class ASensorsDriver;

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

	Robot * robot_;

public:
	ASensorsDriver* sensorsdriver;

	/*!
	 * \brief Constructor.
	 *
	 */
	Sensors(Actions & actions, Robot * robot);

	/*!
	 * \brief Destructor.
	 */
	~Sensors();

	Robot * robot()
	{
		return robot_;
	}

	bool front();

	bool rear();

	void startSensors();

	void stopSensors();

};

/*!
 * \brief Le timer associé
 *
 */
class SensorsTimer: public ITimerListener
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref SensorsTimer.
	 */
	static const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("SensorsTimer");
		return instance;
	}

	/*!
	 * \brief Référence vers sensors.
	 */
	Sensors & sensors_;

	utils::Chronometer chrono_;

public:

	/*!
	 * \brief Constructeur de la classe.
	 * \param amt
	 *        Reference vers l'objet associée.
	 */
	SensorsTimer(Sensors & sensors, int timeSpan_ms, std::string name);

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual inline ~SensorsTimer()
	{
		logger().debug() << "~SensorsTimer()" << logs::end;
	}

	virtual void onTimer(utils::Chronometer chrono);

	virtual void onTimerEnd(utils::Chronometer chrono);

	virtual std::string info();


};

#endif
