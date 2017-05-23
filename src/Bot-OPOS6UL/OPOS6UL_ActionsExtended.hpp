#ifndef OPOS6UL_ACTIONSEXTENDED_HPP_
#define OPOS6UL_ACTIONSEXTENDED_HPP_

#include <string>

#include "../Common/Action/Actions.hpp"
#include "../Common/Action/ButtonBar.hpp"
#include "../Common/Action/LcdShield.hpp"
#include "../Common/Action/LedBar.hpp"
#include "../Common/Action/Sensors.hpp"
#include "../Common/Action/ServoObjectsAx12.hpp"
#include "../Common/Action/Tirette.hpp"

class OPOS6UL_ActionsExtended: public Actions
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
	 * \brief LcdShield 2x16 characters.
	 */
	LcdShield lcd2x16_;

	/*!
	 * \brief la tirette du robot.
	 */
	Tirette tirette_;

	/*!
	 * \brief capteurs IR/US.
	 */
	Sensors sensors_;

	/*!
	 * \brief objets avec servomotors.
	 */
	ServoObjectsAx12 servoObjects_;

public:
	OPOS6UL_ActionsExtended(std::string botId, Robot *robot) :
			ledbar_(botId, *this, 8), buttonbar_(*this), lcd2x16_(botId, *this), tirette_(*this), sensors_(
					*this, robot), servoObjects_(*this)

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

	ButtonBar & buttonBar()
	{
		return buttonbar_;
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
	 * \brief Cette methode retourne l'objet sensors.
	 * \return sensors_.
	 */
	Sensors & sensors()
	{
		return sensors_;
	}

	ServoObjectsAx12 & servoObjects()
	{
		return servoObjects_;
	}

	void stop()
	{
		ledbar_.resetAll();
		lcd2x16_.reset();
		servoObjects().releaseAll();

		Actions::stop(); //stop devices and wait manager to finish
	}

	//--------------------------------------------------------------
	//Actions 2017
	//--------------------------------------------------------------
	void funnyAction_Init()
	{
		servoObjects().deploy(51, 692, 0);
	}
	void funnyAction_Activate()
	{
		servoObjects().deploy(51, 812, 0);
	}

	void turn_nene_left()
	{
		servoObjects().deploy(5, 212, 0);
	}
	void turn_nene_right()
	{
		servoObjects().deploy(5, 812, 0);
	}
	void turn_nene_center()
	{
		servoObjects().deploy(5, 512, 0);
	}
	void turn_nene_center_left()
	{
		servoObjects().deploy(5, 412, 0);
	}
	void turn_nene_center_right()
	{
		servoObjects().deploy(5, 620, 0);
	}

	void nose_init()
	{
		servoObjects().deploy(8, 412, 0);
	}
	void nose_up()
	{
		servoObjects().deploy(8, 512, 0);
	}
	void nose_down()
	{
		servoObjects().deploy(8, 162, 0);
	}
	void nose_back()
	{
		servoObjects().deploy(8, 712, 0);
	}
	void nose_turn()
	{
		servoObjects().turn(12, 512, 0);
	}

	void arm_retract()
	{
		servoObjects().deploy(3, 832, 0);
		servoObjects().deploy(6, 512, 0);
	}
	void arm_deploy()
	{
		servoObjects().deploy(3, 600, 0);
		servoObjects().deploy(6, 812, 0);
		nose_back();
	}
	void arm_right()
	{
		servoObjects().deploy(7, 272, 0);
	}
	void arm_left()
	{
		servoObjects().deploy(7, 692, 0);
	}
	void arm_center()
	{
		servoObjects().deploy(7, 512, 0);
	}
	void servo_lowspeed()
	{
		servoObjects().setSpeedAll(80);
	}

	void servo_mediumspeed()
	{
		servoObjects().setSpeedAll(200);
	}
	void servo_init()
	{
		servo_lowspeed();

		funnyAction_Init();
		nose_down();
		arm_retract();
		arm_center();
		turn_nene_center();

	}

};

#endif

