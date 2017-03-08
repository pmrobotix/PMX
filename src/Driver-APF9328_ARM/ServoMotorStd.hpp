/*!
 * \file
 * \brief Définition de la classe ServoMotorStd (sur n positions).
 * Permet de gérer les R/C servos (Remote control servo moteur) via FPGA.
 * 
 */

#ifndef ARM_SERVOMOTORSTD_HPP
#define	ARM_SERVOMOTORSTD_HPP

#include <string>

#include "../Log/LoggerFactory.hpp"

#define SERVO_DRIVER_SYSFS_BASE "/sys/class/servo/"
#define SERVO_DRIVER_SERVO_FILE "servo"
#define SERVO_DRIVER_SERVO_POSITION_FILE "desired_position"
#define SERVO_DRIVER_SERVO_SPEED_FILE "speed_step"
#define SERVO_DRIVER_SERVO_ENABLE_FILE "enable"
#define SERVO_DRIVER_SERVO_OFFSET_FILE "offset"
#define SERVO_DRIVER_SERVO_MIN_FILE "lower_boundary"
#define SERVO_DRIVER_SERVO_MAX_FILE "upper_boundary"
#define SERVO_DRIVER_SERVO_CURRENT_POS "current_position"
#define NbMaxServo 10 //32

/*!
 * \brief Cette classe représente l'un des servomoteurs fonctionnant sur
 * n positions du robot et utilisant le "motion system" du FPGA.
 *
 */
class ServoMotorStd
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref ServoMotorStd.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("ServoMotorStd");
		return instance;
	}

	int IDServo_;

	char* enableOpFileName_;

	char* offsetOpFileName_;

	char* speedOpFileName_;

	char* positionOpFileName_;

	char* currentPosOpFileName_;

	static int *servoEnableBuffer_;

	static int *servoMinBuffer_;

	static int *servoMaxBuffer_;

	static int *servoOffsetBuffer_;

	static int *servoPositionBuffer_;

	static int *servoSpeedBuffer_;

	static int *servoInvertedBuffer_;

public:

	/*!
	 * \brief Constructeur de la classe.
	 */
	ServoMotorStd(int ID);

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual inline ~ServoMotorStd()
	{
	}

	/*!
	 * \brief Permet d'activer un servomoteur ou non (= mode freewheeling).
	 * \param servoID : identifiant du servomoteur.
	 * \param value : prend la valeur 0:desactivé ou 1:activé.
	 */
	virtual void setServoEnable(int value);

	/*!
	 * \brief Mets à jour la valeur d'offset du servo associé.
	 */
	virtual void setServoOffset(int);

	/*!
	 * \brief Tourne le servo à la valeur de position demandée.
	 */
	virtual void setServoPosition(int);

	/*!
	 * \brief Change la vitesse.
	 */
	virtual void setServoSpeed(int);

	/*!
	 * \brief indique l'identifiant du servomoteur.
	 */
	virtual int getServoId();

	/*!
	 * \brief indique la valeur courante de la position du servo.
	 */
	virtual int getServoCurrentPosition();

	/*!
	 * \brief indique si le servo doit etre inversé.
	 */
	virtual void isInverted();

private:

	char* getFilename(std::string type);

};

#endif
