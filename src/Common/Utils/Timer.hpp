/*!
 * \file
 * \brief Définition de la classe Timer (Minuteur).
 */

#ifndef COMMON_TIMER_HPP
#define	COMMON_TIMER_HPP

#include "../../Log/LoggerFactory.hpp"
#include "../../Thread/Thread.hpp"
#include "Chronometer.hpp"

namespace utils
{
class ITimerListener;
} /* namespace utils */

namespace utils
{
/*!
 * \brief Cette classe permet de lancer des tâches sur une période donnée (thread utilisé).
 */
class Timer: public utils::Thread //!\todo utiliser un seul thread pour tous les minuteurs Timer
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref Timer.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("utils::Timer");
		return instance;
	}

private:
	/*!
	 * \brief Interval de temps d'exécution du Minuteur (en ms).
	 */
	int timeSpan_ms_;

	/*!
	 * \brief Interval minimum de temps du Minuteur (en ms) permettant une interruption.
	 * Doit être un multiple de timeSpan_ pour le moment.
	 *
	 */
	int timeSpanMin_ms_;

	/*!
	 *\brief Chronomètre lié au Minuteur.
	 */
	Chronometer chrono_;

	/*!
	 *\brief Listener lié au Minuteur.
	 */
	ITimerListener* listener_;

	bool stop_;

public:

	/*!
	 * \brief Constructeur de la classe.
	 * Le chronomètre créé n'est pas lancé par le constructeur.
	 *
	 * \param timeSpanMax Interval de temps d'exécution (en ms).
	 * \param timeSpanMin Interval de temps (en ms) permettant une interruption, doit
	 * être un multiple de timeSpan_ pour le moment.
	 */
	Timer(int timeSpanMax, int timeSpanMin, ITimerListener* listener);

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual inline ~ Timer()
	{
	}

	void inline stopTimer()
	{
		this->stop_ = true;
	}

	/*!
	 * \brief Change la fréquence d'appelle du Timer.
	 *
	 * \param value timeSpan (periode) en millisecondes.
	 */
	void inline changeTimeSpan(int value)
	{
		this->timeSpan_ms_ = value;
	}

protected:
	/*!
	 * \brief Assure le suivi du temps.
	 *
	 * Cette méthode est exécuter lors du lancement du thread associé à
	 * l'objet.
	 */
	virtual void execute();

	void runTask(Chronometer chrono);

	void runTaskEnd(Chronometer chrono);
};
}

#endif
