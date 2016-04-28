#ifndef APF9328_ACTIONSEXTENDED_HPP_
#define APF9328_ACTIONSEXTENDED_HPP_

#include <string>

#include "../Common/Action/Actions.hpp"
#include "../Common/Action/LcdShield.hpp"
#include "../Common/Action/LedBar.hpp"
#include "../Common/Action/Sensors.hpp"
#include "../Common/Action/Tirette.hpp"

class APF9328ActionsExtended: public Actions
{
private:

	/*!
	 * \brief LED Bar.
	 */
	LedBar ledbar_;

	/*!
	 * \brief LcdShield 2x16 characters.
	 */
	LcdShield lcd2x16_;

	/*!
	 * \brief la tirette du robot.
	 */
	Tirette tirette_;

	Sensors sensors_;

public:
	APF9328ActionsExtended(std::string botId)
			: ledbar_(botId, *this, 8), lcd2x16_(botId, *this), tirette_(*this), sensors_(*this)

	{
		lcd2x16_.init();
	}

	/*!
	 * \brief Cette methode retourne l'objet ledbar.
	 * \return ledbar_.
	 */
	LedBar & ledBar()
	{
		return ledbar_;
	}

	/*!
	 * \brief Cette methode retourne l'objet LcdShield.
	 * \return lcd2x16_.
	 */
	LcdShield & lcd2x16()
	{
		return lcd2x16_;
	}

	/*!
	 * \brief Cette methode retourne l'objet tirette.
	 * \return tirette_.
	 */
	Tirette & tirette()
	{
		return tirette_;
	}

	/*!
	 * \brief Cette methode retourne l'objet tirette.
	 * \return tirette_.
	 */
	Sensors & sensors()
	{
		return sensors_;
	}

	void stop()
	{
		ledbar_.resetAll();
		lcd2x16_.reset();

		Actions::stop(); //stop devices and wait manager to finish
	}

};

#endif

