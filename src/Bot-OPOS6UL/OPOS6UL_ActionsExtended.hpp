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
		servoObjects().deploy(51, 842, 0);
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
		servoObjects().setSpeed(5, 50);
		servoObjects().deploy(5, 412, 0);
	}
	void turn_nene_center_right()
	{
		servoObjects().setSpeed(5, 50);
		servoObjects().deploy(5, 620, 0);
	}
	void turn_nene_right_trembl()
	{
		servoObjects().setSpeed(5, 200);
		servoObjects().deploy(5, 722, 300);
		servoObjects().deploy(5, 812, 300);
		servoObjects().deploy(5, 722, 300);
		servoObjects().deploy(5, 812, 300);
		servoObjects().deploy(5, 722, 300);
		servoObjects().deploy(5, 812, 300);
	}
	void turn_nene_left_trembl()
	{
		servoObjects().setSpeed(5, 200);
		servoObjects().deploy(5, 282, 300);
		servoObjects().deploy(5, 212, 300);
		servoObjects().deploy(5, 282, 300);
		servoObjects().deploy(5, 212, 300);
		servoObjects().deploy(5, 282, 300);
		servoObjects().deploy(5, 212, 300);
	}

	void nose_init()
	{
		servoObjects().deploy(8, 412, 0);
	}
	void nose_up(int speed)
	{
		servoObjects().setSpeed(8, speed);
		servoObjects().deploy(8, 512, 0);
		sleep(1);
	}
	void nose_down(int speed)
	{
		servoObjects().setSpeed(8, speed);
		servoObjects().deploy(8, 150, 0);
		usleep(1600000);
	}
	void nose_back(int speed)
	{
		servoObjects().setSpeed(8, speed);
		servoObjects().deploy(8, 712, 0);
		sleep(1);
	}
	void nose_turn(int speed)
	{
		servoObjects().turn(12, speed, 0);
	}
	void arm_back_cylinder(int speed)
	{
		servoObjects().setSpeed(6, speed);
		servoObjects().deploy(6, 812, 0);
	}

	void arm_kick(int speed)
	{
		servoObjects().setSpeed(6, speed);
		servoObjects().deploy(6, 672, 0);
	}
	void arm_pousser_plot(int speed)
	{
		servoObjects().setSpeed(6, speed);
		servoObjects().setSpeed(3, speed);
		servoObjects().deploy(6, 672, 0);
		servoObjects().deploy(3, 650, 0);
		sleep(1);
	}
	void arm_retract(int speed)
	{
		servoObjects().setSpeed(3, speed);
		servoObjects().setSpeed(6, speed);
		servoObjects().deploy(3, 832, 0);
		servoObjects().deploy(6, 512, 0);
		sleep(1);
	}
	void arm_deploy(int speed)
	{
		servoObjects().setSpeed(3, speed);
		servoObjects().setSpeed(6, speed);
		servoObjects().deploy(3, 495, 0);
		servoObjects().deploy(6, 812, 0);
		nose_back(speed);
		sleep(1);
	}
	void arm_semi_deploy(int speed)
	{
		servoObjects().setSpeed(3, speed);

		servoObjects().deploy(3, 680, 0);

		sleep(1);
	}
	void arm_right(int speed)
	{
		servoObjects().setSpeed(7, speed);
		servoObjects().deploy(7, 375, 0);
	}
	void arm_right_full(int speed)
	{
		servoObjects().setSpeed(7, speed);
		servoObjects().deploy(7, 215, 0);
	}
	void arm_left_full(int speed)
	{
		servoObjects().setSpeed(7, speed);
		servoObjects().deploy(7, 715, 0);
	}
	void arm_left(int speed)
	{
		servoObjects().setSpeed(7, speed);
		servoObjects().deploy(7, 672, 0);
	}
	void arm_center(int speed)
	{
		servoObjects().setSpeed(7, speed);
		servoObjects().deploy(7, 512, 0);
	}
	void servo_lowspeed()
	{
		servoObjects().setSpeedAll(100);
	}

	void servo_mediumspeed()
	{
		servoObjects().setSpeedAll(200);
	}
	void servo_maxspeed()
	{
		servoObjects().setSpeedAll(1023);
	}

	void servo_init()
	{
		servo_lowspeed();

		//funnyAction_Init();
		//nose_down(150);
		//arm_retract(150);
		//arm_center(150);
		//turn_nene_center();
		//sleep(2);
	}

	void servo_init_end()
	{

	}
	void servo_init_orange()
	{
		servo_lowspeed();

		//arm_right(500);
		//turn_nene_right();
		//nose_up(150);
		//usleep(500000);
	}
	void servo_init_green()
	{
		servo_lowspeed();

		//arm_left(500);
		//turn_nene_left();
		//nose_up(150);
		//usleep(500000);
	}

};

#endif

