#ifndef LEGOEV3_ACTIONSEXTENDED_HPP_
#define LEGOEV3_ACTIONSEXTENDED_HPP_

#include <cstdio>
#include <string>

#include "../Common/Action/Actions.hpp"
#include "../Common/Action/ButtonBar.hpp"
#include "../Common/Action/LedBar.hpp"
#include "../Common/Action/SoundBar.hpp"
#include "../Common/Action/Tirette.hpp"

/*
 * Contains all specific robot element parts.
 */
class LegoEV3ActionsExtended: public Actions
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

	/*!
	 * \brief Sound Bar.
	 */
	SoundBar soundbar_;

	/*!
	 * \brief Tirette.
	 */
	Tirette tirette_;

public:
	LegoEV3ActionsExtended(std::string botId)
			: ledbar_(botId, *this, 2), buttonbar_(*this), soundbar_(*this), tirette_(*this)
	{
	}

	~LegoEV3ActionsExtended()
	{
		//printf("~LegoEV3ActionsExtended()\n");
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

	/*!
	 * \brief Cette methode retourne l'objet soundBar.
	 * \return soundbar_.
	 */
	SoundBar & soundBar()
	{
		return soundbar_;
	}

	/*!
	 * \brief Cette methode retourne l'objet tirette.
	 * \return tirette_.
	 */
	Tirette & tirette()
	{
		return tirette_;
	}

	void stop()
	{
		ledbar_.resetAll();
		Actions::stop(); //stop devices and wait manager to finish
	}

};

#endif

