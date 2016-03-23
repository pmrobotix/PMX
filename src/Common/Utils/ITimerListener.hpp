/*!
 * \file
 * \brief Définition de l'interface ITimerListener.
 */

#ifndef COMMON_ITIMERLISTENER_HPP
#define COMMON_ITIMERLISTENER_HPP


namespace utils
{
/*!
 * \brief Cette interface représente une action executée par un timer lorsqu'il
 * atteint son seuil.
 */
class ITimerListener
{
public:

	virtual void onTimer(Chronometer chrono) = 0;

	virtual void onTimerEnd(Chronometer chrono) = 0;

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual inline ~ ITimerListener()
	{
	}

protected:

	/*!
	 * \brief Constructeur de la classe.
	 */
	ITimerListener()
	{
	}
};
}

#endif
