/*!
 * \file
 * \brief Définition de l'interface ITimerListener.
 */

#ifndef COMMON_ITIMERLISTENER_HPP
#define COMMON_ITIMERLISTENER_HPP

#include <string>

namespace utils
{
class Chronometer;
} /* namespace utils */

/*!
 * \brief Cette interface représente une action executée par un timer lorsqu'il
 * atteint son seuil d'execution.
 */
class ITimerListener
{
public:

	/*!
	 * \brief Getter sur les infos permettant d'identifier le timer.
	 */
	virtual void onTimer(utils::Chronometer chrono) = 0;

	/*!
	 * \brief Getter sur les infos permettant d'identifier le timer.
	 */
	virtual void onTimerEnd(utils::Chronometer chrono) = 0;

	/*!
	 * \brief Getter sur les infos permettant d'identifier le timer.
	 */
	virtual std::string info()
	{
		return name_;
	}

	inline int timeSpan()
	{
		return timeSpan_ms_;
	}

	//last execution time
	inline int getLastTime()
	{
		return lasttime_;
	}

	inline void setLastTime(long l)
	{
		lasttime_ = l;
	}

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual inline ~ ITimerListener()
	{
	}

protected:

	long timeSpan_ms_; //for Timer tasks

	long lasttime_;

	std::string name_;

	/*!
	 * \brief Constructeur de la classe.
	 */
	ITimerListener()
	{
		timeSpan_ms_ = 0;
		lasttime_ = 0;
		name_ = "iTimerListener_default";
	}
};

#endif
