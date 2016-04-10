#ifndef PMX_APF9328_ACTIONSEXTENDED_HPP_
#define PMX_APF9328_ACTIONSEXTENDED_HPP_

#include <string>

#include "../Common/Action/Actions.hpp"
#include "../Common/Action/LcdShield.hpp"
#include "../Common/Action/LedBar.hpp"

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

public:
	APF9328ActionsExtended(std::string botId)
			: ledbar_(botId, *this, 8), lcd2x16_(botId, *this)

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

	LcdShield & lcd2x16()
	{
		return lcd2x16_;
	}

	void stop()
	{
		ledbar_.resetAll();
		lcd2x16_.reset();


		Actions::stop(); //stop devices and wait manager to finish
	}

};

#endif

