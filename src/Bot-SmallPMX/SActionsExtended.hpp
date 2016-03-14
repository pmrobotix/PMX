#ifndef PMX_S_ACTIONSEXTENDED_HPP_
#define PMX_S_ACTIONSEXTENDED_HPP_

#include "../Common/Action/Actions.hpp"
#include "../Common/Action/ButtonBar.hpp"
#include "../Common/Action/LedBar.hpp"


class SActionsExtended: public Actions
{
private:

	/*!
	 * \brief LED Bar.
	 */
	LedBar ledbar_;

	/*!
	 * \brief Button Bar.
	 */
	ButtonBar buttonbar_;


public:
	SActionsExtended(std::string botId) :
			ledbar_(botId, *this, 2), buttonbar_(*this)
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

	/*!
	 * \brief Cette methode retourne l'objet buttonBar.
	 * \return buttonbar_.
	 */
	ButtonBar & buttonBar()
	{
		return buttonbar_;
	}

	void emergencyStop()
	{
		ledbar_.resetAll();
	}

};

#endif

