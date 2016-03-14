#ifndef PMX_B_ACTIONSEXTENDED_HPP_
#define PMX_B_ACTIONSEXTENDED_HPP_

#include "../Common/Action/LedBar.hpp"
#include "../Common/Action/Actions.hpp"

class BActionsExtended: public Actions
{
private:

	/*!
	 * \brief LED Bar.
	 */
	LedBar ledbar_;


public:
	BActionsExtended(std::string botId) :
			ledbar_(botId, *this, 8)
	{
	}

	/*!
	 * \brief Cette methode retourne l'objet ledbar.
	 * \return ledbar_.
	 */
	LedBar & ledBar()
	{
		return ledbar_;
	}


	void stop()
	{
		ledbar_.resetAll();
		Actions::stop(); //stop devices and wait manager to finish
	}

};

#endif

