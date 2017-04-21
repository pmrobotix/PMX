#ifndef LEGOEV3_ACTIONSEXTENDED_HPP_
#define LEGOEV3_ACTIONSEXTENDED_HPP_

#include <string>

#include "../Common/Action/Actions.hpp"
#include "../Common/Action/ButtonBar.hpp"
#include "../Common/Action/FunnyAction.hpp"
#include "../Common/Action/LedBar.hpp"
#include "../Common/Action/Sensors.hpp"
#include "../Common/Action/ServoObjectsSystem.hpp"
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

	/*!
	 * \brief capteurs IR/US.
	 */
	Sensors sensors_;

	/*!
	 * \brief Funny action
	 */
	//FunnyAction funnyAction_;
	/*!
	 * \brief Servo Objects.
	 */
	ServoObjectsSystem servoObjects_;

public:
	LegoEV3ActionsExtended(std::string botId, Robot * robot) :
			ledbar_(botId, *this, 2), buttonbar_(*this), soundbar_(*this), tirette_(*this), sensors_(
					*this, robot),
			//funnyAction_(*this),
			servoObjects_(*this)
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

	/*!
	 * \brief Cette methode retourne l'objet sensors.
	 * \return sensors_.
	 */
	Sensors & sensors()
	{
		return sensors_;
	}

	/*!
	 * \brief Cette methode retourne l'objet FunnyAction.
	 * \return funnyAction_.
	 */
	/*FunnyAction & funnyAction()
	 {
	 return funnyAction_;
	 }*/

	ServoObjectsSystem & servoObjects()
	{
		return servoObjects_;
	}

	void stop()
	{
		//funnyAction_.release();
		servoObjects_.releaseAll();

		ledbar_.resetAll();
		ledbar_.stop();

		soundbar_.stop();

		Actions::stop(); //stop devices and wait manager to finish
	}

	//--------------------------------------------------------------
	//Actions 2017
	//--------------------------------------------------------------
	void funnyAction_Init()
	{
		servoObjects().deploy(SERVO_FUNNY_ACTION, -10);
	}

	void funnyAction_Activate()
	{
		servoObjects().deploy(SERVO_FUNNY_ACTION, -70);
	}

	void pince_HerculeDown()
	{
		servoObjects().deploy(SERVO_HERCULE, 80, 1);
	}

	void pince_HerculeUp()
	{
		servoObjects().deploy(SERVO_HERCULE, 0,1);
	}

	void pince_InitRotation()
	{
		servoObjects().deploy(SERVO_BASCULE, -100, 2);
	}
	void pince_Rotate()
	{
		servoObjects().deploy(SERVO_BASCULE, 100, 3);
	}

	void pince_Open()
	{
		servoObjects().deploy(SERVO_PINCE, -15, 1); //ouverte
	}

	void pince_Close(int keep_sec)
	{
		servoObjects().deploy(SERVO_PINCE, 50, keep_sec); //fermée
	}

};

#endif

